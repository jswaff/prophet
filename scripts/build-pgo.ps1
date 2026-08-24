<#
.SYNOPSIS
Two-pass Profile-Guided Optimization (PGO) build for prophet on Windows/MSVC.

.DESCRIPTION
Pass 1: configure + build an instrumented binary (-DPROFILE_GUIDED=GENERATE),
then run it against a training workload (perft + a few searches on
representative positions) to collect profile data (.pgc files).
Pass 2: reconfigure + rebuild an optimized binary (-DPROFILE_GUIDED=USE),
which merges the collected .pgc data into the .pgd profile database.

This is the Windows/MSVC counterpart to scripts/build-pgo.sh (GCC/Clang).

NOTE: unlike build-pgo.sh, this script has not been verified against a real
MSVC toolchain. MSVC's PGO merge behavior (whether the USE-mode link step
auto-merges .pgc into .pgd, or requires an explicit `pgomgr /merge` step
first) can vary by Visual Studio version. If Pass 2 doesn't seem to be using
the collected profile data (e.g. no speedup, or a warning about missing/stale
profile data), try running `pgomgr /merge <path-to>.pgd` by hand on the .pgd
under the pgo-data directory before re-running Pass 2, and let this script's
maintainers know so it can be fixed for everyone.

.PARAMETER BuildDir
Build directory to use (default: build-pgo).

.PARAMETER PerftDepth
Perft depth for the training run (default: 4). Kept shallow on purpose:
perft is pure move generation with no eval/search/NN involved, and its node
count would otherwise dwarf everything else being profiled.

.PARAMETER Sd
Search depth ('sd') for one untimed training search per FEN, in addition to
the timed ones below (default: 8).

.PARAMETER Timeout
Time-control backstop in seconds ('st'), sent before the untimed 'go'
alongside 'sd' (default: 30). 'sd' alone only bounds the number of
iterations, not the wall-clock cost of reaching one -- without a time
control, a slow/tactical position can run 'go' indefinitely and hang the
subsequent 'ping'.

.PARAMETER TcInc
Increment (seconds) used for the timed training searches below, via the
real 'level'/'time' clock formula rather than a fixed 'st' budget (default:
0.5). Match this to your target time control's increment so the profile
reflects real move budgets.

.PARAMETER TimePressureMs
Simulated clock time remaining (milliseconds) for one timed training search
per FEN (default: 1000, 300, 60). This is what exercises the mid-search
time-cutoff path that dominates real play at fast time controls -- without
it, PGO trains almost entirely on searches that run to full depth, which
real bullet/blitz moves rarely do. Note the engine's time formula
(base/25 + increment) is increment-dominated at these magnitudes, so with
the default TcInc these mostly collapse to a similar per-move budget --
which is itself a faithful reproduction of a fast increment-based game.
Pass values matching your own target time control's actual remaining-clock
range for anything else.

.PARAMETER Fen
Representative FEN(s) to train on (default: four built-in FENs spanning
opening, tactical, quiet middlegame, and endgame positions).

.PARAMETER Nn
Neural network weights file (default: test/resources/nn.txt).

.PARAMETER Config
Build configuration (default: Release).

.PARAMETER Clean
Wipe BuildDir before starting.

.PARAMETER CMakeArg
Extra argument(s) passed through to both cmake configure invocations (e.g.
-CMakeArg -DWITH_AVX=OFF).

.EXAMPLE
.\scripts\build-pgo.ps1

.EXAMPLE
.\scripts\build-pgo.ps1 -Clean -PerftDepth 6 -Timeout 60
#>

[CmdletBinding()]
param(
    [string]$BuildDir = "build-pgo",
    [string]$SourceDir = (Resolve-Path (Join-Path $PSScriptRoot "..")).Path,
    [int]$PerftDepth = 4,
    [int]$Sd = 8,
    [int]$Timeout = 30,
    [double]$TcInc = 0.5,
    [int[]]$TimePressureMs = @(),
    [string[]]$Fen = @(),
    [string]$Nn,
    [string]$Config = "Release",
    [switch]$Clean,
    [string[]]$CMakeArg = @()
)

$ErrorActionPreference = "Stop"

if (-not $Nn) {
    $Nn = Join-Path $SourceDir "test\resources\nn.txt"
}

if (-not [System.IO.Path]::IsPathRooted($BuildDir)) {
    $BuildDir = Join-Path $SourceDir $BuildDir
}

$DefaultFens = @(
    "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1",
    "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1",
    "r4rk1/1pp1qppp/p1np1n2/2b1p1B1/2B1P1b1/P1NP1N2/1PP2PPP/R2Q1RK1 w - - 0 10",
    "8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - 0 1"
)
if ($Fen.Count -eq 0) { $Fen = $DefaultFens }

$DefaultTimePressureMs = @(1000, 300, 60)
if ($TimePressureMs.Count -eq 0) { $TimePressureMs = $DefaultTimePressureMs }

if (-not (Get-Command cmake -ErrorAction SilentlyContinue)) {
    throw "cmake not found in PATH"
}

