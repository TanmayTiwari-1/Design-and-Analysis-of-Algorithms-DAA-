# README — Stable Sorting of Number–Colour Pairs

## 1. Problem Statement

Given `n` pairs of the form:

```text
(number, colour)
```

the input is **already sorted by number**. Each item has one of only three colours:

- `RED`
- `BLUE`
- `YELLOW`

The task is to sort the items by colour in the order:

```text
RED → BLUE → YELLOW
```

while ensuring that the numbers within each colour remain sorted.

---

## 2. Main Idea

Since the input is already sorted by **number**, we do not need to perform a comparison-based sorting algorithm.

Instead, this problem can be treated as a **stable bucketing problem**, similar to counting sort with three buckets.

We make three passes over the input:

1. Copy all `RED` items to the output.
2. Copy all `BLUE` items to the output.
3. Copy all `YELLOW` items to the output.

During each pass, the input is scanned from left to right. Therefore, the original number order is automatically preserved within each colour.

### Example

Input:

```text
(2, BLUE)
(4, RED)
(6, BLUE)
(8, YELLOW)
(10, RED)
(12, YELLOW)
```

Output:

```text
(4, RED)
(10, RED)
(2, BLUE)
(6, BLUE)
(8, YELLOW)
(12, YELLOW)
```

Notice that:

- RED numbers: `4, 10` → sorted
- BLUE numbers: `2, 6` → sorted
- YELLOW numbers: `8, 12` → sorted

---

## 3. Algorithm

### `sortByColour(arr, n, out)`

```text
idx = 0

For each colour RED, BLUE, YELLOW:
    Scan the complete input array
    If item's colour matches the current colour:
        Copy item to output
        Increment idx
```

The algorithm does not compare the numbers at all.

---

## 4. Why Stability Is Important

The input array is already sorted by number.

When we scan it from left to right and append matching items to the output, the relative order of items having the same colour is preserved.

For example:

```text
(3, RED)
(7, RED)
(12, RED)
```

will remain:

```text
(3, RED)
(7, RED)
(12, RED)
```

after the colour-based sorting.

Therefore, the numbers within every colour group remain sorted automatically.

---

## 5. Data Structures Used

### `Colour` enum

```c
typedef enum { RED, BLUE, YELLOW } Colour;
```

This represents the three possible colours using integer values:

```text
RED    = 0
BLUE   = 1
YELLOW = 2
```

### `Item` structure

```c
typedef struct {
    int number;
    Colour colour;
} Item;
```

Each item contains:

- `number` — the numerical value
- `colour` — the colour associated with that number

---

## 6. Functions

### `colourName()`

```c
const char* colourName(Colour c)
```

Converts the enum value into a readable string:

```text
RED
BLUE
YELLOW
```

This is used when printing the final output.

### `sortByColour()`

```c
void sortByColour(Item arr[], int n, Item out[])
```

Performs the stable colour-based bucketing.

It uses three passes:

```text
RED → BLUE → YELLOW
```

and stores the result in `out`.

---

## 7. Time Complexity

There are exactly **3 colours**.

For every colour, the entire array of `n` elements is scanned:

```text
3 × n = 3n
```

Therefore:

```text
Time Complexity = O(3n) = O(n)
```

Since the number of colours is fixed at 3, the algorithm has **linear time complexity**.

### Space Complexity

An additional output array of size `n` is used:

```text
Space Complexity = O(n)
```

The input array itself also requires `O(n)` space.

---

## 8. Important Observation

This is **not a normal comparison-based sorting problem**.

We do not need:

- Bubble Sort
- Selection Sort
- Insertion Sort
- Merge Sort
- Quick Sort

because:

1. The input is already sorted by number.
2. There are only three possible colour values.
3. We only need to group elements according to their colour.
4. Stability automatically preserves the sorted number order.

Thus, a simple stable bucketing approach is more efficient and appropriate.

---

## 9. Input Format

First enter the number of items:

```text
n
```

Then enter each item as:

```text
number colour
```

where:

```text
0 = RED
1 = BLUE
2 = YELLOW
```

The numbers must already be entered in sorted order.

### Example Input

```text
6
2 1
4 0
6 1
8 2
10 0
12 2
```

---

## 10. Expected Output

```text
Sorted by colour (RED, then BLUE, then YELLOW), numbers stay sorted within each colour:
(4, RED)
(10, RED)
(2, BLUE)
(6, BLUE)
(8, YELLOW)
(12, YELLOW)
```

---

## 11. Key Concepts

This program demonstrates:

- Stable sorting
- Bucketing
- Counting-sort style processing
- Enumeration (`enum`) in C
- Structures (`struct`) in C
- Dynamic memory allocation using `malloc()`
- Preservation of relative order
- Linear-time algorithms
- Exploiting already-sorted input

---

## 12. Advantages

- Very simple implementation.
- Runs in `O(n)` time.
- No comparisons between numbers are required.
- Preserves the sorted order of numbers within each colour.
- Efficient because the number of colours is fixed.

## 13. Limitation

The algorithm uses `O(n)` additional memory for the output array.

Also, the approach relies on the important assumption that the input is **already sorted by number**. If the input were not sorted by number, an additional sorting step would be required.

---

## 14. Conclusion

The problem can be solved efficiently using **stable bucketing** instead of comparison-based sorting. Since the input is already sorted by number, scanning it from left to right for each colour preserves the number order within every colour group. With only three possible colours, the algorithm performs three linear passes, giving an overall time complexity of **O(n)**.