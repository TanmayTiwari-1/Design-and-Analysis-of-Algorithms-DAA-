# Q5 — Lexicographically Smallest Connected Component Permutation

**Files:** `q5_smallest_perm.c` · `input.txt` · `output.txt` · `terminal_output.png` · `runtime_graph.png`
**Run:** `gcc -O2 -o q5 q5_smallest_perm.c && ./q5 < input.txt`

**Input format:** `n`, then the permutation `A[1..n]`, then `m`, then m lines `u v` (allowed swaps between positions, 1-indexed).
**Output:** the lexicographically smallest permutation reachable.

## Input representation
The swaps are edges handled directly by the DSU, so no graph is stored.

## Idea
Positions joined by swaps form a connected component, and within one component the values can be rearranged in any order. So within each component the sorted values go onto the sorted positions.

Implementation: sort pairs `(root, position)` and pairs `(root, value)` by root, then by the second field. Both sorted lists group identically, so the k-th position receives the k-th value.

## Complexity
- **Time:** O(n log n + m · α(n)), dominated by the sort.
- **Space:** O(n).

## Verification
Matched a brute-force per-component sort on 500 random inputs (0 mismatches).
The runtime graph follows an N log N trend.
