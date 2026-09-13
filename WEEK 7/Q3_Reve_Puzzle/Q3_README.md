# Q3 — Reve's Puzzle (Tower of Hanoi with 4 pegs)

## Files
- `reve_puzzle.c` — source code
- `reve_puzzle` — compiled executable (Linux binary, built with `gcc -O2`)
- `output.png` — terminal screenshot of it compiling and running

## Problem
8 disks of different sizes stacked on the first of **4** pegs (largest at
bottom). Move all disks to another peg, one at a time, never placing a
larger disk on a smaller one. Solve in 33 moves, and generalize to `n`
disks.

## Input representation
- `n` = number of disks (command-line argument).
- 4 pegs, labelled 1–4.
- Peg contents are stored as small stack arrays (`pegDisk[peg][]`) purely
  so every generated move can be **validated** (no larger-on-smaller
  violation ever slips through).

## Algorithm — Frame–Stewart
For the classic 3-peg Hanoi, `H3(m) = 2^m - 1`. For 4 pegs:
```
R4(0) = 0
R4(n) = min over 1<=k<n of [ 2*R4(k) + H3(n-k) ]
```
DP is used to compute `R4[1..n]` and the optimal split `k` at each `n`.
The move sequence is then generated recursively:
1. Move the top `k` disks from source to a spare peg, **using all 4
   pegs** (`R4(k)` moves).
2. Move the remaining `n-k` (larger) disks from source to destination
   using **ordinary 3-peg Hanoi** (the peg holding the small `k` disks is
   "busy") — `H3(n-k)` moves.
3. Move the `k` disks from the spare peg onto the destination, again
   using all 4 pegs (`R4(k)` moves).

## Usage
```bash
./reve_puzzle <n> [verbose]
# e.g.
./reve_puzzle 8
./reve_puzzle 8 1     # verbose: print every individual move
```

## Verification
- DP table for `n = 1..8`: `R4 = 1, 3, 5, 9, 13, 17, 25, 33`.
- For `n = 8`, the program **simulates and validates** every move of the
  generated sequence against the peg stacks, counts **exactly 33 moves**,
  and confirms the final configuration is correct (all disks on the
  destination peg, in order) — matching the puzzle's required answer.

## Complexity
- Building the DP tables `R4[]` and optimal-`k`: `O(n^2)` time, `O(n)`
  space.
- Generating and validating the actual move list: `O(R4(n))` moves,
  i.e. exponential in `n` (as expected — this is inherent to any Hanoi
  variant, since that many physical moves really are required).
