# Speed Improvement Analysis

Scope:
- I did not read `claude_analysis.md` or the previous `codex_analysis.md`.
- I reviewed the engine sources, build setup, and ran one lightweight baseline: `printf 'perft 5\nquit\n' | ./build/prophet-avx2`, which reported depth-5 perft in 323 ms, about 15,017 kn/s.

## Highest-Impact Opportunities

### 1. Stop validating legality by full make/check/unmake on every candidate move

This is the biggest structural cost I found.

Relevant code:
- `src/search/search.c:188-194`
- `src/search/qsearch.c:60-66`
- `src/movegen/movegen.c:34-50`

Today the engine:
- generates pseudo-legal moves,
- `apply_move()`s each one,
- calls `in_check()` to see whether the move was legal,
- then `undo_move()`s it.

That is happening:
- in the main search,
- in quiescence,
- and even in `gen_legal_moves()` at the root.

Why this is expensive:
- `apply_move()` and `undo_move()` touch a lot of state, including hashes and optional NNUE accumulators.
- `in_check()` relies on attack generation, including sliding attacks.
- this amplifies work on every node, not just interesting nodes.

Best improvement:
- add a fast legality filter before make/unmake:
  - track pinned pieces,
  - handle king moves with attack masks,
  - handle en passant legality separately,
  - or add an evasion generator for check nodes.

Expected impact:
- high. This should reduce total node cost materially in both search and qsearch.

### 2. Avoid calling full `eval()` inside futility pruning

Relevant code:
- `src/search/search.c:175-185`
- `src/eval/eval.c:150-240`

The futility check computes:
- `eval(pos, true, false)`

Even with `material_only=true`, that still calls multiple helper layers and recomputes material on demand. It is cheaper than full eval, but it is still happening inside a hot move loop.

Better options:
- keep an incremental material score in `position_t`,
- or cache a cheap `static_material` field updated by `apply_move()` / `undo_move()`,
- or precompute the parent node’s material score once and pass it down.

Expected impact:
- high for low-depth nodes, because this code is inside a pruning condition that runs often.

### 3. Rework move ordering to avoid repeated rescans of the capture list

Relevant code:
- `src/search/next.c:56-95`
- `src/search/next.c:148-175`
- `src/search/next.c:182-198`

`next()` repeatedly scans `[current, end)` to find the best capture, then does it again for deferred bad captures. That is effectively a selection-sort style approach in a hot path.

Why this matters:
- move ordering runs at every node,
- SEE can already be expensive,
- repeated linear rescans multiply the cost of large capture lists.

Better options:
- score captures once and partition into:
  - winning/promotions,
  - neutral,
  - losing,
- then partially sort only the winning bucket,
- or use insertion sort on the small generated list once per stage,
- or keep separate arrays for good captures / bad captures / quiets.

Expected impact:
- high to medium, especially in tactical positions and qsearch-heavy searches.

### 4. Reduce repeated `good_move()` validation for hash and killer moves

Relevant code:
- `src/search/next.c:27-33`
- `src/search/next.c:100-117`
- `src/movegen/good_move.c:23-135`

Hash and killer moves are validated with `good_move()`, which can call sliding move generators and attack tests for castling. That is reasonable for correctness, but it still adds nontrivial work on hot entry points.

Ways to improve:
- store move metadata that lets you reject obviously stale killers faster,
- keep a lightweight pseudo-legality test specialized for hash/killer validation,
- avoid revalidating the PV move when it already came from the previous legal PV,
- consider storing a compact move signature tied to `from`, `to`, and moved piece in TT entries and validating with simpler occupancy checks first.

Expected impact:
- medium. Smaller than legality generation, but still on a very hot path.

### 5. Handcrafted eval is fully recomputed at each static-eval site

Relevant code:
- `src/search/qsearch.c:36`
- `src/eval/eval.c:178-236`

When NNUE is off, every qsearch stand-pat does a fresh handcrafted evaluation:
- pawn hash probe,
- loops over all pawns, knights, bishops, rooks, queens,
- king eval,
- tapering.

The pawn hash helps, but the rest is still rebuilt from scratch every time.

Best improvement:
- add an incremental handcrafted eval accumulator in `position_t`.

