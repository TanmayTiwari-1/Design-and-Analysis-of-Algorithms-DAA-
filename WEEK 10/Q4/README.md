# Q4 — Reverse Time-Step DSU Application

**Files:** `q4_reverse_dsu.c` · `input.txt` · `output.txt` · `terminal_output.png` · `runtime_graph.png`
**Run:** `gcc -O2 -o q4 q4_reverse_dsu.c && ./q4 < input.txt`

**Input format:** `V E`, then V vertex weights, then E lines `u v` (1-indexed), then the deletion order `p1 ... pV`.
**Output:** after each deletion, the maximum component sum among the remaining vertices.

## Input representation
An adjacency list stored in arrays (`head / nxt / to`, 2E entries), because the neighbours of a vertex must be visited when it is added back.

## Idea
A DSU can merge but cannot split, so time is run backwards:
1. Start with no vertices; the answer after deleting all V vertices is 0.
2. Re-add p_V, p_{V-1}, ..., p_2 in that order. When a vertex is added, union it with every already-active neighbour.
3. After re-adding p_{i+1}, the best component sum so far is the answer for "after deleting p_1 .. p_i".

## Complexity
- **Time:** O((V + E) · α(V)).
- **Space:** O(V + E).

## Assumption
Vertex weights are non-negative, so merging never lowers the running maximum.

## Verification
Matched a brute-force recomputation of components after every deletion on 500 random inputs (0 mismatches).
The runtime graph shows near-linear growth.
