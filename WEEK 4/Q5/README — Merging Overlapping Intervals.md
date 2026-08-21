# README — Merging Overlapping Intervals

## 1. Problem Statement

Given `n` intervals of the form:

```text
(xᵢ, yᵢ)
```

the task is to merge all intervals that overlap.

For example:

```text
Input:
(1,3), (2,6), (8,10), (7,18)
```

After merging:

```text
Output:
(1,6), (7,18)
```

The required time complexity is:

```text
O(n log n)
```

---

## 2. Main Idea

The algorithm uses two main steps:

1. **Sort all intervals by their starting point.**
2. **Scan the sorted intervals from left to right** and merge overlapping intervals.

After sorting, intervals that may overlap will appear next to each other.

For the current merged interval:

```text
current = (start, end)
```

look at the next interval:

```text
next = (nextStart, nextEnd)
```

### If they overlap

If:

```text
nextStart <= currentEnd
```

then the intervals overlap.

Extend the current interval's end:

```text
currentEnd = max(currentEnd, nextEnd)
```

### If they do not overlap

If:

```text
nextStart > currentEnd
```

the current interval is complete.

Store it in the output and start a new current interval.

---

## 3. Example

Consider:

```text
(1,3)
(2,6)
(8,10)
(7,18)
```

### Step 1 — Sort by Start

The sorted intervals are:

```text
(1,3)
(2,6)
(7,18)
(8,10)
```

### Step 2 — Start with the First Interval

```text
current = (1,3)
```

### Step 3 — Compare `(2,6)`

Since:

```text
2 <= 3
```

the intervals overlap.

Extend the end:

```text
current = (1,6)
```

### Step 4 — Compare `(7,18)`

Now:

```text
7 > 6
```

so there is no overlap.

Store:

```text
(1,6)
```

and start:

```text
current = (7,18)
```

### Step 5 — Compare `(8,10)`

Since:

```text
8 <= 18
```

the intervals overlap.

The end remains:

```text
max(18,10) = 18
```

So:

```text
current = (7,18)
```

Finally, store the last interval.

Result:

```text
(1,6), (7,18)
```

---

## 4. Why Sorting Is Necessary

Without sorting, overlapping intervals could appear far apart in the input.

For example:

```text
(8,10), (1,3), (7,18), (2,6)
```

It is difficult to merge them efficiently in this order.

After sorting:

```text
(1,3), (2,6), (7,18), (8,10)
```

all potentially overlapping intervals are processed in a convenient order.

The program uses:

```c
qsort(arr, n, sizeof(Interval), cmp);
```

to sort the intervals by their starting value `x`.

---

## 5. Interval Structure

The program defines:

```c
typedef struct {
    int x, y;
} Interval;
```

where:

- `x` = starting point
- `y` = ending point

For example:

```text
(5,10)
```

is represented as:

```text
x = 5
y = 10
```

---

## 6. Comparison Function

The function:

```c
int cmp(const void *a, const void *b)
```

is used by `qsort()`.

It compares:

```text
a->x
```

and:

```text
b->x
```

so that intervals are sorted according to their starting points.

For example:

```text
Before:
(8,10), (1,4), (5,9)

After:
(1,4), (5,9), (8,10)
```

---

## 7. `mergeIntervals()` Function

The main algorithm is implemented in:

```c
int mergeIntervals(Interval arr[], int n, Interval out[])
```

This function:

1. Checks whether `n` is zero.
2. Sorts the intervals.
3. Takes the first interval as the current interval.
4. Scans through the remaining intervals.
5. Checks for overlap.
6. Extends the current interval when necessary.
7. Stores completed merged intervals in `out`.
8. Returns the number of merged intervals.

---

## 8. Overlap Condition

The key condition is:

```c
if (arr[i].x <= cur.y)
```

This means:

```text
next.start <= current.end
```

Therefore, the next interval overlaps or touches the current interval.

For example:

```text
Current: (1,5)
Next:    (5,8)
```

Since:

```text
5 <= 5
```

the program merges them:

```text
(1,8)
```

Thus, the program treats intervals that meet at an endpoint as overlapping.

---

## 9. Extending the Current Interval

When two intervals overlap, the program uses:

```c
if (arr[i].y > cur.y)
    cur.y = arr[i].y;
```

This keeps the larger ending value.

For example:

```text
Current: (1,10)
Next:    (3,7)
```

The merged interval remains:

```text
(1,10)
```

because `10 > 7`.

For:

```text
Current: (1,5)
Next:    (3,9)
```

the merged interval becomes:

```text
(1,9)
```

---

