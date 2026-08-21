# README — k-Sum Problem Using Sorting and Two-Pointer Technique

## 1. Problem Statement

Given a set `S` containing `n` integers, an integer `k`, and a target value `T`, determine whether there are **k distinct elements** in `S` whose sum is equal to `T`.

In other words, we need to find:

```text
a₁ + a₂ + a₃ + ... + aₖ = T
```

The required time complexity is:

```text
O(n^(k-1) log n)
```

The program uses a combination of:

- Sorting
- Recursive selection of `k-2` elements
- Two-pointer technique for the remaining 2 elements

---

## 2. Main Idea

The problem generalizes the familiar **2-Sum** and **3-Sum** problems.

The algorithm works as follows:

1. Sort the input array.
2. Select the first `k-2` elements using recursive nested-loop-style enumeration.
3. Calculate the remaining sum:

```text
R = T - sum of selected elements
```

4. Find two additional elements whose sum is `R`.
5. Use the **two-pointer technique** to find those two elements efficiently.

Thus, instead of checking all `k` elements using `k` nested loops, the problem is reduced to:

```text
Choose k-2 elements + solve a 2-Sum problem
```

---

## 3. Example

Suppose:

```text
S = {2, 4, 6, 8, 10}
k = 3
T = 18
```

We need three numbers whose sum is `18`.

Choose the first number:

```text
2
```

The remaining required sum is:

```text
R = 18 - 2
  = 16
```

Now find two numbers that sum to `16`.

Using the two-pointer technique:

```text
6 + 10 = 16
```

Therefore:

```text
2 + 6 + 10 = 18
```

A valid combination is found.

---

## 4. Why Sort the Array?

The array is sorted using:

```c
qsort(arr, n, sizeof(int), cmp);
```

Sorting is important because the two-pointer technique requires the elements to be in sorted order.

For example:

```text
Before sorting:
8 2 10 4 6

After sorting:
2 4 6 8 10
```

The sorted order allows the two pointers to decide which direction to move based on whether the current sum is too small or too large.

### Sorting Complexity

```text
O(n log n)
```

---

## 5. Two-Pointer Technique

The function:

```c
int twoPointerFind(int target, int lo, int hi,
                   int *outI, int *outJ)
```

searches for two elements whose sum is equal to `target`.

Two pointers are used:

```text
lo → beginning of the search range
hi → end of the search range
```

At every step:

```text
sum = arr[lo] + arr[hi]
```

### Case 1 — Sum is Equal

If:

```text
sum == target
```

a valid pair is found.

### Case 2 — Sum is Too Small

If:

```text
sum < target
```

increase `lo`.

Because the array is sorted, moving `lo` to the right increases the sum.

### Case 3 — Sum is Too Large

If:

```text
sum > target
```

decrease `hi`.

Moving `hi` to the left decreases the sum.

---

## 6. Two-Pointer Complexity

Each pointer moves only in one direction.

Therefore, the total number of pointer movements is at most proportional to `n`.

Hence:

```text
Time Complexity = O(n)
Space Complexity = O(1)
```

for the two-pointer search itself.

---

## 7. Selecting the First `k-2` Elements

The function:

```c
int chooseFixed(int startIdx, int chosenCount,
                long sumSoFar, int T)
```

recursively chooses `k-2` indices.

For example, if:

```text
k = 4
```

then:

```text
k - 2 = 2
```

elements are selected first.

For each possible selection, the algorithm solves the remaining 2-Sum problem.

The selected indices are stored in:

```c
int *chosenIdx;
```

---

## 8. Why Are Indices Strictly Increasing?

The recursive call uses:

```c
chooseFixed(i + 1, ...)
```

instead of starting again from `i`.

This ensures:

```text
i₁ < i₂ < i₃ < ... < iₖ
```

Therefore, the same array element cannot be selected twice.

It also avoids generating the same combination in different orders.

For example, the combination:

```text
2, 5, 8
```

is considered once rather than also considering:

```text
5, 2, 8
8, 5, 2
```

and so on.

---

## 9. Important Restriction in the Two-Pointer Step

After selecting the first `k-2` elements, the remaining two elements are searched only in:

```text
[startIdx, n-1]
```

This is important because it prevents the algorithm from reusing an element that has already been selected.

The code:

```c
int lo = startIdx;
int hi = n - 1;
```

ensures that the two final elements come after the selected indices.

---

## 10. Recursive Algorithm

The overall recursive process can be summarized as:

```text
chooseFixed(startIdx, chosenCount, sumSoFar)

If chosenCount == k-2:
    Calculate remaining target:
        R = T - sumSoFar

    Use two-pointer search for R
    If pair is found:
        return success

Otherwise:
    For every possible next index:
        choose the index
        recursively choose the next element
```

This effectively behaves like nested loops for selecting `k-2` elements, but recursion allows the algorithm to work for arbitrary `k`.

---

## 11. Special Case: k = 2

