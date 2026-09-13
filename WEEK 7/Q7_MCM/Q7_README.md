# Q7 — Matrix Chain Multiplication (MCM)

## Files
- `mcm.c` — source code
- `mcm` — compiled executable (Linux binary, built with `gcc -O2`)
- `output.png` — terminal screenshot of it compiling and running

## Problem
Given the dimensions of a chain of matrices `A1 x A2 x ... x An`, find
the **minimum number of scalar multiplications** needed to compute the
full product, and the corresponding optimal parenthesization (order of
multiplication).

## Input representation
An array `p[0..n]` of `n+1` integers, where matrix `Ai` has dimensions
`p[i-1] x p[i]` (so consecutive matrices are always compatible for
multiplication). Can be supplied on the command line as a list of
dimensions; defaults to the classic textbook instance
`30, 35, 15, 5, 10, 20, 25` if no arguments are given.

## Algorithm — Dynamic Programming
```
dp[i][i] = 0
dp[i][j] = min over i<=k<j of  dp[i][k] + dp[k+1][j] + p[i-1]*p[k]*p[j]
```
A `split[i][j]` table records the `k` that achieves the minimum at each
`(i,j)`, so the optimal parenthesization can be reconstructed
recursively afterward.

## Usage
```bash
./mcm                          # uses the default textbook dimensions
./mcm 30 35 15 5 10 20 25       # or supply your own dimension list
```

## Verification
On the classic instance (dimensions `30, 35, 15, 5, 10, 20, 25`, 6
matrices), the program reports the well-known textbook answer:
**minimum cost = 15125**, with optimal parenthesization
`((A1(A2A3))((A4A5)A6))`.

## Complexity
- Time: `O(n^3)` — `O(n^2)` subproblems `dp[i][j]`, each considering
  `O(n)` split points `k`.
- Space: `O(n^2)` for the `dp` and `split` tables.
