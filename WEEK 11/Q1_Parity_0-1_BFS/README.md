# Q1 – Shortest Path on Parity-Constrained Graphs via 0-1 BFS

## Problem
Directed graph, edge weights in {0,1}, source `S`, penalty `C ≥ 0`.
For a path `P`: `Cost(P) = W(P) + (0 if k1(P) even, C if k1(P) odd)`.
Find the minimum `Cost` from `S` to every vertex (or report UNREACHABLE).

## Algorithm
The penalty depends only on the **parity of the number of weight-1 edges**, so we track parity as part of the state.

* State = `(v, p)`, `p` = parity of weight-1 edges used so far.
* Edge `(u→v, 0)`: `(u,p) → (v,p)`, cost 0.
* Edge `(u→v, 1)`: `(u,p) → (v,1-p)`, cost 1.
* Run **0-1 BFS** (a deque: 0-edges push to the front, 1-edges to the back) from `(S,0)`.
  `dist[v][p]` = minimum `W` over paths reaching `v` with parity `p`.
* Answer: `ans[v] = min( dist[v][0], dist[v][1] + C )`; if both are infinite, `v` is unreachable.

Why it is correct: for a fixed final parity, the penalty is a constant, so the best path of that parity is simply the one with the smallest `W`. Taking the min over the two parities gives the optimum.

**Assumption:** "path" is treated as a *walk* (vertices/edges may repeat). Allowing repeats is what makes the parity trick work; e.g. in `sample_input.txt`, vertex 3 reaches cost 4 by walking around a cycle to fix the parity (a simple path would cost 6). Restricting to simple paths would be a much harder problem.

## Input representation
Adjacency list in **CSR** (compressed sparse row) form: `start[]`, `to[]`, `wt[]`. Neighbour scans are contiguous in memory, memory is `O(V+E)`.

## Input format (vertices 1-indexed)
```
n m S C
u v w        (m lines: directed edge u->v with weight w in {0,1})
```
Output: one row per vertex with its minimum cost and the (W, parity) that achieves it, or `UNREACHABLE`.

## Complexity
| | |
|---|---|
| States | `2V` |
| Transitions | `2E` (each edge used from both parities) |
| Time | **O(V + E)** – each state is finalised once, deque ops are O(1) |
| Space | **O(V + E)** |

(A normal Dijkstra on the same graph would be O((V+E) log V); 0-1 BFS removes the log.)

## Build & run
```
gcc -Wall -Wextra -O2 -o Q1_prog q1_parity_01bfs.c
./Q1_prog < sample_input.txt
./Q1_prog < sample_input2.txt
```

## Validation
* `../tests/run_tests.py` compares the program with an independent brute force (DP over the exact number of weight-1 edges) on 400 random graphs, including unreachable vertices, self-loops, parallel edges and `C = 0`: **all pass**.
* Scale test: V = 500,000, E = 1,000,000 ran in about 0.4 s.
* Terminal screenshots: `q1_sample1.png`, `q1_sample2.png`.
