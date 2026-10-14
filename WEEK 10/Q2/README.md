# Q2 — Dynamic Connected Components in a Grid (Number of Islands II)

**Files:** `q2_islands.c` · `input.txt` · `output.txt` · `terminal_output.png` · `runtime_graph.png`
**Run:** `gcc -O2 -o q2 q2_islands.c && ./q2 < input.txt`

**Input format:** `R C K`, then K lines `r c` (0-indexed cells to turn into land).
**Output:** number of islands after each operation.

## Input representation
The grid is flattened: `id = r * C + c`. A DSU over the ids plus a `land[]` flag array is all that is stored; there is no explicit graph.

## Idea
- Adding a new land cell: `count++`.
- For each of its 4 land neighbours, a successful union (different roots) does `count--`.
- Adding a cell that is already land changes nothing.

## Complexity
- **Time:** O(R·C + K · α(R·C)); each operation touches at most 4 neighbours.
- **Space:** O(R·C).

## Verification
Matched a brute-force flood-fill count on 500 random inputs (0 mismatches).
The runtime graph shows near-linear growth in K.