The program also handles the normal 2-Sum problem separately.

If:

```text
k == 2
```

there are no fixed elements to choose.

The program directly performs:

```text
twoPointerFind(T, 0, n-1, ...)
```

This takes:

```text
O(n)
```

after sorting.

Therefore, for `k = 2`:

```text
Sorting = O(n log n)
Two-pointer search = O(n)

Total = O(n log n)
```

---

## 12. Handling Invalid k

The program checks:

```c
if (k < 2)
```

and prints:

```text
k must be >= 2
```

because the implemented approach is designed for finding combinations of at least two numbers.

---

## 13. Time Complexity

### Sorting

Sorting the array takes:

```text
O(n log n)
```

### Selecting `k-2` Elements

The algorithm considers combinations of approximately:

```text
O(n^(k-2))
```

possible choices.

### Two-Pointer Search

For each choice, the remaining two numbers are found in:

```text
O(n)
```

Therefore:

```text
O(n^(k-2)) × O(n)
= O(n^(k-1))
```

Including the sorting step:

```text
O(n log n) + O(n^(k-1))
```

For `k >= 3`, the dominant term is:

```text
O(n^(k-1))
```

The question allows the upper bound:

```text
O(n^(k-1) log n)
```

so the implementation satisfies the required asymptotic bound.

---

## 14. Space Complexity

The program uses:

- `arr` of size `n`
- `chosenIdx` of size `k`
- `foundCombo` of fixed size `32`
- Recursive call stack

The main additional storage is:

```text
O(n + k)
```

apart from the input array.

The recursive depth is at most `k-2`.

---

## 15. Example Input

```text
8
1 3 5 7 9 11 13 15
4
24
```

Here:

```text
n = 8
k = 4
T = 24
```

The program searches for four numbers whose sum is `24`.

One possible solution is:

```text
1 + 3 + 5 + 15 = 24
```

---

## 16. Example Output

```text
Found 4 numbers that sum to 24: 1 3 5 15
```

If no combination exists:

```text
No 4 numbers add up to 24
```

---

## 17. Important Functions

### `cmp()`

```c
int cmp(const void *a, const void *b)
```

Comparison function used by `qsort()` to sort integers in ascending order.

---

### `twoPointerFind()`

```c
int twoPointerFind(int target, int lo, int hi,
                   int *outI, int *outJ)
```

Finds two numbers in a sorted range whose sum equals `target`.

Complexity:

```text
O(n)
```

---

### `chooseFixed()`

```c
int chooseFixed(int startIdx, int chosenCount,
                long sumSoFar, int T)
```

Recursively selects the first `k-2` elements and then invokes the two-pointer search for the remaining two elements.

---

### `main()`

The `main()` function:

1. Reads `n`.
2. Reads the set of integers.
3. Reads `k`.
4. Reads target `T`.
5. Sorts the array.
6. Handles `k = 2` separately.
7. Uses recursion for `k > 2`.
8. Prints the result.
9. Frees dynamically allocated memory.

---

## 18. Why This Approach Is Better Than Brute Force

A direct brute-force solution could use `k` nested loops.

Its complexity would be approximately:

```text
O(n^k)
```

The current approach reduces this by solving the final two elements using the two-pointer technique.

Instead of:

```text
Choose k elements directly
```

we use:

```text
Choose k-2 elements
        ↓
Find remaining 2 using two pointers
```

This reduces the main search from approximately:

```text
O(n^k)
```

to:

```text
O(n^(k-1))
```

after sorting.

---

## 19. Key Concepts

This program demonstrates:

- k-Sum problem
- 2-Sum problem
- Recursion
- Sorting
- `qsort()` in C
- Two-pointer technique
- Combination generation
- Index-based selection
- Avoiding repeated elements
- Dynamic memory allocation
- Time-complexity analysis
- Space-complexity analysis

---

## 20. Important Observation

The most important idea is to reduce a `k`-sum problem to a smaller problem.

For every selection of `k-2` elements:

```text
sumSoFar = a₁ + a₂ + ... + aₖ₋₂
```

the remaining target is:

```text
R = T - sumSoFar
```

The problem then becomes:

```text
Find a + b = R
```

which is a standard 2-Sum problem that can be solved efficiently using two pointers because the array is sorted.

---

## 21. Conclusion

This program solves the general **k-Sum problem** by combining recursion, sorting, and the two-pointer technique.

The main strategy is:

```text
Sort the array
      ↓
Choose k-2 elements recursively
      ↓
Calculate remaining target
      ↓
Find the final 2 elements using two pointers
      ↓
Return the combination if found
```

The resulting search complexity is:

```text
O(n^(k-1))
```

with sorting requiring:

```text
O(n log n)
```

and therefore it satisfies the required upper bound of:

```text
O(n^(k-1) log n)
```

The approach is significantly more efficient than using `k` nested loops and demonstrates how a difficult combinatorial search can be reduced using the **two-pointer technique**.