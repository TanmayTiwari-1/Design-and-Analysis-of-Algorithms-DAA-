# Q3 — Maximum Weight Component with Size Constraints

**Files:** `q3_maxweight_size.c` · `input.txt` · `output.txt` · `terminal_output.png` · `runtime_graph.png`
**Run:** `gcc -O2 -o q3 q3_maxweight_size.c && ./q3 < input.txt`

**Input format:** `V K`, then V vertex weights, then `E`, then E lines `u v w` (vertices 1-indexed; the edge weight `w` is read but not needed for the answer).
**Output:** after each edge, the maximum total vertex weight among components of size <= K (0 if none).

## Input representation
Edges arrive sequentially, so only DSU arrays are kept: `size[root]` and `sum[root]` (total vertex weight of the component).

## Idea
A union merges both the size and the sum into the new root. After every edge, scan all roots and take the largest `sum` among roots with `size <= K`. A scan is needed because a component that grows beyond K stops counting, so the answer can go down.

## Complexity
- **Time:** O(E · V) overall (an O(V) scan per edge, plus O(E · α) for the unions).
- **Space:** O(V).
- A max-heap or multiset of valid component sums would reduce this to O(E log V); the plain scan was chosen for simplicity.

## Assumption
Vertex weights are non-negative.

## Verification
Matched a brute-force recomputation of components after every edge on 500 random inputs (0 mismatches).
The runtime graph shows quadratic growth, as expected from O(E · V) with E = V = N.
