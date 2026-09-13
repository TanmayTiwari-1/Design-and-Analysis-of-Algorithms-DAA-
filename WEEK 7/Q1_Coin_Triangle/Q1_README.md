# Q1 — Invert the Coin-Triangle

## Files
- `coin_triangle.c` — source code
- `coin_triangle` — compiled executable (Linux binary, built with `gcc -O2`)
- `output.png` — terminal screenshot of it compiling and running

## Problem
An equilateral triangle of `n` rows is formed by closely packed coins
(row `r` has `r+1` coins, `r = 0..n-1`), giving `T = n(n+1)/2` coins in
total. On each move you may slide **one** coin to a new position. Find
the **minimum number of coins that must move** to flip the whole triangle
upside down, and give a compact formula.

## Input representation
Each coin is given **integer** lattice coordinates:
```
X = 2*col - row        R = row
```
Doubling the horizontal axis turns the usual triangular-lattice spacing
into plain integers, so no floating point is needed anywhere.

## Algorithm
Flipping the triangle 180° corresponds to the coordinate map
`(X, R) -> (A - X, B - R)` for some integer shift `(A, B)`. The program
brute-force searches over candidate shifts `(A, B)` and keeps the one
that **maximises the overlap** between the original coin positions and
the rotated + shifted coin positions. The coins that don't overlap are
exactly the ones that must be physically slid — from their old, unmatched
spot to one of the target's unmatched spots.

This search independently reproduces the well-known **closed-form
formula**:
```
M(n) = floor(T / 3) = floor( n(n+1) / 6 )
```

## Usage
```bash
./coin_triangle <n>
# e.g.
./coin_triangle 4
```

## Verification
Ran for `n = 2..6`; algorithm's answer matched `floor(T/3)` every time:

| n (rows) | coins T | moves (algorithm) | floor(T/3) |
|---|---|---|---|
| 2 | 3  | 1 | 1 |
| 3 | 6  | 2 | 2 |
| 4 | 10 | 3 | 3 |
| 5 | 15 | 5 | 5 |
| 6 | 21 | 7 | 7 |

This matches the classic puzzle result that a 10-coin (4-row) triangle
needs exactly **3** moves, and a 15-coin (5-row) triangle needs **5**.

## Complexity
- Brute-force search + verification: `O(n^6)` with the simple linear
  "contains" scan used in the code (`O(n^4)` if a hash-set were used for
  O(1) lookups instead).
- The closed-form formula answers in `O(1)`.