# Sends $StdinLines to $ExePath's stdin with LF-only line endings (this
# engine's stdin parser strips a trailing '\n' but not '\r', so PowerShell's
# default CRLF pipeline behavior would leave a stray '\r' on every command
# and break exact-match parsing, e.g. 'go' vs "go`r"). Using
# System.Diagnostics.Process directly (rather than a native '|' pipe) also
# sidesteps PowerShell's default console-encoding quirks (e.g. a BOM) when
# piping a string to a native executable's stdin.
function Invoke-ProphetStdin {
    param(
        [string]$ExePath,
        [string[]]$ExeArgs,
        [string[]]$StdinLines,
        [hashtable]$EnvVars = @{}
    )

    $psi = New-Object System.Diagnostics.ProcessStartInfo
    $psi.FileName = $ExePath
    $psi.Arguments = ($ExeArgs -join " ")
    $psi.RedirectStandardInput = $true
    $psi.RedirectStandardOutput = $true
    $psi.RedirectStandardError = $true
    $psi.UseShellExecute = $false
    $psi.WorkingDirectory = $SourceDir

    foreach ($key in $EnvVars.Keys) {
        $psi.EnvironmentVariables[$key] = $EnvVars[$key]
    }

    $proc = [System.Diagnostics.Process]::Start($psi)
    $proc.StandardInput.NewLine = "`n"
    foreach ($line in $StdinLines) {
        $proc.StandardInput.WriteLine($line)
    }
    $proc.StandardInput.Close()

    $stdout = $proc.StandardOutput.ReadToEnd()
    $stderr = $proc.StandardError.ReadToEnd()
    $proc.WaitForExit()

    return [PSCustomObject]@{
        ExitCode = $proc.ExitCode
        StdOut   = $stdout
        StdErr   = $stderr
    }
}

function Find-ProphetExe {
    param([string]$Dir, [string]$Config)
    foreach ($name in @("prophet.exe", "prophet-avx2.exe")) {
        foreach ($candidate in @((Join-Path $Dir "$Config\$name"), (Join-Path $Dir $name))) {
            if (Test-Path $candidate) { return $candidate }
        }
    }
    return $null
}

if ($Clean -and (Test-Path $BuildDir)) {
    Remove-Item -Recurse -Force $BuildDir
}
$PgoDataDir = Join-Path $BuildDir "pgo-data"
New-Item -ItemType Directory -Force -Path $PgoDataDir | Out-Null

Write-Host "==> Pass 1/2: configuring + building instrumented (GENERATE) binary"
$configureGenerateArgs = @(
    "-S", $SourceDir,
    "-B", $BuildDir,
    "-DPROFILE_GUIDED=GENERATE",
    "-DPROFILE_GUIDED_DATA_DIR=$PgoDataDir",
    "-DBUILD_TEST_EXECUTABLE=OFF"
) + $CMakeArg
& cmake @configureGenerateArgs
if ($LASTEXITCODE -ne 0) { throw "cmake configure (GENERATE) failed" }

& cmake --build $BuildDir --config $Config
if ($LASTEXITCODE -ne 0) { throw "cmake build (GENERATE) failed" }

$ExePath = Find-ProphetExe -Dir $BuildDir -Config $Config
if (-not $ExePath) {
    throw "could not locate built prophet executable under $BuildDir"
}

Write-Host "==> Running training workload against $ExePath"

$stdinLines = New-Object System.Collections.Generic.List[string]
$stdinLines.Add("perft $PerftDepth")
foreach ($f in $Fen) {
    # one untimed, depth-limited search -- exercises full-depth eval/search
    $stdinLines.Add("new")
    $stdinLines.Add("setboard $f")
    $stdinLines.Add("st $Timeout")
    $stdinLines.Add("sd $Sd")
    $stdinLines.Add("go")
    $stdinLines.Add("ping 1")

    # plus several clock-limited searches using the same 'level'/'time'
    # formula a real game uses, at a spread of remaining-time values. This
    # is what makes the mid-search time-cutoff path (the one that actually
    # fires on most moves at fast time controls) show up in the profile at
    # all -- the untimed search above never hits it.
    foreach ($ms in $TimePressureMs) {
        $stdinLines.Add("new")
        $stdinLines.Add("setboard $f")
        $stdinLines.Add("level 0 0:01 $TcInc")
        $stdinLines.Add("time $([int]($ms / 10))")
        $stdinLines.Add("go")
        $stdinLines.Add("ping 1")
    }
}
$stdinLines.Add("quit")

# MSVC instrumented binaries write .pgc files to the directory named by the
# VCPROFILE_PATH environment variable (or the current directory if unset) --
# point it at pgo-data so profile output lands next to the .pgd, matching
# the GCC/Clang scripts' convention of keeping everything under
# <builddir>/pgo-data.
$result = Invoke-ProphetStdin -ExePath $ExePath -ExeArgs @("-n", "`"$Nn`"") -StdinLines $stdinLines -EnvVars @{ VCPROFILE_PATH = $PgoDataDir }
if ($result.ExitCode -ne 0) {
    Write-Host $result.StdOut
    Write-Host $result.StdErr
    throw "training run exited with code $($result.ExitCode)"
}

$pgcFiles = Get-ChildItem -Path $PgoDataDir -Filter "*.pgc" -ErrorAction SilentlyContinue
if (-not $pgcFiles) {
    Write-Host $result.StdOut
    Write-Host $result.StdErr
    throw "no profile data (.pgc) found in $PgoDataDir after training run"
}
Write-Host "==> Collected $($pgcFiles.Count) .pgc file(s) in $PgoDataDir"

Write-Host "==> Pass 2/2: reconfiguring + rebuilding optimized (USE) binary"
$configureUseArgs = @(
    "-S", $SourceDir,
    "-B", $BuildDir,
    "-DPROFILE_GUIDED=USE"
) + $CMakeArg
& cmake @configureUseArgs
if ($LASTEXITCODE -ne 0) { throw "cmake configure (USE) failed" }

& cmake --build $BuildDir --config $Config --clean-first
if ($LASTEXITCODE -ne 0) { throw "cmake build (USE) failed" }

Write-Host "==> Done. Optimized binary: $ExePath"
