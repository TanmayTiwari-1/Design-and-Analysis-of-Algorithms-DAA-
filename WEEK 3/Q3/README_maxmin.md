# Divide-and-Conquer Maximum & Minimum Finder (C)

A C program that finds both the **maximum** and **minimum** elements of an array simultaneously using a **divide-and-conquer** algorithm, minimizing the total number of comparisons — close to the theoretical lower bound of `3n/2 - 2` comparisons.

## Overview

The naive approach to finding both the max and min of an array scans it twice (or once while comparing each element against both the running max and min), costing roughly `2n` comparisons. This program instead uses divide-and-conquer:

- Split the array into two halves.
- Recursively find the `(max, min)` pair of each half.
- Combine the two pairs using just **2 comparisons** (one to find the overall max, one to find the overall min).

This brings the total comparison count down to approximately `3n/2`, which is provably optimal for this problem.

## Files

| File | Description |
|------|--------------|
| `maxmin.c` | Main source file containing the full implementation |

## How It Works

1. **`Pair` struct** — Holds a `mx` (max) and `mn` (min) value, returned from each recursive call.
2. **`maxMin(arr, lo, hi)`** — The core recursive routine:
   - **Base case, 1 element** (`n == 1`): `mx = mn = arr[lo]`, no comparisons needed.
   - **Base case, 2 elements** (`n == 2`): a single comparison determines which of the two is the max and which is the min.
   - **Recursive case** (`n > 2`):
     - Splits the range at the midpoint into `[lo, mid]` and `[mid+1, hi]`.
     - Recursively computes `left = maxMin(lo, mid)` and `right = maxMin(mid+1, hi)`.
     - Uses **1 comparison** to find the overall max (`left.mx` vs `right.mx`).
     - Uses **1 comparison** to find the overall min (`left.mn` vs `right.mn`).
3. **`comparisons`** — A global counter incremented on every comparison performed, used to empirically verify the algorithm's efficiency against the theoretical bound.
4. **`main()`** — Reads `n` elements into an array, runs `maxMin()` over the whole array, and prints the result along with the comparison count versus the `3n/2` bound.

## Building

```bash
gcc -O2 -o maxmin maxmin.c
```

## Running

```bash
./maxmin
```

Example session:

```
Enter number of elements: 6
Enter 6 elements: 4 9 1 7 3 8
Maximum = 9, Minimum = 1
Comparisons used = 7 (bound 3n/2 = 9.0)
```

## Comparison Count Analysis

The recurrence for the number of comparisons `T(n)` is:

```
T(1) = 0
T(2) = 1
T(n) = 2*T(n/2) + 2      for n > 2
```

Solving this recurrence gives approximately:

```
T(n) ≈ 3n/2 - 2
```

This is the well-known **optimal comparison count** for the simultaneous max-min problem — no comparison-based algorithm can do better than `⌈3n/2⌉ - 2` in the worst case. The program's printed "bound" (`1.5 * n`) is a simple reference value; the true tight bound is `3n/2 - 2`, so the actual comparison count printed will typically be slightly below the displayed `1.5 * n` figure.

## Notes & Limitations

- **No input validation**: `n <= 0` is not checked; a non-positive `n` will cause undefined behavior (e.g., `malloc(0)` or reading into an empty/invalid buffer).
- **Recursion depth**: For very large `n`, the recursive calls consume stack space proportional to `O(log n)`, which is generally not a concern in practice.
- **Global comparison counter**: `comparisons` is a global variable, so the program is not thread-safe / not reentrant if `maxMin()` were ever called concurrently or recursively from multiple independent contexts.
- **Integer arrays only**: The array holds `int` values; no overflow handling is needed for the max/min comparisons themselves, but very large inputs are limited by available memory (`malloc`).

## Complexity

| Operation | Complexity |
|-----------|------------|
| Time      | O(n) |
| Comparisons | ≈ 3n/2 - 2 (optimal) |
| Space     | O(log n) (recursion stack) |

## License

Free to use and modify for educational or personal purposes.
