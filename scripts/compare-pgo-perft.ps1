<#
.SYNOPSIS
Verifies that a PGO-optimized build produces identical perft node counts to
a plain, non-PGO Release build on Windows/MSVC, and reports the timing
difference between them.

.DESCRIPTION
This is the Windows/MSVC counterpart to scripts/compare-pgo-perft.sh.

Movegen is deterministic, so node counts must match exactly -- PGO should
only change speed, never search/movegen correctness (node counts are
compared on their own, separately from timing, since timing naturally
varies run to run and is not a correctness signal). Builds a plain baseline
Release build if one doesn't already exist, then runs perft on both
binaries across a few representative positions (including Kiwipete, a
classic movegen stress-test position), diffs the node counts, and prints
each build's time/rate at the deepest ply plus the speedup.

NOTE: unlike the .sh version, this script has not been verified against a
real MSVC toolchain.

.PARAMETER PgoBuildDir
Directory of an already-built PGO build (default: build-pgo, e.g. via
scripts/build-pgo.ps1).

.PARAMETER BaselineBuildDir
Directory for the plain baseline build (default: build-baseline;
configured+built by this script if not already present).

.PARAMETER PerftDepth
Perft depth to run (default: 6).

.PARAMETER Config
Build configuration (default: Release).

.EXAMPLE
.\scripts\compare-pgo-perft.ps1
#>

[CmdletBinding()]
param(
    [string]$PgoBuildDir = "build-pgo",
    [string]$BaselineBuildDir = "build-baseline",
    [string]$SourceDir = (Resolve-Path (Join-Path $PSScriptRoot "..")).Path,
    [int]$PerftDepth = 6,
    [string]$Config = "Release"
)

$ErrorActionPreference = "Stop"

if (-not [System.IO.Path]::IsPathRooted($PgoBuildDir)) {
    $PgoBuildDir = Join-Path $SourceDir $PgoBuildDir
}
if (-not [System.IO.Path]::IsPathRooted($BaselineBuildDir)) {
    $BaselineBuildDir = Join-Path $SourceDir $BaselineBuildDir
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

# See build-pgo.ps1 for why this uses System.Diagnostics.Process directly
# (LF-only stdin line endings; avoids native-pipe encoding quirks) instead of
# a plain '|' pipe.
function Invoke-ProphetStdin {
    param(
        [string]$ExePath,
        [string[]]$StdinLines
    )

    $psi = New-Object System.Diagnostics.ProcessStartInfo
    $psi.FileName = $ExePath
    $psi.RedirectStandardInput = $true
    $psi.RedirectStandardOutput = $true
    $psi.RedirectStandardError = $true
    $psi.UseShellExecute = $false

    $proc = [System.Diagnostics.Process]::Start($psi)
    $proc.StandardInput.NewLine = "`n"
    foreach ($line in $StdinLines) {
        $proc.StandardInput.WriteLine($line)
    }
    $proc.StandardInput.Close()

    $stdout = $proc.StandardOutput.ReadToEnd()
    $proc.WaitForExit()

    return $stdout
}

function Invoke-Perft {
    param([string]$ExePath, [string]$Fen, [int]$Depth)

    $lines = New-Object System.Collections.Generic.List[string]
    if ($Fen) {
        $lines.Add("setboard $Fen")
    }
    $lines.Add("perft $Depth")
    $lines.Add("quit")

    $output = Invoke-ProphetStdin -ExePath $ExePath -StdinLines $lines
    return ($output -split "`r?`n") | Where-Object { $_ -match '^depth \d+ - nodes: \d+' }
}

if (-not (Find-ProphetExe -Dir $PgoBuildDir -Config $Config)) {
    throw "no built executable found in $PgoBuildDir (run scripts\build-pgo.ps1 first)"
}

Write-Host "==> Configuring + building plain (non-PGO) baseline Release build in $BaselineBuildDir"
& cmake -S $SourceDir -B $BaselineBuildDir -DBUILD_TEST_EXECUTABLE=OFF
if ($LASTEXITCODE -ne 0) { throw "cmake configure (baseline) failed" }
& cmake --build $BaselineBuildDir --config $Config
if ($LASTEXITCODE -ne 0) { throw "cmake build (baseline) failed" }

$PgoExe = Find-ProphetExe -Dir $PgoBuildDir -Config $Config
$BaseExe = Find-ProphetExe -Dir $BaselineBuildDir -Config $Config

Write-Host "PGO binary:      $PgoExe"
Write-Host "Baseline binary: $BaseExe"

# representative positions: empty string means "use default starting
# position" (no 'setboard' sent); the second is Kiwipete, a classic perft
# stress-test position with castling/en-passant/promotions all in play.
$Fens = @(
    "",
    "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1"
)

$allMatched = $true

foreach ($fen in $Fens) {
    $label = if ($fen) { $fen } else { "startpos" }
    Write-Host ""
    Write-Host "=== position: $label (depth $PerftDepth) ==="

    $pgoLines = Invoke-Perft -ExePath $PgoExe -Fen $fen -Depth $PerftDepth
    $baseLines = Invoke-Perft -ExePath $BaseExe -Fen $fen -Depth $PerftDepth

    $pgoNodes = $pgoLines | ForEach-Object { $_ -replace '^(depth \d+) - nodes: (\d+).*', '$1 nodes:$2' }
    $baseNodes = $baseLines | ForEach-Object { $_ -replace '^(depth \d+) - nodes: (\d+).*', '$1 nodes:$2' }

    if (($pgoNodes -join "`n") -eq ($baseNodes -join "`n")) {
        Write-Host "MATCH"
        Write-Host ($pgoNodes | Select-Object -Last 1)
    } else {
        Write-Host "MISMATCH"
        Write-Host "--- PGO ---"
        $pgoNodes | ForEach-Object { Write-Host $_ }
        Write-Host "--- baseline ---"
        $baseNodes | ForEach-Object { Write-Host $_ }
        $allMatched = $false
    }

    $pgoLast = $pgoLines | Select-Object -Last 1
    $baseLast = $baseLines | Select-Object -Last 1

    $pgoMs = [regex]::Match($pgoLast, 'time: (\d+)ms').Groups[1].Value
    $baseMs = [regex]::Match($baseLast, 'time: (\d+)ms').Groups[1].Value
    $pgoRate = [regex]::Match($pgoLast, 'rate: (\d+) kn/s').Groups[1].Value
    $baseRate = [regex]::Match($baseLast, 'rate: (\d+) kn/s').Groups[1].Value

    Write-Host "Timing (depth $PerftDepth):"
    Write-Host "  PGO:      ${pgoMs}ms (${pgoRate} kn/s)"
    Write-Host "  Baseline: ${baseMs}ms (${baseRate} kn/s)"
    if ([int]$pgoMs -gt 0 -and [int]$baseMs -gt 0) {
        $speedup = [int]$baseMs / [int]$pgoMs
        Write-Host ("  Speedup:  {0:N2}x" -f $speedup)
    } else {
        Write-Host "  Speedup:  (too fast at this depth to measure reliably; increase -PerftDepth)"
    }
}

if (-not $allMatched) {
    exit 1
}
