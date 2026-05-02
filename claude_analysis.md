# Prophet Speed Improvement Suggestions

## Search Algorithm Improvements

### 1. Aspiration Windows (High Impact)

`src/search/iterate.c:126` uses `-CHECKMATE` to `+CHECKMATE` on every iteration. Adding aspiration
windows around the previous iteration's score lets most iterations complete with a much smaller tree:

```c
int32_t delta = 50; // centipawns
int32_t alpha_bound = prev_score - delta;
int32_t beta_bound  = prev_score + delta;
// re-search with full window on fail low/high
```

This is typically worth 50-100+ Elo and is the most impactful missing feature.

---

### 2. History Heuristic (High Impact)

The engine uses only killer moves for quiet move ordering. A history table (indexed by
`[piece][to_square]` or `[from][to]`) that accumulates scores for moves that caused beta cutoffs
would substantially improve ordering of non-killer quiet moves.

---

### 3. Hoist `eval()` Out of the Move Loop (Easy Fix)

In `src/search/search.c:178`, `eval(pos, true, false)` is called inside the while-loop but
**before** `apply_move`, so the material score is constant for every move at this node. It is
recomputed on every iteration:

```c
// Current: eval() called for every move in the loop
while (next(pos, &mp, &mo_dto)) {
    if (num_moves_searched > 0 && !incheck && depth < 3 && ...) {
        int32_t mat = eval(pos, true, false);  // unnecessary repeated work
```

Move it above the loop with a conditional:

```c
int32_t mat = (!incheck && depth < 3) ? eval(pos, true, false) : 0;
while (next(pos, &mp, &mo_dto)) {
    if (num_moves_searched > 0 && !incheck && depth < 3 && ...) {
        // use pre-computed mat
```

---

### 4. Fix Zugzwang Check (Correctness + Performance)

`src/search/zugzwang.c` returns `true` if **either** player has no pieces, but null move pruning
only needs to be suppressed when the **side to move** is in potential zugzwang. Currently it skips
null move if the opponent has no pieces, which is overly conservative. Only `pos->player` should
be checked.

---

### 5. Vectorize the Clamp Step in `nn_eval` (AVX2)

`src/nn/nn_eval.c:34-37` clamps 768 int16 accumulator values to int8 in a scalar loop. Since the
rest of the NN forward pass already uses AVX2, this step could use `_mm256_packs_epi16` +
`_mm256_max_epi8` to process 16 values per iteration instead of 1.

---

### 6. LMR Table Instead of Fixed Reduction

`src/search/search.c:214` uses a fixed `depth-2` (R=2) for all LMR cases. Most modern engines
compute a logarithmic reduction table at startup:

```c
// pre-computed at init
int lmr_table[MAX_PLY][MAX_MOVES_PER_PLY];
// lmr_table[d][m] = (int)(log(d) * log(m) / 1.5)
```

This enables more aggressive reductions on later moves at greater depths.

---

### 7. TT Probe in Qsearch

`src/search/qsearch.c` does not probe the hash table at all. Adding a TT probe at the start of
qsearch (exact scores only, or bounds for cutoffs) can eliminate redundant work in tactical
sequences that repeat.

---

### 8. Counter-Move Heuristic

For each `(previous_move, quiet_move)` pair that causes a cutoff, store and try that quiet move
first as a "refutation" of the previous move. Low implementation cost, complements killers well.

---

## Summary

| # | Suggestion                      | Area          | Effort | Expected Gain |
|---|---------------------------------|---------------|--------|---------------|
| 1 | Aspiration windows              | Search        | Medium | High (50-100+ Elo) |
| 2 | History heuristic               | Move ordering | Medium | High          |
| 3 | Hoist `eval()` out of loop      | Search        | Low    | Medium        |
| 4 | Fix zugzwang check              | Search        | Low    | Low-Medium    |
| 5 | AVX2 clamp in nn_eval           | NN inference  | Medium | Medium        |
| 6 | LMR table                       | Search        | Low    | Medium        |
| 7 | TT probe in qsearch             | Search        | Low    | Low-Medium    |
| 8 | Counter-move heuristic          | Move ordering | Low    | Low-Medium    |
