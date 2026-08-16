# Design and Analysis of Algorithms (DAA)

---

## Repository Overview

This repository contains the C implementations and experimental analysis for **Lab-01, Lab-02, and Lab-03** of the **Design and Analysis of Algorithms (DAA)** course.

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

# Lab-03

## Experiments Included

## Experiment 1 – Binary vs Ternary Search

### Objective

In binary search, an `n`-element list is divided into nearly two equal halves; in ternary search, it is divided into nearly three equal intervals. Design and implement a C program to search for an element `x` in a sorted list of size `n` using both binary and ternary search, and justify/validate that binary search is better than ternary search.

### Approach

* `binarySearch` splits the range in two per iteration, using up to 2 comparisons per step.
* `ternarySearch` splits the range in three per iteration (via two midpoints), using up to 4 comparisons per step.
* A global comparison counter instruments both functions, and the program sums comparisons across searches for **every** element of a 100,000-element sorted array to empirically show binary search performs fewer total comparisons.

### Concepts Covered

* Divide and Conquer
* Binary Search
* Ternary Search
* Comparison Counting
* Asymptotic vs Empirical Analysis

### Complexity

| Algorithm      | Iterations | Comparisons/iteration | Total comparisons     |
| -------------- | ---------- | ---------------------- | ---------------------- |
| Binary search  | O(log₂ n)  | up to 2                 | O(2 log₂ n)            |
| Ternary search | O(log₃ n)  | up to 4                 | O(4 log₃ n) ≈ O(2.52 log₂ n) |

Both run in O(log n) time asymptotically, but binary search performs fewer total comparisons in practice — the extra comparisons per iteration in ternary search outweigh the benefit of its faster-shrinking range.

### Output

* `search_compare.c`
* Comparison counts for a single lookup and for a full sweep over all `n` elements

---

## Experiment 2 – Search the Defective (Lighter) Coin

### Objective

Given `n` coins where at most one coin may be lighter than the rest (never heavier), find the defective coin — or report that none exists — using a divide-and-conquer algorithm on a balance scale, bounded by `⌊log₂ n⌋ + c` weighings for some positive constant `c`.

### Approach

* `weigh()` simulates placing two coin ranges on a balance scale by comparing their summed weights, returning which side is lighter or whether they balance.
* `findLighter()` recursively splits the current range into two halves `L` and `R` (setting aside a leftover coin if the range size is odd), weighs `L` against `R`, and recurses into whichever side is lighter. If balanced, only the leftover coin (if any) can be defective.
* Once a single candidate remains, one **final confirming weighing** against another (now-guaranteed-genuine) coin verifies whether it is actually lighter, or whether all coins are genuine.

### Concepts Covered

* Divide and Conquer
* Balance-Scale Search Simulation
* Recursion
* Logarithmic Time Complexity

### Complexity

* **Weighings:** O(log n)
* **Space:** O(log n) recursion stack

### Notes

* Correctness assumes **at most one** defective (lighter) coin among the inputs.
* The "weighing" operation sums arbitrary-length coin ranges, which is a simulation for algorithmic analysis rather than a literal equal-pan-size physical balance.

### Output

* `find_lighter_coin.c`
* Reported defective coin index/weight (or "no defective coin") and total weighings used vs. the `log₂ n` reference

---

## Experiment 3 – Max and Min using D&C Approach

### Objective

Using divide and conquer, find the maximum and minimum elements of an array of size `n` such that the number of comparisons is bounded by `3n/2`.

### Approach

* `maxMin()` recursively splits the array in half, finds `(max, min)` of each half, and combines them using exactly 2 comparisons (one for the overall max, one for the overall min).
* Base cases: 1 element needs 0 comparisons; 2 elements need 1 comparison.
* A global comparison counter validates the `3n/2` bound experimentally.

### Concepts Covered

* Divide and Conquer
* Simultaneous Max-Min Finding
* Recurrence Relations
* Optimal Comparison-Based Algorithms