Practical path:
- first make material and PST terms incremental,
- keep pawn structure behind the pawn hash as now,
- leave more complex king-safety terms as recompute-only if needed,
- compare speed/strength before pushing further.

Expected impact:
- high if handcrafted eval is the main engine mode.
- lower if NNUE is always on.

## Medium-Impact Opportunities

### 6. Attack detection likely dominates king-safety and legality checks

Relevant code:
- `src/movegen/attacked.c:13-31`
- `src/search/search.c:191`
- `src/search/search.c:197`
- `src/search/qsearch.c:63`

`attacked()` recomputes sliding attacks with `get_rook_moves()` and `get_bishop_moves()` each time. That is normal, but it gets called in several hot places:
- legal move verification,
- check extensions,
- castling validation.

Possible improvements:
- specialize `in_check()` for the king square rather than routing through general attack helpers everywhere,
- cache occupancy-derived attack info per node when profitable,
- or split attack tests into fast non-slider checks first, sliders second.

Expected impact:
- medium, and it compounds with item 1.

### 7. NNUE path still does scalar clamp/pack and float post-processing

Relevant code:
- `src/nn/nn_eval.c:32-75`

The AVX2 path accelerates the hidden layer dot product, which is good, but:
- building `L1` is still scalar,
- the final score uses float math and rounding,
- loads are unaligned.

Possible improvements:
- vectorize clamp-and-pack for `L1`,
- store aligned NN weights and accumulators,
- replace float conversion with fixed-point integer math if it preserves output exactly,
- benchmark fused handling of both outputs in one pass.

Expected impact:
- medium in NNUE mode.
- low if search overhead dominates more than eval.

### 8. TT replacement is simple and cheap, but probe/store policy may be leaving pruning on the table

Relevant code:
- `src/hash/hash_probe.c:15-33`
- `src/hash/hash_store.c:13-68`

This is more of a speed-through-better-hit-rate idea than a raw micro-optimization.

Observations:
- probe is lean,
- replacement is depth-based only,
- aging-based replacement was disabled.

Possible experiments:
- keep one always-replace slot plus one depth-preferred slot,
- separate PV/exact entries from bounds,
- test a two-tier bucket policy rather than pure min-depth replacement.

Expected impact:
- medium if it improves cutoff quality.
- not guaranteed; this needs A/B testing because it affects strength too.

## Lower-Impact But Easy Wins

### 9. Remove heap allocation from `iterate_from_position()`

Relevant code:
- `src/search/iterate.c:185-217`

`iterator_options_t` and `iterator_context_t` are allocated with `malloc()` for each API call even though both are tiny and trivially stack-allocatable.

Expected impact:
- low for long searches,
- but easy cleanup with zero downside.

### 10. Push the build harder with optional PGO and native tuning

Relevant code:
- `CMakeLists.txt:17-24`
- `CMakeLists.txt:182-191`

Current build already has:
- AVX2/BMI2,
- LTO,
- Release mode defaults.

What is still missing:
- profile-guided optimization,
- optional `-march=native` or architecture-specific release presets,
- possibly function/data section tuning if it benchmarks well.

Recommendation:
- add a `PROFILE_GUIDED` option and benchmark with a realistic search workload,
- add an opt-in `NATIVE_TUNE` preset for local builds,
- keep portable release defaults for shipped binaries.

Expected impact:
- medium if PGO is used,
- small otherwise.

## Suggested Implementation Order

1. Replace pseudo-legal-plus-verify flow with more direct legality handling.
2. Remove full/static eval work from futility pruning.
3. Rework move ordering to avoid repeated capture rescans.
4. Optimize attack/check detection around king legality.
5. If handcrafted eval is important, start incremental PST/material evaluation.
6. Then benchmark NNUE-specific improvements and PGO.

## What I Would Measure Next

- NPS before/after each change on a fixed depth search suite.
- qnodes as a percentage of total nodes.
- average legal-move generation cost at root and interior nodes.
- TT hit rate and fail-high rate after any TT policy changes.
- NNUE eval throughput separately from full search throughput.

## Bottom Line

The largest speed gains are most likely to come from cutting node cost in the move/legality path, not from isolated micro-optimizations. The current engine appears reasonably optimized at the build/intrinsics level already, so the best remaining gains are in:
- legality handling,
- repeated eval work,
- move ordering overhead,
- attack/check computation frequency.
