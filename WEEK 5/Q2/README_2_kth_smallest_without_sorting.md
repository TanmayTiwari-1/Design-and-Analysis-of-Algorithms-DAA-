# Q2 — Find the K'th Smallest Element Without Sorting

## Problem
Given a list of N numbers and an integer K, find the K'th smallest element **without sorting the entire list**, and analyze the time complexity.

## Idea
This is a direct generalization of Q1: instead of always looking for the "middle" rank, we look for an arbitrary rank `k` (converted to 0-based index `k-1`). The same **Quickselect** technique applies — we only need to explore the part of the array containing the answer.

## Algorithm — Randomized Quickselect
1. Pick a **random pivot** and swap it to the end of the current sub-array.
2. **Partition** the sub-array (Lomuto scheme) so elements smaller than the pivot are on the left, larger on the right. The pivot lands at its correct sorted index `p`.
3. Compare `p` to the target index `k-1`:
   - `p == k-1` → pivot **is** the K'th smallest element, return it.
   - `k-1 < p` → recurse **only** into the left sub-array.
   - `k-1 > p` → recurse **only** into the right sub-array.
4. Because only one side is ever explored, the array is never fully sorted — we do just enough work to pin down one value.

## Complexity Analysis
Let `T(n)` be the time to quickselect over `n` elements. Each partition step costs `O(n)`.

| Case | Recurrence | Result |
|---|---|---|
| Average case | `T(n) = T(n/2) + O(n)` | **O(n)** |
| Worst case | `T(n) = T(n-1) + O(n)` | **O(n²)** |
| Worst case (Median-of-Medians pivot) | — | O(n) guaranteed |

- **Average Time:** O(n) — the pivot roughly halves the search space each recursive call, giving a geometric-series total of O(n).
- **Worst Time:** O(n²) — only when pivot selection is consistently unlucky (e.g., always picking the current min/max). Randomization makes this vanishingly rare.
- **Space Complexity:** O(1) extra (in-place); recursion stack O(log n) average, O(n) worst case.

Sorting first and then indexing would always cost **O(n log n)**, regardless of how small or large K is — Quickselect avoids that unnecessary extra work on average.

## Files
- `2_kth_smallest_without_sorting.c`

## How to Compile & Run
```bash
gcc 2_kth_smallest_without_sorting.c -o kth_smallest -Wall
./kth_smallest
```

## Sample Run
```
Enter number of elements N: 6
Enter 6 elements:
9 3 7 1 5 2
Enter K (1-based rank, e.g. K=1 -> smallest element): 3
The 3-th smallest element is: 3
```

```
Enter number of elements N: 6
Enter 6 elements:
9 3 7 1 5 2
Enter K (1-based rank, e.g. K=1 -> smallest element): 1
The 1-th smallest element is: 1
```
(Sorted array is {1, 2, 3, 5, 7, 9} — 1st smallest = 1, 3rd smallest = 3, confirming correctness.)
