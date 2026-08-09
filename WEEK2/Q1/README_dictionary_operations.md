# Dictionary Operations — Data Structure Comparison

## Overview

This project compares the performance of six implementations of a Dictionary:

- Unsorted Array
- Sorted Array
- Unsorted Singly Linked List
- Sorted Singly Linked List
- Unsorted Doubly Linked List
- Sorted Doubly Linked List

The program measures the execution time of **insert, search, and delete** operations for different values of `n`.

## Operations & Expected Complexity

| Data Structure | Insert | Search | Delete |
|---|---:|---:|---:|
| Unsorted Array | `O(1)` | `O(n)` | `O(1)` |
| Sorted Array | `O(n)` | `O(log n)` | `O(n)` |
| Unsorted Singly Linked List | `O(1)` | `O(n)` | `O(n)` |
| Sorted Singly Linked List | `O(n)` | `O(n)` | `O(n)` |
| Unsorted Doubly Linked List | `O(1)` | `O(n)` | `O(1)` |
| Sorted Doubly Linked List | `O(n)` | `O(n)` | `O(n)` |

The implementation uses insertion at the head for unsorted linked lists and maintains sorted order for sorted structures. The sorted array uses binary search for searching. fileciteturn3file0L18-L57 fileciteturn3file0L59-L136

## Experimental Setup

The program tests:

```text
n = 500, 1000, 2000, 4000, 8000, 16000, 32000, 64000
```

A fixed random seed is used:

```text
srand(42)
```

The timing is measured using `clock()` and reported in microseconds. fileciteturn3file0L139-L147

For more reliable measurements, each operation is repeated multiple times depending on `n`. Searches use an absent key to measure the worst-case linear/binary-search behavior. fileciteturn3file0L155-L157 fileciteturn3file0L199-L222

## Results

The program generates:

```text
dict_ops.csv
```

The CSV contains the measured time for all six data structures:

```text
n,
ua_insert, sa_insert, slu_insert, sls_insert, dlu_insert, dls_insert,
ua_search, sa_search, slu_search, sls_search, dlu_search, dls_search,
ua_delete, sa_delete, slu_delete, sls_delete, dlu_delete, dls_delete
```

The CSV is generated directly by the C program. fileciteturn3file0L149-L153

## Key Observations

- **Unsorted arrays and linked lists** provide very fast insertion because insertion is performed at the end/head.
- **Sorted arrays** provide the fastest search asymptotically because binary search takes `O(log n)`.
- Maintaining sorted order makes insertion more expensive.
- **Doubly linked lists** allow deletion in `O(1)` when the target node is already known because both `prev` and `next` pointers are available.
- The experimental results can be compared with the theoretical Big-O complexities above.

## Conclusion

There is no single best dictionary representation for every operation.

- If **fast insertion** is important, an unsorted array or linked list is useful.
- If **fast searching** is the priority, a sorted array is the best of these implementations.
- If **fast deletion of a known node** is required, a doubly linked list is advantageous.
- Maintaining sorted order generally increases insertion cost but provides useful ordering properties.

## Repository Structure

```text
dictionary-analysis/
│
├── README.md
├── dict_ops.c
├── data/
│   └── dict_ops.csv
└── plots/
    ├── insert.png
    ├── search.png
    └── delete.png
```

## Tools Used

- C
- Standard C Library
- CSV for experimental results
- Graphs for performance comparison
