# Q1 — Find the Median of N Numbers Without Sorting

## Problem
Given a list of N numbers, find the median **without sorting the entire list**, and analyze the time complexity of your approach.

## Idea
The median is nothing but an **order statistic**:
- If N is odd, median = the element at rank `N/2` (0-indexed) in sorted order.
- If N is even, median = average of elements at ranks `N/2 - 1` and `N/2`.

Instead of sorting the whole array (which costs O(n log n)) just to read off one or two positions, we use **Quickselect** — a cousin of Quicksort that only explores the part of the array that can contain the answer.

## Algorithm — Randomized Quickselect
1. Pick a **random pivot** from the current sub-array (avoids worst-case behavior on sorted/adversarial input).
2. **Partition** the sub-array (Lomuto scheme) around the pivot: smaller elements go left, larger go right. The pivot ends up at its final sorted position `p`.
3. Compare the target rank `k` to `p`:
   - `k == p` → pivot **is** the answer, return it.
   - `k < p` → recurse **only** into the left part.
   - `k > p` → recurse **only** into the right part.
4. Unlike Quicksort, we never recurse into both halves — so we never fully sort the array.

For even N, Quickselect is called twice (once for each middle rank) and the two results are averaged.

## Complexity Analysis
Let `T(n)` be the time to quickselect over `n` elements. Partitioning always costs `O(n)`.

| Case | Recurrence | Result |
|---|---|---|
| Average case | `T(n) = T(n/2) + O(n)` | **O(n)** |
| Worst case | `T(n) = T(n-1) + O(n)` | **O(n²)** |
| Worst case (with Median-of-Medians pivot selection) | — | O(n) guaranteed |

- **Average Time:** O(n) — each recursive call only explores *one* half, so the work forms a decreasing geometric series `n + n/2 + n/4 + … = O(n)`.
- **Worst Time:** O(n²) — happens only if the pivot repeatedly turns out to be the smallest/largest element (e.g., sorted input with naive pivot choice). Random pivot selection makes this astronomically unlikely in practice.
- **Space Complexity:** O(1) extra (in-place partitioning); recursion stack is O(log n) on average, O(n) worst case.

Compare this to "sort then pick the middle," which always costs **O(n log n)** regardless of data — Quickselect is asymptotically better on average.

## Files
- `1_median_without_sorting.c`

## How to Compile & Run
```bash
gcc 1_median_without_sorting.c -o median -Wall
./median
```

## Sample Run
```
Enter number of elements N: 5
Enter 5 elements:
9 3 7 1 5
Median = 5.00
```

```
Enter number of elements N: 6
Enter 6 elements:
9 3 7 1 5 2
Median = 4.00
```
