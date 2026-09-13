# Design and Analysis of Algorithms — Lab 07
### BTech (CS-B and CE), 3rd Semester — September 8, 2026
### Instructor: Dr. Ajaya Kumar Dash

This archive contains C solutions to all seven puzzle-based algorithm
questions in the Lab-07 sheet. Every question folder contains:

- the **`.c`** source file (heavily commented: input representation,
  algorithm design, and complexity analysis are all in the header comment),
- the compiled **executable** (built with `gcc -O2`),
- an **`output.png`** — a terminal-style screenshot of the program actually
  compiling and running, showing that the results match the known/expected
  answers for each puzzle.

All programs were compiled with:
```
gcc -O2 -o <name> <name>.c
```
on Ubuntu 24.04 / gcc, and take an optional command-line argument for the
puzzle size (defaults are provided so `./program` alone always works).

---

## Q1 — Invert the Coin-Triangle
**Folder:** `Q1_Coin_Triangle/`  |  **Program:** `coin_triangle`  |  Usage: `./coin_triangle <n>`

**Algorithm.** Coins are placed on a triangular lattice using integer
coordinates `(X, R) = (2*col - row, row)`. Inverting the triangle 180°
corresponds to the map `(X,R) -> (A-X, B-R)`. The program searches over
all integer translations `(A,B)` and picks the one that maximises the
overlap between the original coin-set and the rotated+translated
coin-set — the un-overlapped coins are exactly the ones that must be
physically moved. This independently reproduces the well-known
**closed-form formula**:

```
M(n) = floor( T / 3 ),   where T = n(n+1)/2 is the total number of coins
```

Verified for n = 2..6 → 1, 2, 3, 5, 7 moves respectively (matches the
classic "10-coin triangle needs 3 moves" and "15-coin triangle needs 5
moves" puzzle results).

**Complexity.** Brute-force search + verification: `O(n^6)` (or `O(n^4)`
with a hash-set lookup instead of the linear scan used here for clarity).
The closed-form formula itself is `O(1)`.

---

## Q2 — Super Egg Testing Experiment
**Folder:** `Q2_Egg_Drop/`  |  **Program:** `egg_drop`  |  Usage: `./egg_drop <E> <F>`

**Algorithm.** Generalized dynamic programming over "how many floors can
be resolved with `e` eggs and `t` trials":
```
best[t][e] = best[t-1][e-1] + best[t-1][e] + 1
```
We increase `t` until `best[t][E] >= F`; that `t` is the answer.

Verified: **E=2, F=100 → 14 trials** (the classic textbook result).
Also generalized and tested for E=3, F=200 → 11 trials.

**Complexity.** Time `O(E*F)`, Space `O(E*F)` (reducible to `O(E)`).

---

## Q3 — Reve's Puzzle (Tower of Hanoi, 4 pegs)
**Folder:** `Q3_Reve_Puzzle/`  |  **Program:** `reve_puzzle`  |  Usage: `./reve_puzzle <n>`

**Algorithm.** The Frame–Stewart algorithm: for `n` disks and 4 pegs,
```
R4(n) = min over 1<=k<n of [ 2*R4(k) + (2^(n-k) - 1) ]
```
The optimal split `k` is found by DP, and used to recursively (a) move
the top `k` disks aside using all 4 pegs, (b) move the remaining
`n-k` disks with ordinary 3-peg Hanoi, (c) move the `k` disks back onto
the destination with all 4 pegs. Every move is validated in a simulated
peg-stack (never places a larger disk on a smaller one).

Verified: **n = 8 disks → exactly 33 moves**, as required by the puzzle,
and the DP table for R4(1..8) = 1, 3, 5, 9, 13, 17, 25, 33 is printed.

**Complexity.** DP table: `O(n^2)`. Generating/validating the actual move
sequence: `O(R4(n))` moves (exponential in `n`, as expected for Hanoi-type
puzzles).

---

## Q4 — Security Switches
**Folder:** `Q4_Security_Switches/`  |  **Program:** `security_switches`  |  Usage: `./security_switches <n>`

**Algorithm.** This puzzle is structurally identical to the classic
"Chinese Rings" (Baguenaudier) puzzle. States are `n`-bit masks (bit
`i-1` = switch `i` is ON); legal single-switch toggles define edges of a
graph on `2^n` states. A **BFS** from the all-ON state to the all-OFF
state finds the *provably minimum* number of toggles (BFS on an
unweighted graph = shortest path).

Verified for n = 1..6 → 1, 2, 5, 10, 21, 42 moves, matching the closed
form `M(n) = floor(2^(n+1)/3)` exactly, and the optimal move sequence is
printed.

**Complexity.** BFS over `2^n` states, `O(n)` edges per state:
Time `O(n * 2^n)`, Space `O(2^n)`. (The closed-form gives `O(1)`.)

---

## Q5 — Hitting a Moving Target
**Folder:** `Q5_Hitting_Moving_Target/`  |  **Program:** `hitting_target`  |  Usage: `./hitting_target <n>`

**Algorithm.** Such an algorithm **does exist** (for n ≥ 2), based on a
parity argument: shoot spots `2,3,...,n-1` then `n-1,n-2,...,2`
(`2(n-2)` shots total). The program *proves* correctness constructively
by tracking the set `S` of spots consistent with "the target has evaded
every shot so far" (removing the shot spot, then expanding to neighbours
to model the forced move); if `S` empties out, a hit is guaranteed.

Verified for n = 6 and n = 10: `S` is driven to empty exactly on the
final shot of the sequence, confirming the algorithm is correct and
tight.

**Complexity.** `O(n)` shots, `O(n)` work each → Time `O(n^2)`, Space `O(n)`.

---

## Q6 — The Best Time to Be Alive
**Folder:** `Q6_Best_Time_Alive/`  |  **Program:** `best_time_alive`

**Algorithm.** Classic max-overlapping-intervals sweep: turn each
scientist's lifetime into a `(birth,+1)` and `(death,-1)` event, sort by
year with **deaths processed before births in the same year** (per the
problem's tie-break rule), then sweep left-to-right tracking a running
"alive" counter and recording its maximum.

Demonstrated on a built-in sample of 15 prominent scientists (Copernicus
through Hawking); the program reports the peak year and which scientists
were alive then.

**Complexity.** Time `O(n log n)` (sorting dominates), Space `O(n)`.

---

## Q7 — Matrix Chain Multiplication (MCM)
**Folder:** `Q7_MCM/`  |  **Program:** `mcm`  |  Usage: `./mcm [dims...]`

**Algorithm.** Standard DP:
```
dp[i][j] = min over i<=k<j of dp[i][k] + dp[k+1][j] + p[i-1]*p[k]*p[j]
```
with a `split[i][j]` table used to reconstruct the optimal
parenthesization.

Verified against the classic textbook instance (dimensions
30,35,15,5,10,20,25) → **minimum cost 15125**, with optimal
parenthesization `((A1(A2A3))((A4A5)A6))`.

**Complexity.** Time `O(n^3)`, Space `O(n^2)`.

---

## How to rebuild everything from scratch
```bash
for d in Q1_Coin_Triangle Q2_Egg_Drop Q3_Reve_Puzzle Q4_Security_Switches \
         Q5_Hitting_Moving_Target Q6_Best_Time_Alive Q7_MCM; do
  ( cd "$d" && gcc -O2 -o "$(ls *.c | sed 's/\.c$//')" *.c )
done
```
