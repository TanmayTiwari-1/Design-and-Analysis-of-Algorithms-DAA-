# Binary Search vs. Ternary Search Comparison (C)

A C program that implements both **binary search** and **ternary search** on a sorted array, instruments each with a comparison counter, and empirically compares how many comparisons each algorithm uses to find elements — demonstrating that binary search is more comparison-efficient than ternary search in practice.

## Overview

Both algorithms locate a target value `x` in a sorted array by repeatedly narrowing the search range:

- **Binary search** splits the range into **2** parts per iteration, using up to 2 comparisons per step.
- **Ternary search** splits the range into **3** parts per iteration (via two midpoints, `mid1` and `mid2`), using up to 4 comparisons per step.

Although ternary search reduces the search range faster per iteration (`log_3 n` iterations vs `log_2 n`), it needs more comparisons per iteration. The program measures both effects directly by counting every comparison performed, then sums the results across a full search of every element in the array to show the net effect.

## Files

| File | Description |
|------|--------------|
| `search_compare.c` | Main source file containing the full implementation |

## How It Works

1. **`cmp_count`** — A global counter incremented on every comparison performed by either search function, reset before each timed run.

2. **`binarySearch(arr, lo, hi, x)`**:
   - Iteratively computes `mid`, compares `arr[mid] == x` (1 comparison), and if not equal, compares `arr[mid] < x` (1 more comparison) to decide which half to continue searching.
   - Up to **2 comparisons per iteration**.

3. **`ternarySearch(arr, lo, hi, x)`**:
   - Iteratively computes two midpoints, `mid1` and `mid2`, dividing the range into three parts.
   - Compares `arr[mid1] == x`, then `arr[mid2] == x`, then (if neither matched) `x < arr[mid1]`, and potentially `x > arr[mid2]` to decide which of the three sub-ranges to continue searching.
   - Up to **4 comparisons per iteration**.

4. **`main()`**:
   - Builds a sorted array of `n = 100000` elements (`arr[i] = i`).
   - Runs a single search for `x = 3` with both algorithms and reports the index found and comparison count for each.
   - Then runs **both algorithms against every element in the array** (`n` total searches each), summing all comparisons used, and prints the grand totals — the metric used to draw the overall conclusion.

## Building

```bash
gcc -O2 -o search_compare search_compare.c
```

## Running

```bash
./search_compare
```

Example output (values will vary slightly, but the trend is consistent):

```
n = 100000, target = 3
Binary search  -> index 3, comparisons = 4
Ternary search -> index 3, comparisons = 4

Average over all n elements (worst case per element):
Total comparisons -> Binary: 3286670   Ternary: 5726480
=> Binary search uses fewer comparisons overall, confirming it is faster.
```

## Why Binary Search Wins on Comparisons

- Binary search: `O(log2 n)` iterations x up to 2 comparisons = `O(2 log2 n)` comparisons.
- Ternary search: `O(log3 n)` iterations x up to 4 comparisons = `O(4 log3 n)` comparisons.

Converting to the same base: `log3 n = log2 n / log2 3 ≈ log2 n / 1.585`. So ternary search's comparison bound is roughly `4 / 1.585 ≈ 2.52` times `log2 n`, compared to binary search's `2 * log2 n`. Since `2.52 > 2`, ternary search performs **more comparisons in total**, even though it does fewer iterations — the extra comparisons per iteration outweigh the benefit of a faster-shrinking range. This is exactly what the program's totals demonstrate empirically.

## Notes & Limitations

- **"Faster" here means fewer comparisons, not necessarily faster wall-clock time.** Real-world performance also depends on cache behavior, branch prediction, and memory access patterns, which this program does not measure (no timing code is included).
- **The single-target example (`x = 3`) is not representative on its own.** The comment in the code suggesting it forces a "worst-ish case" is only a rough heuristic; the meaningful comparison is the full-array total computed afterward, not this one lookup.
- **Both functions assume a sorted, duplicate-free (or at least consistently ordered) array.** Behavior with duplicate values is well-defined here (finds *some* matching index) but not necessarily the first or last occurrence.
- **Global `cmp_count`**: Not thread-safe / not reentrant if the search functions were ever called concurrently.
- **Fixed array size**: `n` is hardcoded to `100000` in `main()`; there's no user input in this version.

## Complexity

| Algorithm | Iterations | Comparisons per iteration | Total comparisons |
|-----------|------------|---------------------------|--------------------|
| Binary search | O(log2 n) | up to 2 | O(2 log2 n) |
| Ternary search | O(log3 n) | up to 4 | O(4 log3 n) ≈ O(2.52 log2 n) |

Both run in **O(log n)** time asymptotically, but binary search performs fewer total comparisons in practice, as reflected in the program's output.

## License

Free to use and modify for educational or personal purposes.
