# Design and Analysis of Algorithms (DAA) Lab-01 & Lab-02

---

## Repository Overview

This repository contains the C implementations and experimental analysis for **Lab-01 and Lab-02** of the **Design and Analysis of Algorithms (DAA)** course.

The objective of these laboratories is to understand the implementation, analysis, and comparison of different algorithms using practical experiments. Each experiment includes source code, generated datasets, observations, complexity analysis, and README documentation.

---

## Course Information

| Field            | Details                                 |
| ---------------- | --------------------------------------- |
| **Department**   | Computer Engineering (CE)               |
| **Course**       | Design and Analysis of Algorithms (DAA) |
| **Semester**     | 3rd Semester                            |
| **Student Name** | Tanmay Tiwari                           |
| **Student ID**   | B325044                                 |
| **Institute**    | IIIT Bhubaneswar                        |

---

# Lab-01

## Experiments Included

## Experiment 1 – Order of Growth of Functions

### Objective

Compare various mathematical functions and arrange them in increasing order of asymptotic growth.

### Concepts Covered

* Asymptotic Analysis
* Big-O Notation
* Growth of Functions
* Logarithmic Plotting

### Output

* `growth.csv`

---

## Experiment 2 – Fair vs Biased Coin Simulation

### Objective

Simulate tossing a fair coin and a biased coin to verify experimental probabilities.

### Concepts Covered

* Probability Simulation
* Random Number Generation
* Law of Large Numbers

### Output

* `results.csv`

---

## Experiment 3 – Bubble Sort Performance Analysis

### Objective

Compare Optimized Bubble Sort and Unoptimized Bubble Sort.

### Concepts Covered

* Bubble Sort
* Best Case
* Worst Case
* Comparison Counting
* Time Complexity

### Output Files

* `bubble_sort_data.csv`
* `bubble_sort_sorted_data.csv`

---

## Experiment 4 – Tower of Hanoi

### Objective

Determine the minimum number of moves required to solve the Tower of Hanoi problem using recursion.

### Concepts Covered

* Recursion
* Recurrence Relation
* Exponential Time Complexity

### Output

* `moves.csv`

---

## Experiment 5 – Partition Point Detection

### Objective

Find the first occurrence of `1` in a binary array containing consecutive `0`s followed by consecutive `1`s.

### Concepts Covered

* Linear Search
* Binary Search
* Comparison Analysis

---

## Experiment 6 – Duplicate Element Detection

### Objective

Determine whether a randomly generated array contains duplicate elements using the Brute Force approach.

### Concepts Covered

* Brute Force Algorithm
* Comparison Counting
* Worst Case Analysis

### Output

* `comparisons.csv`

---

# Lab-02

## Experiments Included

## Experiment 1 – Dictionary Operations

### Objective

Analyze the asymptotic worst-case running time of the primary Dictionary ADT operations using different data structures.

### Dictionary Operations

* **Search(D, k)** – Search for an item using its key.
* **Insert(D, x)** – Insert a new data item.
* **Delete(D, x)** – Delete an item from the dictionary.
* **Maximum(D)** – Find the item with the largest key.
* **Minimum(D)** – Find the item with the smallest key.
* **Predecessor(D, x)** – Find the item immediately before `x` in sorted order.
* **Successor(D, x)** – Find the item immediately after `x` in sorted order.

### Data Structures Compared

* Unsorted Array
* Sorted Array
* Singly Linked Unsorted List
* Singly Linked Sorted List
* Doubly Linked Unsorted List
* Doubly Linked Sorted List

### Concepts Covered

* Dictionary ADT
* Arrays
* Sorted and Unsorted Data Structures
* Singly Linked Lists
* Doubly Linked Lists
* Searching
* Insertion and Deletion
* Predecessor and Successor
* Worst-Case Time Complexity
* Order of Growth
* Experimental Performance Analysis

### Complexity Summary

| Data Structure              | Search   | Insert | Delete | Maximum | Minimum | Predecessor | Successor |
| --------------------------- | -------- | ------ | ------ | ------- | ------- | ----------- | --------- |
| Unsorted Array              | O(n)     | O(1)   | O(n)   | O(n)    | O(n)    | O(n)        | O(n)      |
| Sorted Array                | O(log n) | O(n)   | O(n)   | O(1)    | O(1)    | O(1)        | O(1)      |
| Singly Linked Unsorted List | O(n)     | O(1)   | O(n)   | O(n)    | O(n)    | O(n)        | O(n)      |
| Singly Linked Sorted List   | O(n)     | O(n)   | O(n)   | O(n)    | O(1)    | O(n)        | O(1)      |
| Doubly Linked Unsorted List | O(n)     | O(1)   | O(1)   | O(n)    | O(n)    | O(1)*       | O(1)*     |
| Doubly Linked Sorted List   | O(n)     | O(n)   | O(1)   | O(1)    | O(1)    | O(1)*       | O(1)*     |