### Recurrence

```text
T(1) = 0
T(2) = 1
T(n) = 2T(n/2) + 2      for n > 2
```

### Complexity

* **Time:** O(n)
* **Comparisons:** ≈ 3n/2 − 2 (optimal for this problem)
* **Space:** O(log n) recursion stack

### Output

* `maxmin.c`
* Maximum, minimum, and total comparisons used vs. the `3n/2` bound

---

## Experiment 4 – Matrix Multiplication using D&C Approach (Strassen's Method)

### Objective

Write a C program to multiply two square matrices of size `n × n` using Strassen's method.

### Approach

* Matrices are recursively split into quadrants (`M11, M12, M21, M22`).
* Instead of the 8 multiplications needed by naive block multiplication, Strassen's algorithm computes 7 products (`P1..P7`) from sums/differences of quadrants, then combines them into the four result quadrants.
* Since Strassen's algorithm requires power-of-2 dimensions, input matrices are padded up to the next power of 2 before recursion, and the result is trimmed back to `n × n`.

### Concepts Covered

* Divide and Conquer
* Strassen's Algorithm
* Recurrence Relations
* Sub-cubic Matrix Multiplication

### Recurrence

```text
T(n) = 7T(n/2) + O(n²)
```

### Complexity

* **Time:** O(n^log₂7) ≈ O(n^2.807)
* **Space:** O(n²) plus recursive temporaries

### Notes

* For small matrices, the constant-factor overhead of Strassen's algorithm can make it slower in practice than naive O(n³) multiplication; the crossover favors Strassen mainly for large `n`.

### Output

* `strassen.c`
* Resulting product matrix `C = A × B`

---

## Experiment 5 – Multiply Special-Pattern Square Matrices using D&C Approach

### Objective

Two `n × n` matrices are given, where `n = 2^k`. Each matrix has a recursive block-symmetric structure:

```text
M = [ M1  M2 ]
    [ M2  M1 ]
```

where each block itself recursively follows the same structure down to single elements. Give a divide-and-conquer O(n²) algorithm to multiply two such matrices, and validate the complexity.

### Approach

* Because of the special pattern, only the top half (`M1`, `M2`) of each matrix needs to be read; the bottom half is redundant by the pattern.
* Using the identities `(M1+M2)(N1+N2) = M1N1+M1N2+M2N1+M2N2` and `(M1−M2)(N1−N2) = M1N1−M1N2−M2N1+M2N2`, only **2 recursive multiplications** (`S` and `D`) are needed per level, instead of the usual 4.
* `P1 = (S+D)/2 = M1N1+M2N2` and `P2 = (S−D)/2 = M1N2+M2N1` are derived, and the result is reassembled as `[[P1, P2], [P2, P1]]` — itself following the same special pattern.

### Concepts Covered

* Divide and Conquer
* Structural/Pattern Exploitation
* Recurrence Relations
* Sub-Strassen Multiplication for Structured Inputs

### Recurrence

```text
T(n) = 2T(n/2) + O(n²)
```

### Complexity

* **Time:** O(n² log n)
* **Space:** O(n²) plus recursive temporaries

### Notes

* Correctness requires the input matrices to genuinely satisfy the special pattern at every recursive level (`M21 = M12`, `M22 = M11`); this is not validated by the program, since the bottom half is never read.
* `n` must be a power of 2; the program does not pad non-power-of-2 sizes.

### Output

* `special_multiply.c`
* Resulting product matrix `C = M × N`

---

## Experiment 6 – Use of Loop Invariants in Sorting (Selection Sort)

### Objective

Sort `n` numbers stored in `A[1..n]` by repeatedly finding the smallest element of the unsorted suffix and exchanging it into place — first the smallest of `A[1..n]` into `A[1]`, then the smallest of `A[2..n]` into `A[2]`, and so on for the first `(n−1)` elements. State the loop invariant this algorithm maintains, prove initialization/maintenance/termination, explain why only the first `(n−1)` elements (not all `n`) need this process, give the worst-case running time in Θ-notation, and determine whether the best case is any better. Implement the algorithm in C to validate the claim.

