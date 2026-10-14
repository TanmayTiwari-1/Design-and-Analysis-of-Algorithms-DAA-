# Q1 — Online Graph-Bipartiteness Testing

**Files:** `q1_bipartite.c` · `input.txt` · `output.txt` · `terminal_output.png` · `runtime_graph.png`
**Run:** `gcc -O2 -o q1 q1_bipartite.c && ./q1 < input.txt`

**Input format:** `V E`, then E lines `u v` (vertices 1-indexed).
**Output:** `ADDED` or `REJECTED` for every arriving edge.

## Input representation
Edges arrive online, so no adjacency list is stored. Only the DSU arrays `parent`, `parity` and `size` are kept.

## Idea
`parity[x]` is the colour difference between x and its parent. For an edge (u,v):
- If find(u) == find(v): the edge is legal only when parity(u) != parity(v). Equal parity means the edge closes an odd cycle, so it is rejected (this also rejects self-loops).
- Otherwise merge the two sets (union by size) and set the attached root's parity to `d[u] ^ d[v] ^ 1`, which forces u and v to get different colours.

## Complexity
- **Time:** O((V + E) · α(V)), where α is the inverse Ackermann function (effectively constant).
- **Space:** O(V).

## Verification
Matched a brute-force BFS 2-colouring check on 500 random inputs (0 mismatches).
The runtime graph shows near-linear growth in input size.