`*` Predecessor/successor are O(1) when a pointer to the relevant node is already available.

### Output

* Experimental data for plotting and comparing the order of growth of dictionary operations.

---

## Experiment 2 – Merge Sort vs Modified Merge Sort

### Objective

Compare the standard Merge Sort algorithm with a modified version that divides the input array into three parts instead of two.

### Standard Merge Sort

The standard Merge Sort divides the input into two halves, recursively sorts both halves, and then merges the sorted halves.

### Recurrence

```text
T(n) = 2T(n/2) + O(n)
```

### Complexity

```text
Θ(n log n)
```

### Modified Three-Way Merge Sort

The modified algorithm divides the input array into three parts, recursively sorts each part, and combines them using a three-way merge subroutine.

### Recurrence

```text
T(n) = 3T(n/3) + O(n)
```

### Complexity

```text
Θ(n log n)
```

Both standard Merge Sort and Three-Way Merge Sort have the same asymptotic order of growth, although their practical running times may differ because of implementation and constant-factor differences.

### Concepts Covered

* Divide and Conquer
* Recursion
* Merge Sort
* Three-Way Merge
* Recurrence Relations
* Master Theorem
* Time Complexity
* Experimental Comparison
* Order of Growth

### Output

* Experimental timing/operation data for Standard Merge Sort
* Experimental timing/operation data for Three-Way Merge Sort

---

## Experiment 3 – Merging k Sorted Arrays

### Objective

Merge `k` sorted arrays, each containing `n` elements, into one sorted array containing `kn` elements.

Two different approaches are considered.

---

### Method 1 – Sequential Merging

The first two arrays are merged, then the result is merged with the third array, then with the fourth array, and so on.

```text
A1 + A2
   ↓
Result + A3
   ↓
Result + A4
   ↓
...
   ↓
Result + Ak
```

### Complexity

The intermediate arrays continuously increase in size.

```text
2n + 3n + 4n + ... + kn
```

Therefore, the worst-case running time is:

```text
Θ(nk²)
```

---

### Method 2 – Pairwise Merging

The arrays are first divided into pairs and each pair is merged.

```text
A1 + A2
A3 + A4
A5 + A6
...
```

The resulting arrays are then merged pairwise again until only one sorted array remains.

```text
k arrays
   ↓
k/2 arrays
   ↓
k/4 arrays
   ↓
...
   ↓
1 array
```

There are approximately `log₂ k` levels, and every level processes all `kn` elements.

### Complexity

```text
Θ(kn log k)
```

### Comparison

| Method             | Worst-Case Complexity |
| ------------------ | --------------------- |
| Sequential Merging | Θ(nk²)                |
| Pairwise Merging   | Θ(kn log k)           |

Pairwise merging is asymptotically more efficient for large values of `k`.

### Concepts Covered

* Merging Sorted Arrays
* Divide and Conquer
* Pairwise Merging
* Sequential Merging
* Recurrence Relations
* Time Complexity
* Order of Growth
* Experimental Validation

### Output

* Experimental data for Sequential Merging
* Experimental data for Pairwise Merging

---

# Programming Language

* C

---

# Compiler

The programs can be compiled using **GCC**.

Example:

```bash
gcc filename.c -o output
```

Run:

### Linux/macOS

```bash
./output
```

### Windows

```bash
output.exe
```

---

# Repository Structure

