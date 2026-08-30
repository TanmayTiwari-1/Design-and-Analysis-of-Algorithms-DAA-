# Q3 — Quick Sort of N Random Elements Stored in a File

## Problem
Implement Quick Sort of N random elements that are stored in a file.

## Workflow
1. **Generate** N random integers and write them to `input.txt`.
2. **Read** the N numbers back from `input.txt` into an array.
3. **Sort** the array using Quick Sort.
4. **Write** the sorted array to `output.txt` and also print it to the console.

## Algorithm — Quick Sort (Lomuto Partition)
1. Pick a pivot (here, the last element of the current sub-array).
2. **Partition**: rearrange elements so everything `< pivot` comes before it and everything `>= pivot` comes after it. The pivot now sits in its final sorted position.
3. Recursively apply Quick Sort to the sub-array **left** of the pivot and the sub-array **right** of the pivot.
4. Recursion ends when a sub-array has 0 or 1 elements (already "sorted").

## Complexity Analysis
Let `T(n)` be the time for Quick Sort on `n` elements. Partitioning costs `O(n)`.

| Case | When it happens | Recurrence | Result |
|---|---|---|---|
| Best case | Pivot always splits array evenly | `T(n) = 2T(n/2) + O(n)` | **O(n log n)** |
| Average case | Random data (as generated here) | Expected | **O(n log n)** |
| Worst case | Already-sorted/reverse-sorted data with a poor pivot choice | `T(n) = T(n-1) + O(n)` | **O(n²)** |

- **Best/Average Time:** O(n log n) — the recursion tree has O(log n) levels, and each level does O(n) total work across all partitions.
- **Worst Time:** O(n²) — occurs when the pivot is always the smallest or largest element, causing maximally unbalanced partitions. Since this program uses **randomly generated data**, the worst case is very unlikely to be triggered.
- **Space Complexity:** O(1) extra (in-place swaps); recursion stack is O(log n) on average, O(n) in the worst case.
- **File I/O overhead:** Reading N numbers costs O(n); writing N numbers costs O(n) — this doesn't change the asymptotic sorting complexity.

## Files
- `3_quicksort_file.c`

## How to Compile & Run
```bash
gcc 3_quicksort_file.c -o quicksort_file -Wall
./quicksort_file
```

## Sample Run
```
Enter number of random elements N to generate: 20
Generated 20 random numbers into input.txt
Unsorted elements read from file:
3413 9740 9775 1868 7294 4021 9407 9738 8104 9812 4529 6353 7109 5217 224 991 9453 3313 3223 8443
Sorted elements (also written to output.txt):
224 991 1868 3223 3313 3413 4021 4529 5217 6353 7109 7294 8104 8443 9407 9453 9738 9740 9775 9812
```

## Output Files Produced
- `input.txt` — the unsorted, randomly generated numbers (one per line).
- `output.txt` — the same numbers, sorted in ascending order (one per line).