### Approach — Selection Sort

**Pseudocode:**

```text
SELECTION-SORT(A, n)
for i = 1 to n - 1
    min_index = i
    for j = i + 1 to n
        if A[j] < A[min_index]
            min_index = j
    exchange A[i] with A[min_index]
```

### Loop Invariant

At the start of each iteration of the outer `for` loop (index `i`), the subarray `A[1..i-1]` consists of the `i-1` smallest elements of the original array, in sorted order.

* **Initialization:** Before the first iteration (`i = 1`), the subarray `A[1..0]` is empty, so the invariant holds trivially.
* **Maintenance:** Each iteration finds the minimum of the unsorted suffix `A[i..n]` and swaps it into `A[i]`, extending the sorted prefix `A[1..i-1]` to `A[1..i]` while keeping it sorted and containing the `i` smallest elements.
* **Termination:** The loop ends when `i = n`, so the invariant gives that `A[1..n-1]` holds the `n-1` smallest elements in sorted order. Since the single remaining element in `A[n]` must be the largest, the entire array `A[1..n]` is sorted.

### Why Only the First `(n−1)` Elements?

Once the first `n−1` elements have been correctly placed in sorted order via the loop invariant, the single remaining element (at position `n`) is, by elimination, the largest element and is already in its correct final position. Running an extra iteration for `i = n` would compare `A[n]` against itself and accomplish nothing.

### Concepts Covered

* Loop Invariants
* Proof of Correctness (Initialization, Maintenance, Termination)
* Selection Sort
* Best/Worst-Case Analysis

### Complexity

* **Worst-case running time:** Θ(n²) — the inner loop always scans the full remaining unsorted suffix regardless of input order.
* **Best-case running time:** Also Θ(n²) — unlike algorithms such as Bubble Sort (which can early-exit on an already-sorted array), Selection Sort always performs the same number of comparisons in the inner loop no matter the input, so the best case is **not** asymptotically better than the worst case. (The number of *swaps*, however, can be as low as 0 in the best case, versus O(n) in the worst case.)

### Output

* `selection_sort.c`
* Comparison/swap counts validating Θ(n²) behavior on both randomized and pre-sorted input

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
├── Lab-02/
│   ├── Q1_Dictionary_Operations/
│   │   ├── dictionary.c
│   │   ├── results.csv
│   │   └── README.md
│   │
│   ├── Q2_Merge_Sort/
│   │   ├── merge_sort.c
│   │   ├── three_way_merge_sort.c
│   │   ├── results.csv
│   │   └── README.md
│   │
│   └── Q3_Merging_K_Sorted_Arrays/
│       ├── sequential_merge.c
│       ├── pairwise_merge.c
│       ├── results.csv
│       └── README.md
│
└── Lab-03/
    ├── Q1_Binary_vs_Ternary_Search/
    │   ├── search_compare.c
    │   └── README.md
    │
    ├── Q2_Defective_Coin/
    │   ├── find_lighter_coin.c
    │   └── README.md
    │
    ├── Q3_Max_Min_DC/
    │   ├── maxmin.c
    │   └── README.md
    │
    ├── Q4_Strassen_Matrix_Multiplication/
    │   ├── strassen.c
    │   └── README.md
    │
    ├── Q5_Special_Pattern_Matrix_Multiplication/
    │   ├── special_multiply.c
    │   └── README.md
    │
    └── Q6_Loop_Invariant_Selection_Sort/
        ├── selection_sort.c
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
* Ternary Search
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
* Balance-Scale (Defective Coin) Search
* Simultaneous Max-Min Finding
* Strassen's Matrix Multiplication
* Structured/Special-Pattern Matrix Multiplication
* Loop Invariants and Correctness Proofs
* Selection Sort
* Performance Analysis of Algorithms
* Experimental Validation of Theoretical Complexities

