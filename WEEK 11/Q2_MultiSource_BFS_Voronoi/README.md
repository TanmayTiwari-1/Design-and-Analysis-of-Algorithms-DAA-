# Q2 – Multi-Source BFS for Radius-Constrained Voronoi Cell Partitioning

## Problem
Unweighted undirected graph, `K` hubs `h_1..h_K`, radius `R`. For each vertex `v` output the closest hub index (ties → smallest index) and the distance, or `-1` if `min_i dist(v,h_i) > R`.

## Algorithm
Running one BFS per hub would cost `O(K(V+E))`. Instead, put **all hubs in the queue at distance 0** and run a **single BFS**:

1. `dist[h_i] = 0`, `owner[h_i] = i` for all hubs (hubs inserted in index order; if the same vertex is listed twice, the smaller index is kept).
2. Pop `u`. If `dist[u] == R`, do not expand it (everything farther than `R` stays `-1`).
3. For each neighbour `v` of `u`:
   * unvisited: `dist[v] = dist[u]+1`, `owner[v] = owner[u]`, enqueue;
   * already at the same next level (`dist[v] == dist[u]+1`) and `owner[u] < owner[v]`: set `owner[v] = owner[u]` (**deterministic tie-break**).

Why the tie-break is right: `owner[u]` can only be changed by vertices one level closer, and BFS dequeues all of level `d-1` before any of level `d`. So when `u` is dequeued its owner is already final, and the smallest index among all hubs at distance `dist(v)` propagates correctly by induction on the level.

## Input representation
Undirected adjacency list in **CSR** form (`start[]`, `adj[]`), each edge stored twice. `O(V+E)` memory, cache-friendly scans.

## Input format (vertices 1-indexed)
```
n m K R
h_1 h_2 ... h_K
u v          (m lines, undirected edge)
```
Output: table `Vertex  Owner  Distance` (`-1` owner and `-` distance when outside coverage).

## Complexity
| | |
|---|---|
| Time | **O(V + E + K)** – every vertex is enqueued once, every adjacency entry scanned once |
| Space | **O(V + E)** |

## Build & run
```
gcc -Wall -Wextra -O2 -o Q2_prog q2_voronoi_bfs.c
./Q2_prog < sample_input.txt        # R=1: shows unassigned (-1) vertices
./Q2_prog < sample_input2.txt       # shows tie-breaking (vertex 5 is 3 from both hubs -> owner 1)
```

## Validation
* `../tests/run_tests.py` checks against a per-hub BFS reference on 400 random graphs (disconnected graphs, duplicate hubs, `R = 0`, many ties): **all pass**.
* Scale test: V = 500,000, E = 1,000,000, K = 1,000 ran in about 0.3 s.
* Terminal screenshots: `q2_sample1.png`, `q2_sample2_tiebreak.png`.
