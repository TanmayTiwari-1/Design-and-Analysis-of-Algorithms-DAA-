# Dictionary Operations — Data Structure Comparison

## Overview

This project compares six implementations of a Dictionary:

- Unsorted Array
- Sorted Array
- Unsorted Singly Linked List
- Sorted Singly Linked List
- Unsorted Doubly Linked List
- Sorted Doubly Linked List

The following dictionary operations are considered:

- Insert
- Search
- Delete
- Minimum
- Maximum
- Predecessor
- Successor

The program measures operation performance for different values of `n` and stores the results in CSV files.

---

## Data Structures

| Data Structure | Insert | Search | Delete | Min/Max | Predecessor/Successor |
|---|---|---|---|---|---|
| Unsorted Array | O(1) | O(n) | O(1)* | O(n) | O(n) |
| Sorted Array | O(n) | O(log n) | O(n) | O(1) | O(1) |
| Unsorted Singly Linked List | O(1) | O(n) | O(n) | O(n) | O(n) |
| Sorted Singly Linked List | O(n) | O(n) | O(n) | O(1) | O(n) |
| Unsorted Doubly Linked List | O(1) | O(n) | O(n) | O(n) | O(n) |
| Sorted Doubly Linked List | O(n) | O(n) | O(n) | O(1) | O(n) |

`*` Delete can be O(1) after the target position/node is already known; finding the target is O(n).

---

## Min and Max

**Minimum** returns the smallest key in the dictionary.

**Maximum** returns the largest key.

For unsorted structures, all elements generally need to be examined:

```text
O(n)
```

For sorted structures, the minimum and maximum are directly available at the appropriate ends:

```text
O(1)
```

---

## Predecessor and Successor

For a key `x`:

- **Predecessor(x):** the largest key smaller than `x`.
- **Successor(x):** the smallest key larger than `x`.

Example:

```text
Keys: 10 20 30 40 50

For x = 30:

Predecessor = 20
Successor   = 40
```

Sorted structures can take advantage of their ordering, while unsorted structures generally need to scan the elements.

---

## Experimental Setup

The program tests different dictionary sizes:

```text
n = 500, 1000, 2000, 4000, 8000, 16000, 32000, 64000
```

A fixed random seed is used to generate reproducible data.

The experiment records execution time for the dictionary operations and compares the practical results with the theoretical complexities.

---

## Results

The experimental results are stored in CSV format.

The CSV contains measurements for the different data structures and operations, including:

```text
Insert
Search
Delete
Minimum
Maximum
Predecessor
Successor
```

The CSV files can be uploaded directly to GitHub and will be displayed as tables.

---

## Key Observations

- **Unsorted structures** generally provide fast insertion but require linear scans for searching and ordered queries.
- **Sorted arrays** provide particularly efficient searching using binary search.
- **Sorted structures** make Minimum and Maximum operations very efficient.
- **Predecessor and Successor** benefit from maintaining sorted order.
- Maintaining sorted order usually increases insertion and deletion costs.
- Doubly linked lists provide convenient access to both previous and next nodes.

---

## Conclusion

No single data structure is optimal for every dictionary operation.

If fast insertion is the priority, an unsorted representation is useful.

If fast searching is important, a sorted array provides the best asymptotic search time among the structures tested.

If operations such as **Minimum, Maximum, Predecessor, and Successor** are frequently required, maintaining sorted order can provide significant advantages.

The experimental results demonstrate how the choice of dictionary representation affects both theoretical complexity and practical execution time.

---

## Repository Structure

```text
dictionary-analysis/
│
├── README.md
├── dict_ops.c
│
├── data/
│   └── dict_ops.csv
│
└── plots/
    ├── insert.png
    ├── search.png
    ├── delete.png
    ├── minimum.png
    ├── maximum.png
    ├── predecessor.png
    └── successor.png
```

## Tools Used

- C
- Standard C Library
- CSV for experimental results
- Graphs for performance comparison
