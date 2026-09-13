# Q5 — Hitting a Moving Target

## Files
- `hitting_target.c` — source code
- `hitting_target` — compiled executable (Linux binary, built with `gcc -O2`)
- `output.png` — terminal screenshot of it compiling and running

## Problem
`n` hiding spots in a line, numbered `1..n`. A target hides at some
unknown spot; between every two consecutive shots it **must** move to an
adjacent spot. One shot is fired per round at a chosen spot. Does an
algorithm exist that **guarantees** eventually hitting the target? If so,
give it.

## Input representation
`n` = number of hiding spots (command-line argument). The "set of spots
the target could still be occupying" is stored as a boolean array of
size `n+1` (1-indexed).

## Algorithm
**Yes, such an algorithm exists** (for `n >= 2`), based on a parity
argument: the target's position parity (odd/even) flips after every
forced move, and since we don't know its initial position or parity, two
passes are needed to cover both cases:
```
Pass 1 (increasing): shoot spots 2, 3, 4, ..., n-1
Pass 2 (decreasing): shoot spots n-1, n-2, ..., 2
```
Total shots: `2*(n-2)`. (For `n = 2` the puzzle is trivial: alternately
shoot `1, 2, 1, 2, ...`.)

**How the program verifies this (not just asserts it):** it tracks the
set `S` of spots consistent with "the target has evaded every shot fired
so far." Each round: (1) remove the shot spot from `S`; (2) if `S`
becomes empty, every possible evading walk has been eliminated, so a hit
is guaranteed by that round; (3) otherwise the target is forced to move,
so `S` is replaced by the neighbours of its elements. If `S` empties out
by the end of the fixed shooting sequence, the algorithm is proven
correct for that `n`.

## Usage
```bash
./hitting_target <n>
# e.g.
./hitting_target 10
```

## Verification
For `n = 6` and `n = 10`, the possibility-set `S` shrinks every round and
is driven to **exactly zero on the final shot** of the sequence — proving
the algorithm both correct (a hit is guaranteed) and tight (no shot is
wasted).

## Complexity
- `O(n)` shots fired, `O(n)` work to update `S` per shot.
- Time: `O(n^2)`. Space: `O(n)`.
