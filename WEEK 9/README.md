# DAA Lab-09 – Greedy Design Paradigm (C solutions)

BTech (CS-B and CE), 3rd semester · Instructor: Dr. Ajaya Kumar Dash · Lab sheet dated 6 Oct 2026

Each folder holds, for one question:

| File | Purpose |
|---|---|
| `qNN_*.c` | C program (reads stdin; `--test` runs a randomised self-check against brute force) |
| `sample_input.txt` | sample input that shows the chosen input representation |
| `output.txt` | captured terminal session (compile, sample run, self-test) |
| `terminal_output.png` | the same session rendered as a terminal screenshot |
| `README.md` | problem, input representation, algorithm, correctness, complexity derivation, how to run |

| # | Problem | Technique | Time |
|---|---|---|---|
| 1 | Fractional knapsack with deterioration | sort by decay rate + exact concave DP | O(n²) |
| 2 | Huffman coding (canonical) | min-heap merging | O(n log n) |
| 3 | Minimum refuelling stops | lazy max-heap | O(m log m) |
| 4 | Connect sticks | min-heap merging | O(n log n) |
| 5 | Candy distribution | two-pass / slope scan | O(n) |
| 6 | Reorganise string, K apart | greedy with cool-down | O(n·σ) |
| 7 | Minimise deviation | max-heap, halve the maximum | O(n log n log M) |
| 8 | Meeting rooms | sweep / min-heap | O(n log n) |
| 9 | Hu–Tucker | two-phase greedy | O(n³) simple simulation |
| 10 | Greedy superstring | greedy vs exact DP workbench | see folder |

## Build everything
```bash
cd Q01_Fractional_Knapsack_Deterioration
gcc -O2 -Wall -Wextra -o q01 q01_fractional_knapsack.c -lm
./q01 < sample_input.txt
./q01 --test
```
(the same pattern for every folder), or regenerate all outputs, screenshots and README sample blocks with:
```bash
python3 tools/build_outputs.py      # needs gcc, Python 3, Pillow
```

## Notes
* **Q1** uses a precise continuous-decay model (stated at the top of its README); check it against your lecture notes.
* **Q3** answers the question as written (minimum number of stops) and also reports the minimum initial fuel, since the title mentions it.
* **Q9** passes its checks against an exact DP, but the simple simulation is O(n³); the README explains the faster known variants.
* **Q10** is an exploration workbench. The arXiv:2609.01365 claim is a preprint and is not reproduced here; see the folder's README.
* All self-tests use a fixed random seed, so results are reproducible.
