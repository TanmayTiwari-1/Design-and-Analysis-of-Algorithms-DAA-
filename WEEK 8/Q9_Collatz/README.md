# Q9 - Collatz Conjecture (assignment)

## Idea
Repeat `n -> n/2` (even) or `n -> 3n+1` (odd) until n = 1. The program is split into small functions:

| function | job |
|---|---|
| `collatz_next` | one step, with **overflow check** (`n > (ULLONG_MAX-1)/3` before computing 3n+1) |
| `analyze` | steps to reach 1 and peak value, no storage |
| `trajectory` | full path stored in a **dynamic array** (`malloc`/`realloc`) |
| `run_single` | prints the path of one n, saves `trajectory.csv` |
| `run_interval` | for every n in [a,b]: longest path, highest peak, average steps; saves `collatz_interval.csv` |

## Input format (stdin)
    1 n        -> single start value        e.g. `1 27`      (111 steps, peak 9232)
    2 a b      -> interval [a,b]            e.g. `2 1 10000` (longest: 6171 with 261 steps; highest peak: 9663 -> 27114424)

## Graphs
    ./collatz < input_single.txt
    ./collatz < input_interval.txt
    python3 plot_collatz.py          # needs matplotlib
produces `graph_trajectory.png` and `graph_interval_steps.png` (already included).

## Complexity
No proof exists that the loop always terminates (open problem), so there is no proven worst case.
Empirically the number of steps for n grows roughly like O(log n) on average.
Interval mode: time = sum of the steps over all starts (~ (b-a+1) x average steps); space = O(b-a+1) for the result table.

## Run
    gcc -O2 -o collatz collatz.c
    ./collatz < input_single.txt
