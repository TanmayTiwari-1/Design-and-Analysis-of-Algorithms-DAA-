# README — Pair Sum from Two Sets in O(n log n)

## 1. Problem Statement

Given two sets `S1` and `S2`, each containing `n` elements, and a target value `x`, determine whether there exists:

- an element `a` from `S1`
- an element `b` from `S2`

such that:

```text
a + b = x
```

If such a pair exists, the program prints the pair.

The required time complexity is:

```text
O(n log n)
```

---

## 2. Main Idea

For every element `a` in `S1`, we need to find:

```text
b = x - a
```

So the problem becomes:

> Does `S2` contain the value `x - a`?

To answer this efficiently:

1. Make a copy of `S2`.
2. Sort the copy using `qsort()`.
3. For every element `a` in `S1`:
   - Calculate `need = x - a`.
   - Perform binary search for `need` in the sorted `S2`.
4. If the value is found, a valid pair exists.

---

## 3. Example

Suppose:

```text
S1 = {2, 5, 8, 10}
S2 = {1, 3, 6, 7}
x = 9
```

For `a = 2`:

```text
need = 9 - 2
     = 7
```

Since `7` exists in `S2`:

```text
2 + 7 = 9
```

Therefore, a valid pair is found.

Output:

```text
Pair found: 2 (from S1) + 7 (from S2) = 9
```

---

## 4. Algorithm

### Step 1 — Sort `S2`

A copy of `S2` is created so that the original set is not modified.

```text
sortedS2 = copy of S2
sort(sortedS2)
```

Sorting takes:

```text
O(n log n)
```

---

### Step 2 — Search for the Required Value

For every `a` in `S1`, calculate:

```text
need = x - a
```

Then perform binary search in `sortedS2`.

Binary search takes:

```text
O(log n)
```

for each element.

Since there are `n` elements in `S1`:

```text
n × O(log n) = O(n log n)
```

---

## 5. Binary Search

The function:

```c
int binarySearch(int arr[], int n, int target)
```

searches for `target` in the sorted array.

It maintains two boundaries:

```text
lo = 0
hi = n - 1
```

At every step:

```text
mid = lo + (hi - lo) / 2
```

Then:

- If `arr[mid] == target`, the value is found.
- If `arr[mid] < target`, search the right half.
- Otherwise, search the left half.

The search space is approximately halved after every iteration.

### Binary Search Complexity

```text
Time:  O(log n)
Space: O(1)
```

---

## 6. `findPairSum()` Function

```c
int findPairSum(int S1[], int S2[], int n, int x,
                int *foundA, int *foundB)
```

This is the main function that solves the problem.

### Steps performed:

1. Allocate memory for `sortedS2`.
2. Copy all elements of `S2`.
3. Sort `sortedS2`.
4. Iterate through every element of `S1`.
5. Calculate the required value:

```text
need = x - S1[i]
```

6. Binary-search for `need`.
7. If found:
   - Store the values in `foundA` and `foundB`.
   - Free allocated memory.
   - Return `1`.
8. If no pair exists:
   - Free memory.
   - Return `0`.

---

## 7. Why Do We Copy `S2`?

The program creates:

```c
int *sortedS2 = malloc(n * sizeof(int));
```

and copies the elements of `S2` into it.

This means the original `S2` remains unchanged.

The sorting operation is performed only on the copy:

```c
qsort(sortedS2, n, sizeof(int), cmp);
```

This is useful when the original input sets need to be preserved.

---

## 8. Sorting Function

The program uses the C standard library function:

```c
qsort()
```

with the comparison function:

```c
int cmp(const void *a, const void *b)
```

The comparison function determines the ascending order of the integers.

Sorting `n` elements takes:

```text
O(n log n)
```

on typical implementations of `qsort()`.

---

## 9. Time Complexity Analysis

There are two major operations.

### Sorting `S2`

```text
O(n log n)
```

### Binary searches

There are `n` elements in `S1`.

Each requires a binary search:

```text
O(log n)
```

Therefore:

```text
n × O(log n) = O(n log n)
```

### Total

```text
O(n log n) + O(n log n)
= O(n log n)
```

Therefore, the complete algorithm satisfies the required:

```text
Time Complexity = O(n log n)
```

---

## 10. Space Complexity

The program creates a copy of `S2`:

```text
sortedS2
```

which requires `O(n)` additional memory.

Therefore:

```text
Auxiliary Space Complexity = O(n)
```

The binary search itself uses:

```text
O(1)
```

extra space because it is implemented iteratively.

---

## 11. Input Format

First enter the size of each set:

```text
n
```

Then enter:

```text
n elements of S1
n elements of S2
target value x
```

### Example Input

```text
4
2 5 8 10
1 3 6 7
9
```

---

## 12. Expected Output

```text
Pair found: 2 (from S1) + 7 (from S2) = 9
```

If no pair exists, the program prints:

```text
No pair from S1 and S2 adds up to 9
```

---

## 13. Important Concepts

This program demonstrates:

- Pair-sum problem
- Sorting
- Binary search
- Divide-and-conquer searching
- `qsort()` in C
- Dynamic memory allocation
- Pointer parameters
- Time-complexity analysis
- Space-complexity analysis
- Reducing a search problem to `x - a`

---

## 14. Why Not Use a Simple Nested Loop?

A straightforward approach would compare every element of `S1` with every element of `S2`.

That would require:

```text
n × n = n²
```

comparisons.

Therefore:

```text
Nested-loop approach = O(n²)
```

The given solution improves this to:

```text
O(n log n)
```

by sorting `S2` once and using binary search.

---

## 15. Alternative Approach

Another possible solution is to sort both sets and use two pointers.

For example:

```text
i = 0
j = n - 1

while i < n and j >= 0:
    sum = S1[i] + S2[j]

    if sum == x:
        pair found

    if sum < x:
        i++

    else:
        j--
```

After sorting both sets, the two-pointer search takes `O(n)`, giving an overall complexity of:

```text
O(n log n)
```

However, the current program specifically demonstrates the **sort + binary search** approach.

---

## 16. Key Observation

The most important mathematical transformation is:

```text
a + b = x
```

Rearranging:

```text
b = x - a
```

This allows us to fix an element `a` from `S1` and search for the exact required value in `S2`.

Because `S2` is sorted, binary search can find this value in `O(log n)` time.

---

## 17. Conclusion

The program efficiently solves the two-set pair-sum problem by sorting `S2` and performing binary searches for `x - a` for every element `a` in `S1`.

The total complexity is:

```text
Sorting:       O(n log n)
n searches:    O(n log n)
--------------------------------
Total:         O(n log n)
```

Thus, the solution meets the required **O(n log n)** time complexity while using **O(n)** additional space.