# Q3 – Bidirectional BFS for Shortest Transformation Sequence (Word Graph)

## Problem
Dictionary `D` of `N ≤ 10^5` words of length `L ≤ 10` (lowercase). Transform `A` into `B` by changing one letter at a time, every word after `A` must be in `D`. Output the minimum number of mutations `k`, or `-1`.

## Algorithm
**Graph:** one node per word; edge between words at Hamming distance exactly 1.

**Finding neighbours without comparing all pairs (O(N²L) is too slow).**
Two words differ in exactly one position `i` ⇔ they are equal once position `i` is replaced by a wildcard. For each position `i`:
1. Encode each word as a base-27 integer (letters 1..26, wildcard 0; `27^10 < 2^63`, so it fits a 64-bit key) with position `i` blanked.
2. Sort words by that key; equal keys form a *group*, i.e. a clique of mutual neighbours, stored as one contiguous range.

A word's neighbours via position `i` are exactly its group's range.

**Search: bidirectional BFS.**
* Forward search from `A`, backward search from `B`; always expand one full level of the side with the **smaller frontier**.
* Each `(position, group)` is scanned at most once per direction, so work per direction is `O(N·L)` even when groups are large.
* While expanding, whenever a visited vertex of the opposite side is touched, record `dist_s[u] + 1 + dist_other[v]`. After the level is finished, the minimum candidate is the exact shortest distance (any shorter path would have been detected in an earlier level).
* Parent arrays from both sides are stitched to print the actual word path.

**Edge cases handled:** `A == B` → 0; `B ∉ D` → -1; `A ∉ D` → `A` is added as an extra start-only node (the spec leaves this to the input constraints); no path → -1.

## Input format
```
N L
w_1 w_2 ... w_N      (dictionary)
A B
```

## Complexity
| | |
|---|---|
| Build neighbour groups | **O(L · N log N)** (L sorts of N 64-bit keys) |
| Bidirectional BFS | **O(N · L)** worst case (each node/position group handled once per side) |
| Space | **O(N · L)** (group ranges + sorted orders) |

In practice bidirectional BFS explores about `b^(d/2)` nodes per side instead of `b^d` for a one-sided BFS (branching factor `b`, answer length `d`), so it often stops far earlier than the worst-case bound.

## Build & run
```
gcc -Wall -Wextra -O2 -o Q3_prog q3_word_ladder_bibfs.c
./Q3_prog < sample_input.txt        # hit -> cog, k = 4
./Q3_prog < sample_input2.txt       # cog missing from D -> -1
```

## Validation
* `../tests/run_tests.py` compares with a plain BFS using O(N²) Hamming checks on 600 random dictionaries (alphabets of 2–3 letters, `L = 1..5`, `A` inside and outside `D`, `A == B`, unreachable targets). It also checks that every printed path starts at `A`, ends at `B`, stays inside `D`, and changes one letter per step: **all pass**.
* Scale test at the stated limits: `N = 100,000`, `L = 10` (4-letter alphabet so the graph is well connected), answer `k = 16`, ran in about 0.2 s.
* Terminal screenshots: `q3_sample1.png`, `q3_sample2_no_path.png`.
