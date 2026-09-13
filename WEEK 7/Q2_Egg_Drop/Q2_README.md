# Q2 — Super Egg Testing Experiment (Generalized Egg Drop)

## Files
- `egg_drop.c` — source code
- `egg_drop` — compiled executable (Linux binary, built with `gcc -O2`)
- `output.png` — terminal screenshot of it compiling and running

## Problem
Given `E` identical eggs and an `F`-storey building, find the **minimum
number of trials** that guarantees (worst case) finding the highest floor
from which an egg can be dropped without breaking. Generalize the
classic "2 eggs, 100 floors" version to arbitrary `E` and `F`.

## Input representation
Two integers: `E` (number of eggs) and `F` (number of floors), taken as
command-line arguments.

## Algorithm
Dynamic programming, phrased in terms of "how many floors can be
resolved with `e` eggs and `t` trials":
```
best[t][e] = best[t-1][e-1] + best[t-1][e] + 1
```
Reasoning: drop an egg on some floor.
- If it breaks → we have `e-1` eggs and `t-1` trials left to search
  **below** that floor.
- If it survives → we still have `e` eggs and `t-1` trials to search
  **above** that floor.
- The floor just tested is also "resolved" (+1).

We increase `t` starting from 1 until `best[t][E] >= F`; that smallest
`t` is the answer.

## Usage
```bash
./egg_drop <E> <F>
# e.g.
./egg_drop 2 100
```

## Verification
- `E = 2, F = 100` → **14** trials — this is the well-known textbook
  answer for the classic 2-egg, 100-floor puzzle.
- `E = 3, F = 200` → **11** trials, generalizing correctly beyond the
  classic 2-egg case.

The full DP table (`best[t][e]` for every `t` up to the answer) is
printed so the growth pattern (`1,3,6,10,15,...` — triangular numbers —
for the 2-egg column) can be inspected directly.

## Complexity
- Time: `O(E * F)` — `t` only needs to go up to `O(F)` in the worst case
  (e.g. with 1 egg), and each `t` does `O(E)` work.
- Space: `O(E * F)` for the DP table as implemented (reducible to `O(E)`
  with a rolling array, since row `t` only depends on row `t-1`).
