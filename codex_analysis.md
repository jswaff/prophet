# Prophet Speed Analysis (Codex)

I reviewed the search, move ordering, move generation, eval, NNUE, and xboard glue code with a focus on runtime hotspots. This is a static analysis pass (no profiler run yet), so priorities are based on engine architecture and code paths hit per node.

## Highest-Impact Opportunities

1. Aspiration windows in iterative deepening
- Location: `src/search/iterate.c:125` and `src/search/iterate.c:126`
- Current behavior: every iteration searches `[-CHECKMATE, +CHECKMATE]`.
- Why it matters: wide windows increase node count significantly once the score is stable.
- Suggestion: center a narrow window around previous iteration score (e.g. +/- 25 to 75 cp), and re-search wider only on fail-low/fail-high.
- Expected impact: high.

2. Add quiet-move history ordering (and optionally counter-moves)
- Location: `src/search/next.c:135` to `src/search/next.c:145`
- Current behavior: quiets are played in generation order after killers.
- Why it matters: after captures/killers, ordering quality drops sharply; this hurts alpha-beta efficiency.
- Suggestion: add a history table (`from,to` or `piece,to`), bump on beta cutoffs, sort/partial-select quiets by history score. Counter-move table is a good complement.
- Expected impact: high.

3. Replace fixed LMR with depth/move-index reduction table
- Location: `src/search/search.c:212` to `src/search/search.c:218`
- Current behavior: fixed reduction (`depth-2`) for later quiets.
- Why it matters: fixed reduction is usually too conservative at deep/late moves and too aggressive in some shallow cases.
- Suggestion: precompute `lmr[depth][move_index]`; reduce more for deeper nodes and later moves, with re-search on fail-high as you already do.
- Expected impact: high.

## Medium-Impact Opportunities

4. Hoist repeated material eval out of the move loop
- Location: `src/search/search.c:180`
- Current behavior: `eval(pos, true, false)` is recomputed per move in futility pruning, but `pos` is unchanged at that point.
- Suggestion: compute once before the `while (next(...))` loop when futility conditions can apply.
- Expected impact: medium (easy win).

5. Add TT usage in quiescence search
- Location: `src/search/qsearch.c:21` to `src/search/qsearch.c:85`
- Current behavior: no hash probe/store in qsearch.
- Why it matters: tactical leaf structures repeat often; TT can prune redundant qsearch trees.
- Suggestion: probe TT at qsearch entry (respect depth semantics for q nodes), store bounds on return.
- Expected impact: medium.

6. Avoid O(n^2) rescans in capture ordering
- Location: `src/search/next.c:57`, `src/search/next.c:94`, `src/search/next.c:165`, `src/search/next.c:182`
- Current behavior: repeatedly scans remaining list to pick best capture (`index_best_capture`).
- Why it matters: done at essentially every interior node; repeated linear scans add up.
- Suggestion: score once and use a partition/selection scheme (or tiny insertion sort) for capture subsets.
- Expected impact: medium.

7. Repetition draw check is linear in half-move clock at every non-root node
- Location: `src/position/draw_rep.c:19` to `src/position/draw_rep.c:24` (called from `src/search/search.c:104`)
- Current behavior: scans undo history from `move_counter - fifty_counter` each time.
- Why it matters: this is on a very hot path and can cost a lot in long quiet positions.
- Suggestion: maintain an incremental repetition counter/map per hash key (or a compact rolling history structure) to make checks O(1) or near O(1).
- Expected impact: medium.

## Lower-Risk Engineering Wins

8. Eliminate per-search heap allocations in hot command paths
- Location: `src/command/xboard/think_and_make_move.c:88` to `src/command/xboard/think_and_make_move.c:113`, `src/search/iterate.c:191` to `src/search/iterate.c:217`
- Current behavior: alloc/free `iterator_options_t`, `iterator_context_t`, and copied `position_t` each search.
- Suggestion: use stack/local structs (or persistent thread-local storage) instead of heap allocation.
- Expected impact: low-to-medium (helps latency jitter, cleaner threading path).

9. Tighten zugzwang gating for null-move pruning
- Location: `src/search/zugzwang.c:10` to `src/search/zugzwang.c:19`, used in `src/search/search.c:135`
- Current behavior: returns true if either side lacks major/minor pieces.
- Why it matters: suppresses null move more often than needed; can reduce pruning and speed.
- Suggestion: base decision on side-to-move only.
- Expected impact: low-to-medium (and likely Elo-positive).

10. Vectorize NNUE clamp stage and avoid float post-processing
- Location: `src/nn/nn_eval.c:33` to `src/nn/nn_eval.c:37`, `src/nn/nn_eval.c:71` to `src/nn/nn_eval.c:74`
- Current behavior: scalar clamp loop, then float math for final score.
- Suggestion: AVX2 clamp/pack path for accumulator -> `int8`, and integer arithmetic for final scaling/rounding.
- Expected impact: medium when NNUE is enabled.

11. Add optional PGO build mode
- Location: `CMakeLists.txt` (release flags)
- Current behavior: LTO is enabled, but no PGO flow.
- Suggestion: add profile-generate/profile-use targets for representative search workloads.
- Expected impact: medium with low code risk.

## Suggested Implementation Order

1. `Aspiration windows` + `history ordering` + `LMR table`
2. `Hoist futility eval` + `qsearch TT`
3. `Repetition check optimization`
4. `Capture-ordering rescan cleanup`
5. `NNUE clamp vectorization` + `PGO`
6. `Heap allocation cleanup` + `zugzwang gating tweak`

## Notes

- This pass is intentionally conservative: I prioritized changes that are likely to improve speed without destabilizing correctness.
- Best next step is a quick measurement loop (fixed-depth suite + tactical set + selfplay sample) before and after each group of changes.
