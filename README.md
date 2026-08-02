# Design and Analysis of Algorithms (DAA) Lab-01

<div align="center">

## **IIIT Bhubaneswar*

---

</div>

## Repository Overview

This repository contains the C implementations and experimental analysis for **Lab-01** of the **Design and Analysis of Algorithms (DAA)** course.

The objective of this laboratory is to understand the implementation, analysis, and comparison of different algorithms using practical experiments. Each experiment includes source code, generated datasets, observations, complexity analysis, and README documentation.

---

## Course Information

| Field | Details |
|-------|---------|
| **Department** | Computer Engineering (CE) |
| **Course** | Design and Analysis of Algorithms (DAA) |
| **Semester** | 3rd Semester |
| **Student Name** | Tanmay Tiwari |
| **Student ID** | B325044 |

---

# Experiments Included

## Experiment 1 – Order of Growth of Functions

### Objective

Compare various mathematical functions and arrange them in increasing order of asymptotic growth.

### Concepts Covered

- Asymptotic Analysis
- Big-O Notation
- Growth of Functions
- Logarithmic Plotting

### Output

- `growth.csv`

---

## Experiment 2 – Fair vs Biased Coin Simulation

### Objective

Simulate tossing a fair coin and a biased coin to verify experimental probabilities.

### Concepts Covered

- Probability Simulation
- Random Number Generation
- Law of Large Numbers

### Output

- `results.csv`

---

## Experiment 3 – Bubble Sort Performance Analysis

### Objective

Compare Optimized Bubble Sort and Unoptimized Bubble Sort.

### Concepts Covered

- Bubble Sort
- Best Case
- Worst Case
- Comparison Counting
- Time Complexity

### Output Files

- `bubble_sort_data.csv`
- `bubble_sort_sorted_data.csv`

---

## Experiment 4 – Tower of Hanoi

### Objective

Determine the minimum number of moves required to solve the Tower of Hanoi problem using recursion.

### Concepts Covered

- Recursion
- Recurrence Relation
- Exponential Time Complexity

### Output

- `moves.csv`

---

## Experiment 5 – Partition Point Detection

### Objective

Find the first occurrence of `1` in a binary array containing consecutive `0`s followed by consecutive `1`s.

### Concepts Covered

- Linear Search
- Binary Search
- Comparison Analysis

---

## Experiment 6 – Duplicate Element Detection

### Objective

Determine whether a randomly generated array contains duplicate elements using the Brute Force approach.

### Concepts Covered

- Brute Force Algorithm
- Comparison Counting
- Worst Case Analysis

### Output

- `comparisons.csv`

---

# Programming Language

- C

---

# Compiler

The programs can be compiled using **GCC**.

Example:

```bash
gcc filename.c -o output
```

Run:

Linux/macOS

```bash
./output
```

Windows

```bash
output.exe
```

---

# Repository Structure

```
DAA-Lab-01/
│
├── Q1_Order_of_Growth/
│   ├── growth.c
│   ├── growth.csv
│   └── README.md
│
├── Q2_Fair_vs_Biased_Coin/
│   ├── coin_simulation.c
│   ├── results.csv
│   └── README.md
│
├── Q3_Bubble_Sort/
│   ├── bubble_sort.c
│   ├── bubble_sort_data.csv
│   ├── bubble_sort_sorted_data.csv
│   └── README.md
│
├── Q4_Tower_of_Hanoi/
│   ├── tower_of_hanoi.c
│   ├── moves.csv
│   └── README.md
│
├── Q5_Partition_Point/
│   ├── partition_point.c
│   └── README.md
│
├── Q6_Duplicate_Element/
│   ├── duplicate_detection.c
│   ├── comparisons.csv
│   └── README.md
│
└── README.md
```

---

# Learning Outcomes

After completing these experiments, the following concepts were understood:

- Asymptotic Growth Analysis
- Time Complexity Analysis
- Bubble Sort Optimization
- Recursive Algorithms
- Binary Search
- Divide and Conquer
- Brute Force Techniques
- Probability Simulation
- Performance Analysis of Algorithms
- Experimental Validation of Theoretical Complexities

---

# Complexity Summary

| Experiment | Algorithm | Time Complexity |
|------------|-----------|----------------|
| Q1 | Growth Function Evaluation | O(log n) |
| Q2 | Coin Toss Simulation | O(n) |
| Q3 | Bubble Sort | O(n²) |
| Q4 | Tower of Hanoi | O(2ⁿ) |
| Q5 | Linear Search | O(n) |
| Q5 | Binary Search | O(log n) |
| Q6 | Duplicate Detection (Brute Force) | O(n²) |

---

# Software Requirements

- GCC Compiler
- Visual Studio Code / Code::Blocks / Dev-C++
- Windows / Linux / macOS

---

# Author

**Tanmay Tiwari**

**Student ID:** B325044

**Department:** Computer Engineering (CE)

**Institute:** IIIT Bhubaneswar

---

## Acknowledgement

I sincerely thank **Dr. Ajaya Kumar Dash** and the **Department of Computer Engineering, IIIT Bhubaneswar**, for providing the opportunity to perform these laboratory experiments and gain practical knowledge of algorithm design, implementation, and performance analysis.

---

⭐ If you found this repository useful, consider giving it a star.
