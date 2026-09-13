# Q4 — Security Switches

## Files
- `security_switches.c` — source code
- `security_switches` — compiled executable (Linux binary, built with `gcc -O2`)
- `output.png` — terminal screenshot of it compiling and running

## Problem
`n` switches in a row, all initially **ON**. Rules:
1. The rightmost switch (switch 1) can be toggled freely.
2. Any other switch `i` can be toggled only if switch `i-1` (immediately
   to its right) is ON **and** all switches to the right of that
   (switches `1..i-2`) are OFF.
3. Only one switch may be toggled per move.

Turn all switches OFF in the minimum number of moves.

## Input representation
The state of all `n` switches is packed into a single **n-bit integer**
`mask` (bit `i-1` = 1 means switch `i` is ON). All `2^n` reachable states
fit comfortably in one machine word for lab-sized `n`.

## Algorithm — BFS over the state graph
This puzzle is structurally identical to the classic **Chinese Rings /
Baguenaudier** puzzle. Build a graph where nodes are the `2^n`
configurations and edges are legal single-switch toggles (as per the
rules above). A **Breadth-First Search** from the all-ON state to the
all-OFF state finds the shortest path — since BFS on an unweighted graph
always returns the shortest path, this is a **provably optimal** answer,
not just a formula lookup.

## Usage
```bash
./security_switches <n>
# e.g.
./security_switches 5
```

## Verification
BFS results for `n = 1..6`: **1, 2, 5, 10, 21, 42** moves — these match
the closed-form check printed alongside them, `M(n) = floor(2^(n+1)/3)`,
exactly. The optimal move sequence (list of switch numbers toggled, in
order) is also printed and can be hand-verified against the rules.

## Complexity
- States: `2^n`. Edges checked per state: `O(n)`.
- Time: `O(n * 2^n)`. Space: `O(2^n)` (for `dist[]`/`parent[]` arrays).
- This is exact and exponential, which is appropriate for exhaustively
  searching a combinatorial state space; for very large `n` one would
  instead use the closed-form formula, which is `O(1)`.
