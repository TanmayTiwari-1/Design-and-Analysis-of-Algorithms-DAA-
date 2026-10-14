# DAA Lab-10 — Graphs & Disjoint Set Union

Compile: `gcc -O2 -o qN src/<file>.c`  Run: `./qN < input/qN.txt`

Every program was checked against a naive Python brute force on 500 random
small inputs each (`tools/test_bruteforce.py`): 0 mismatches.
Vertices are **1-indexed** in Q1, Q3, Q4, Q5; grid cells are **0-indexed** in Q2.

DSU used everywhere: **union by size + path compression/halving**, so a
find/union costs amortised O(α(n)) ≈ O(1) (α = inverse Ackermann).

---

## Q1 — Online bipartiteness (`src/q1_bipartite.c`)
**Representation:** edges read one at a time (online), so no adjacency list is
stored — only the DSU arrays `parent`, `parity`, `size`.
**Idea:** `parity[x]` = colour difference between x and its parent. For edge
(u,v): if find(u)==find(v) then it is legal only when parity(u)≠parity(v)
(equal parity closes an odd cycle → reject). Otherwise merge and set the
attached root's parity to `d[u]^d[v]^1` so u and v get different colours.
**Time:** O((V+E)·α(V)).  **Space:** O(V).

## Q2 — Number of Islands II (`src/q2_islands.c`)
**Representation:** the grid is flattened, id = r·C + c, with a DSU over ids
and a `land[]` flag array (no explicit graph).
**Idea:** adding a land cell does `count++`; each successful union with one of
its 4 land neighbours does `count--`. Duplicate positions change nothing.
**Time:** O(R·C + K·α(R·C)) (4 neighbours per op = constant).  **Space:** O(R·C).

## Q3 — Max weight component with size ≤ K (`src/q3_maxweight_size.c`)
**Representation:** edges arrive sequentially, so only DSU arrays are needed:
`size[root]` and `sum[root]` (total vertex weight).
**Idea:** union merges size and sum. After each edge scan all roots and take
the largest `sum` among roots with `size ≤ K` (0 if none). A scan is needed
because a component that grows past K stops counting, so the answer can drop.
**Time:** O(E·V) total (O(V) scan per edge + O(E·α) unions).  **Space:** O(V).
*(Possible speed-up: keep a max-heap / multiset of valid component sums to get
O(E log V); the plain scan was chosen for simplicity.)*
Assumes non-negative vertex weights.

## Q4 — Reverse time-step DSU (`src/q4_reverse_dsu.c`)
**Representation:** adjacency list stored in arrays (`head/nxt/to`, 2E entries)
because we must look at a vertex's neighbours when it is re-added.
**Idea:** DSU cannot split, so run time backwards: start with nothing, add
p_V, p_{V-1}, …, and after re-adding p_{i+1} record the best component sum as
the answer for "after deleting p_1..p_i". `res[V] = 0`.
**Time:** O((V+E)·α(V)).  **Space:** O(V+E). Assumes non-negative weights.

## Q5 — Lexicographically smallest permutation (`src/q5_smallest_perm.c`)
**Representation:** swaps are edges, handled directly by the DSU (no stored graph).
**Idea:** positions in one component can be permuted arbitrarily. Sort pairs
(root, position) and pairs (root, value); both lists group the same way, so the
k-th position receives the k-th value → smallest values on the earliest positions.
**Time:** O(n log n + m·α(n)) (dominated by the sort).  **Space:** O(n).

---

| Q | Time | Space |
|---|------|-------|
| 1 | O((V+E)α(V)) | O(V) |
| 2 | O(R·C + Kα) | O(R·C) |
| 3 | O(E·V) | O(V) |
| 4 | O((V+E)α(V)) | O(V+E) |
| 5 | O(n log n + mα(n)) | O(n) |

Folders: `src/` C code · `input/` sample inputs · `output/` sample outputs ·
`screenshots/` terminal PNGs · `graphs/` runtime-vs-N plots · `tools/` tests & asset scripts.
