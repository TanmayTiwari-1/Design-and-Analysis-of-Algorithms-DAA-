# DAA Lab-06 — Design and Analysis of Algorithms

**BTech (CS-B and CE), 3rd Semester | August 31, 2026**
**Instructor:** Dr. Ajaya Kumar Dash

This repository contains solutions to all four questions of Lab-06, each
with:
- a C program implementing the required algorithm(s),
- a `--csv` mode used to collect timing/measurement data,
- a `.csv` file of the collected data,
- a `.png` graph generated from that data, and
- a `.md` file explaining the approach, complexity analysis, and results.

## Structure

```
.
├── q1_array_operations/
│   ├── q1_array_ops.c
│   ├── q1_results.csv
│   ├── q1_graph.png
│   └── q1.md
├── q2_matrix_operations/
│   ├── q2_matrix_ops.c
│   ├── q2_results.csv
│   ├── q2_graph.png
│   └── q2.md
├── q3_convolution/
│   ├── q3_convolution_fft.c
│   ├── q3_results.csv
│   ├── q3_graph.png
│   └── q3.md
├── q4_sorting_reversal/
│   ├── q4_sorting_reversal.c
│   ├── q4_results.csv
│   ├── q4_graph.png
│   └── q4.md
├── plot_q1.py
├── plot_q2.py
├── plot_q3.py
├── plot_q4.py
└── README.md
```

## Questions Summary

| # | Topic | Key result |
|---|---|---|
| Q1 | 1D array operations | Most ops are O(n); median & mode are O(n log n) via sorting |
| Q2 | n×n matrix operations | Add/zero-test/symmetric-test/transpose are O(n²); multiply/determinant are O(n³); eigenvalues need iterative methods (no closed form for n≥5) |
| Q3 | Convolution | O(n log n) via divide-and-conquer FFT (Cooley–Tukey), verified against naive O(n·m) |
| Q4 | Sorting by reversal | O(n) reversals always suffice (pancake-sort style); O(n log²n) total *cost* achievable via reversal-based merge sort |

## How to Build & Run Everything

Each question folder is self-contained. From inside any folder:

```bash
gcc -O2 -o q<N> q<N>_*.c -lm
./q<N>                       # human-readable demo / validation
./q<N> --csv > q<N>_results.csv   # regenerate the CSV
```

To regenerate all graphs from the CSVs (run from the repo root, requires
Python 3 + matplotlib):

```bash
pip install matplotlib
python3 plot_q1.py
python3 plot_q2.py
python3 plot_q3.py
python3 plot_q4.py
```

## Notes
- All programs were compiled with `gcc -O2` and tested on Linux.
- CSV timing data will vary slightly by machine, but the *scaling trend*
  (the shape of the growth curve) is what validates the complexity
  analysis, not the absolute numbers.
