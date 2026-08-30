# Q4 — Heap Sort of N Randomly Generated Elements Stored in a File

## Problem
Implement Heap Sort to sort N randomly generated elements stored in a file, and analyze the time complexity.

## Workflow
1. **Generate** N random integers and write them to `input.txt`.
2. **Read** the N numbers back from `input.txt` into an array.
3. **Sort** the array using Heap Sort.
4. **Write** the sorted array to `output.txt` and also print it to the console.

## Algorithm — Heap Sort
1. **Build a max-heap** from the input array: starting from the last non-leaf node (`index n/2 - 1`) and moving up to the root, call `heapify()` on each node so the max-heap property (`parent ≥ children`) holds everywhere.
2. **Repeatedly extract the maximum**:
   - Swap the root (largest remaining element) with the last element of the current heap.
   - Shrink the heap size by 1 (the last position is now "sorted" and frozen).
   - Call `heapify()` on the new root to restore the max-heap property.
3. Repeating step 2 exactly `n-1` times places every element into its correct final position, from largest to smallest, producing a fully sorted array.

## Complexity Analysis
Let `n` = number of elements.

- **Building the initial heap:** A naive per-node analysis suggests O(n log n), but a tighter amortized analysis (accounting for the fact that most nodes are near the bottom of the tree and need very few swaps) shows building a heap bottom-up actually costs **O(n)**.
- **Extract-max + heapify, repeated n times:** Each `heapify` call on a heap of current size `k` costs O(log k). Summing over all extractions:
  `Σ (k=1 to n) O(log k) = O(n log n)`
- **Total Time Complexity:**
  `T(n) = O(n) [build heap] + O(n log n) [n extractions] = O(n log n)`

| Case | Time |
|---|---|
| Best case | **O(n log n)** |
| Average case | **O(n log n)** |
| Worst case | **O(n log n)** |

Unlike Quick Sort, Heap Sort has **no bad-input scenario** — its performance is O(n log n) for *every* input, because the heap's shape depends only on `n`, not on the values or their initial order.

- **Space Complexity:** O(1) extra — the heap is built directly inside the input array (in-place), no auxiliary array needed (unlike Merge Sort).
- **Stability:** Heap Sort is **not stable** — equal elements can be reordered relative to each other.
- **File I/O overhead:** Reading N numbers costs O(n); writing N numbers costs O(n) — this doesn't change the asymptotic sorting complexity.

## Files
- `4_heapsort_file.c`

## How to Compile & Run
```bash
gcc 4_heapsort_file.c -o heapsort_file -Wall
./heapsort_file
```

## Sample Run
```
Enter number of random elements N to generate: 20
Generated 20 random numbers into input.txt
Unsorted elements read from file:
4291 4499 6158 5320 5790 775 5973 5714 8240 5534 7997 2652 8784 1569 1468 8008 8805 695 2714 9341
Sorted elements (also written to output.txt):
695 775 1468 1569 2652 2714 4291 4499 5320 5534 5714 5790 5973 6158 7997 8008 8240 8784 8805 9341
```

## Output Files Produced
- `input.txt` — the unsorted, randomly generated numbers (one per line).
- `output.txt` — the same numbers, sorted in ascending order (one per line).