## 10. Non-Overlapping Intervals

If:

```text
next.start > current.end
```

there is no overlap.

The current interval is finished and stored:

```c
out[count++] = cur;
```

Then the next interval becomes the new current interval:

```c
cur = arr[i];
```

---

## 11. Why the Greedy Scan Works

After sorting by starting point, the current interval always represents the complete merged range of all overlapping intervals processed so far.

If the next interval starts after the current end:

```text
next.start > current.end
```

then it cannot overlap with the current interval or any earlier interval in that merged group.

Therefore, the current interval can safely be finalized.

This is a **greedy approach** because the algorithm makes the locally correct decision at every step and never needs to revisit a completed interval.

---

## 12. Time Complexity

The algorithm has two main operations.

### Sorting

Sorting `n` intervals takes:

```text
O(n log n)
```

### Scanning

The program scans the sorted array once:

```text
O(n)
```

### Total

Therefore:

```text
O(n log n) + O(n)
```

which simplifies to:

```text
O(n log n)
```

Thus, the program meets the required time complexity.

---

## 13. Space Complexity

The program uses:

```c
Interval *arr
```

to store the input intervals and:

```c
Interval *out
```

to store the merged result.

Both arrays can contain up to `n` intervals.

Therefore, the total additional space is:

```text
O(n)
```

The actual merging logic itself uses only a constant amount of extra working space apart from the output array and sorting implementation.

---

## 14. Input Format

First enter the number of intervals:

```text
n
```

Then enter each interval as:

```text
x y
```

where:

- `x` is the start
- `y` is the end

### Example Input

```text
4
1 3
2 6
8 10
7 18
```

---

## 15. Expected Output

```text
Merged intervals: (1, 6) (7, 18)
```

---

## 16. More Examples

### Example 1

Input:

```text
(1,4)
(4,5)
(10,12)
```

Output:

```text
(1,5)
(10,12)
```

Because `(1,4)` and `(4,5)` touch at `4`, they are merged.

---

### Example 2

Input:

```text
(1,2)
(5,7)
(9,12)
```

Output:

```text
(1,2)
(5,7)
(9,12)
```

No intervals overlap.

---

### Example 3

Input:

```text
(1,10)
(2,3)
(4,8)
(6,15)
```

Output:

```text
(1,15)
```

All intervals belong to one overlapping group.

---

## 17. Important Functions

### `cmp()`

```c
int cmp(const void *a, const void *b)
```

Sorts intervals according to their starting points.

---

### `mergeIntervals()`

```c
int mergeIntervals(Interval arr[], int n, Interval out[])
```

Performs the complete merging algorithm.

It returns the number of merged intervals.

---

### `main()`

The `main()` function:

1. Reads the number of intervals.
2. Dynamically allocates memory.
3. Reads all intervals.
4. Calls `mergeIntervals()`.
5. Prints the merged intervals.
6. Frees allocated memory.

---

## 18. Why Not Compare Every Pair?

A brute-force solution could repeatedly compare every interval with every other interval.

This could require:

```text
O(n²)
```

comparisons.

The sorting + scanning approach improves this to:

```text
O(n log n)
```

because sorting organizes the intervals so that a single scan is enough to merge them.

---

## 19. Key Concepts

This program demonstrates:

- Interval merging
- Sorting
- Greedy algorithms
- `qsort()` in C
- Structures (`struct`)
- Dynamic memory allocation
- Array processing
- Overlap detection
- Time-complexity analysis
- Space-complexity analysis

---

## 20. General Algorithm

The complete algorithm can be summarized as:

```text
Sort intervals by start

current = first interval

For every remaining interval:
    If next.start <= current.end:
        current.end = max(current.end, next.end)
    Else:
        output current
        current = next

Output current
```

---

## 21. Key Observation

The most important condition is:

```text
next.start <= current.end
```

If this is true, the intervals overlap and can be merged.

If it is false:

```text
next.start > current.end
```

the intervals are separate, so the current interval can be finalized.

Sorting by start time makes this simple one-pass decision possible.

---

## 22. Conclusion

This program efficiently merges all overlapping intervals using a **sorting + greedy scanning** approach.

The strategy is:

```text
Sort by starting point
        ↓
Take first interval as current
        ↓
Check the next interval
        ↓
Overlap? → Extend current interval
        ↓
No overlap? → Save current and start a new one
        ↓
Continue until all intervals are processed
```

The final complexity is:

```text
Sorting = O(n log n)
Scanning = O(n)
----------------
Total = O(n log n)
```

Therefore, the program provides an efficient `O(n log n)` solution to the interval-merging problem.