```text
DAA/
│
├── README.md
│
├── Lab-01/
│   ├── Q1_Order_of_Growth/
│   │   ├── growth.c
│   │   ├── growth.csv
│   │   └── README.md
│   │
│   ├── Q2_Fair_vs_Biased_Coin/
│   │   ├── coin_simulation.c
│   │   ├── results.csv
│   │   └── README.md
│   │
│   ├── Q3_Bubble_Sort/
│   │   ├── bubble_sort.c
│   │   ├── bubble_sort_data.csv
│   │   ├── bubble_sort_sorted_data.csv
│   │   └── README.md
│   │
│   ├── Q4_Tower_of_Hanoi/
│   │   ├── tower_of_hanoi.c
│   │   ├── moves.csv
│   │   └── README.md
│   │
│   ├── Q5_Partition_Point/
│   │   ├── partition_point.c
│   │   └── README.md
│   │
│   └── Q6_Duplicate_Element/
│       ├── duplicate_detection.c
│       ├── comparisons.csv
│       └── README.md
│
└── Lab-02/
    ├── Q1_Dictionary_Operations/
    │   ├── dictionary.c
    │   ├── results.csv
    │   └── README.md
    │
    ├── Q2_Merge_Sort/
    │   ├── merge_sort.c
    │   ├── three_way_merge_sort.c
    │   ├── results.csv
    │   └── README.md
    │
    └── Q3_Merging_K_Sorted_Arrays/
        ├── sequential_merge.c
        ├── pairwise_merge.c
        ├── results.csv
        └── README.md
```

---

# Learning Outcomes

After completing these experiments, the following concepts were understood:

* Asymptotic Growth Analysis
* Big-O, Big-Omega and Big-Theta
* Time Complexity Analysis
* Bubble Sort Optimization
* Recursive Algorithms
* Binary Search
* Divide and Conquer
* Brute Force Techniques
* Probability Simulation
* Dictionary ADT
* Arrays and Linked Lists
* Singly and Doubly Linked Lists
* Merge Sort
* Three-Way Merge Sort
* Recurrence Relations
* Master Theorem
* Merging Multiple Sorted Arrays
* Performance Analysis of Algorithms
* Experimental Validation of Theoretical Complexities

---

# Complexity Summary

| Lab    | Experiment | Algorithm / Technique             | Time Complexity           |
| ------ | ---------- | --------------------------------- | ------------------------- |
| Lab-01 | Q1         | Growth Function Evaluation        | Depends on function       |
| Lab-01 | Q2         | Coin Toss Simulation              | O(n)                      |
| Lab-01 | Q3         | Bubble Sort                       | O(n²)                     |
| Lab-01 | Q4         | Tower of Hanoi                    | O(2ⁿ)                     |
| Lab-01 | Q5         | Linear Search                     | O(n)                      |
| Lab-01 | Q5         | Binary Search                     | O(log n)                  |
| Lab-01 | Q6         | Duplicate Detection (Brute Force) | O(n²)                     |
| Lab-02 | Q1         | Dictionary Operations             | Depends on data structure |
| Lab-02 | Q2         | Standard Merge Sort               | Θ(n log n)                |
| Lab-02 | Q2         | Three-Way Merge Sort              | Θ(n log n)                |
| Lab-02 | Q3         | Sequential Merging                | Θ(nk²)                    |
| Lab-02 | Q3         | Pairwise Merging                  | Θ(kn log k)               |

---

# Important DAA Concepts

## Asymptotic Analysis

Asymptotic analysis describes how the running time of an algorithm grows as the input size increases.

### Big-O

Upper bound:

```text
O(f(n))
```

### Big-Omega

Lower bound:

```text
Ω(f(n))
```

### Big-Theta

Tight bound:

```text
Θ(f(n))
```

---

## Divide and Conquer

A divide-and-conquer algorithm generally follows three steps:

```text
Divide → Conquer → Combine
```

Merge Sort and pairwise merging are important examples used in these labs.

---

## Recursion

Recursion occurs when a function calls itself to solve smaller versions of the same problem.

Tower of Hanoi and Merge Sort are examples of recursive algorithms.

---

## Recurrence Relations

Recurrence relations are used to express the running time of recursive algorithms.

Examples:

```text
Merge Sort:
T(n) = 2T(n/2) + O(n)

Three-Way Merge Sort:
T(n) = 3T(n/3) + O(n)
```

---

## Master Theorem

The Master Theorem is useful for analyzing recurrences of the form:

```text
T(n) = aT(n/b) + f(n)
```

It can be used to derive the complexity of standard and three-way Merge Sort.

---

# Software Requirements

* GCC Compiler
* Visual Studio Code / Code::Blocks / Dev-C++
* Windows / Linux / macOS

---

# Author

**Tanmay Tiwari**

**Student ID:** B325044

**Department:** Computer Engineering (CE)

**Semester:** 3rd Semester

**Institute:** IIIT Bhubaneswar

---

