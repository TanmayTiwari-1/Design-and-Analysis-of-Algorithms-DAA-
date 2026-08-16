# Selection Sort in C

## 1. Aim
To implement the **Selection Sort** algorithm in C and sort an array of integers in ascending order.

## 2. Description
Selection Sort is a simple comparison-based sorting algorithm. It repeatedly finds the **smallest element** from the unsorted portion of the array and places it at the beginning of that portion.

For every position `i`:
1. Assume `A[i]` is the minimum element.
2. Search the remaining unsorted elements.
3. Find the actual minimum element.
4. Swap it with `A[i]`.
5. Repeat until the array is sorted.

## 3. Algorithm
1. Read the number of elements `n`.
2. Read `n` elements into array `A`.
3. Set `i = 0`.
4. Assume `A[i]` is the minimum element.
5. Compare it with every element from `A[i+1]` to `A[n-1]`.
6. Store the index of the smallest element in `min_idx`.
7. Swap `A[i]` and `A[min_idx]`.
8. Increment `i`.
9. Repeat until `i = n - 1`.
10. Print the sorted array.

## 4. Example

**Input:**
```text
Enter number of elements: 5
Enter 5 elements: 64 25 12 22 11
```

**Output:**
```text
Sorted array: 11 12 22 25 64
```

## 5. Important Concepts

### Selection Sort
The algorithm divides the array into:
- **Sorted portion** – elements on the left.
- **Unsorted portion** – remaining elements on the right.

In every pass, the smallest element from the unsorted portion is selected and moved to the sorted portion.

### `min_idx`
`min_idx` stores the index of the smallest element found during the current pass.

```c
int min_idx = i;
```

Whenever a smaller element is found:

```c
if (A[j] < A[min_idx])
    min_idx = j;
```

### Swapping
The selected minimum element is swapped with the first element of the unsorted portion:

```c
int tmp = A[i];
A[i] = A[min_idx];
A[min_idx] = tmp;
```

## 6. Complexity Analysis

| Case | Time Complexity |
|---|---|
| Best Case | O(n²) |
| Average Case | O(n²) |
| Worst Case | O(n²) |

**Space Complexity:** `O(1)` auxiliary space because the sorting is performed directly in the original array.

## 7. Properties
- Comparison-based sorting algorithm.
- In-place sorting algorithm.
- Requires no additional array.
- Simple and easy to implement.
- Performs a fixed number of comparisons regardless of the initial ordering.
- The given implementation sorts in **ascending order**.

## 8. Program Structure

### `selectionSort()`
The function performs the complete selection sort operation.

```c
void selectionSort(int A[], int n)
```

### `main()`
The `main()` function:
- Takes the array size as input.
- Reads array elements.
- Calls `selectionSort()`.
- Prints the sorted array.

## 9. Note
The array is declared as:

```c
int A[1000];
```

Therefore, the program is designed to store up to **1000 integer elements**. The input value of `n` should not exceed 1000.

## 10. Conclusion
The program successfully implements **Selection Sort** to arrange integer elements in ascending order. The algorithm is simple and uses constant extra space, but its `O(n²)` time complexity makes it less efficient for large datasets compared with algorithms such as Merge Sort or Quick Sort.