---

# Complexity Summary

| Lab    | Experiment | Algorithm / Technique                        | Time Complexity            |
| ------ | ---------- | --------------------------------------------- | --------------------------- |
| Lab-01 | Q1         | Growth Function Evaluation                    | Depends on function         |
| Lab-01 | Q2         | Coin Toss Simulation                          | O(n)                        |
| Lab-01 | Q3         | Bubble Sort                                   | O(n²)                       |
| Lab-01 | Q4         | Tower of Hanoi                                | O(2ⁿ)                       |
| Lab-01 | Q5         | Linear Search                                 | O(n)                        |
| Lab-01 | Q5         | Binary Search                                 | O(log n)                    |
| Lab-01 | Q6         | Duplicate Detection (Brute Force)             | O(n²)                       |
| Lab-02 | Q1         | Dictionary Operations                         | Depends on data structure   |
| Lab-02 | Q2         | Standard Merge Sort                           | Θ(n log n)                  |
| Lab-02 | Q2         | Three-Way Merge Sort                          | Θ(n log n)                  |
| Lab-02 | Q3         | Sequential Merging                            | Θ(nk²)                      |
| Lab-02 | Q3         | Pairwise Merging                              | Θ(kn log k)                 |
| Lab-03 | Q1         | Binary Search                                 | O(2 log₂ n) comparisons     |
| Lab-03 | Q1         | Ternary Search                                | O(4 log₃ n) comparisons     |
| Lab-03 | Q2         | Defective Coin (Balance Scale, D&C)           | O(log n) weighings          |
| Lab-03 | Q3         | Max-Min Finding (D&C)                         | O(n) time, ≈3n/2 comparisons|
| Lab-03 | Q4         | Strassen's Matrix Multiplication              | O(n^log₂7) ≈ O(n^2.807)     |
| Lab-03 | Q5         | Special-Pattern Matrix Multiplication (D&C)   | O(n² log n)                 |
| Lab-03 | Q6         | Selection Sort (Loop Invariant)               | Θ(n²) worst and best case   |

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

Merge Sort, pairwise merging, Strassen's matrix multiplication, special-pattern matrix multiplication, the max-min algorithm, and the defective-coin search are all important examples used in these labs.

---

## Recursion

Recursion occurs when a function calls itself to solve smaller versions of the same problem.

Tower of Hanoi, Merge Sort, Strassen's algorithm, and the defective-coin search are examples of recursive algorithms.

---

## Recurrence Relations

Recurrence relations are used to express the running time of recursive algorithms.

Examples:

```text
Merge Sort:
T(n) = 2T(n/2) + O(n)

Three-Way Merge Sort:
T(n) = 3T(n/3) + O(n)

Max-Min (D&C):
T(n) = 2T(n/2) + 2

Strassen's Matrix Multiplication:
T(n) = 7T(n/2) + O(n²)

Special-Pattern Matrix Multiplication:
T(n) = 2T(n/2) + O(n²)
```

---

## Master Theorem

The Master Theorem is useful for analyzing recurrences of the form:

```text
T(n) = aT(n/b) + f(n)
```

It can be used to derive the complexity of standard Merge Sort, Three-Way Merge Sort, Strassen's algorithm, and the special-pattern matrix multiplication algorithm.

---

## Loop Invariants

A loop invariant is a property that holds true before and after each iteration of a loop, used to prove an algorithm's correctness via three steps:

* **Initialization** — the invariant holds before the first iteration.
* **Maintenance** — if the invariant holds before an iteration, it still holds before the next.
* **Termination** — when the loop ends, the invariant (combined with the loop's exit condition) yields a useful property that proves correctness.

Selection Sort (Lab-03, Q6) is used to demonstrate this technique.

---

# Software Requirements

* GCC Compiler
* Visual Studio Code / Code::Blocks / Dev-C++
* Windows / Linux / macOS
