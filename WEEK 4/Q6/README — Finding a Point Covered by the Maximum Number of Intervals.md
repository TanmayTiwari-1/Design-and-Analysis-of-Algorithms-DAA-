# README — Finding a Point Covered by the Maximum Number of Intervals

## 1. Problem Statement

Given `n` intervals on a number line:

```text
[lᵢ, rᵢ]
```

the task is to find a point `p` that is covered by the **largest possible number of intervals**.

The important condition is:

> **Endpoints are included in the intervals.**

Therefore, if an interval is:

```text
[2, 5]
```

then both `2` and `5` are considered part of the interval.

The required time complexity is:

```text
O(n log n)
```

---

## 2. Main Idea

The problem is solved using the **sweep-line / event technique**.

For every interval `[l, r]`, create two events:

```text
Start event → +1
End event   → -1
```

For example:

```text
Interval: [3, 8]

Events:
(3, +1)
(8, -1)
```

Then:

1. Create `2n` events.
2. Sort all events by coordinate.
3. At the same coordinate, process **start events before end events**.
4. Sweep through the events from left to right.
5. Maintain the number of currently active intervals.
6. Whenever the count reaches a new maximum, record that coordinate.

---

## 3. Why the Tie-Breaking Rule Is Important

The most important detail in this problem is what happens when an interval starts and another interval ends at the **same point**.

Consider:

```text
[1,5]
[5,10]
```

At point:

```text
p = 5
```

both intervals contain the point because endpoints are included.

Therefore, at `5`, the count should be:

```text
2
```

The events are:

```text
(5, start)
(5, end)
```

The program processes the **start first**.

So the count becomes:

```text
Before 5: 1
Start at 5: 2
End at 5:   1
```

Thus, the maximum count of `2` is correctly detected at:

```text
p = 5
```

---

## 4. Event Structure

The program defines:

```c
typedef struct {
    int coord;
    int type;
} Event;
```

Each event contains:

- `coord` — the coordinate where the event occurs.
- `type` — whether it is a start or end event.

The program uses:

```text
type = 0 → START
type = 1 → END
```

The corresponding changes are:

```text
START → +1
END   → -1
```

---

## 5. Creating the Events

For every interval:

```text
[l, r]
```

the program creates:

```c
events[2 * i]     = (Event){l, 0};
events[2 * i + 1] = (Event){r, 1};
```

Therefore, each interval contributes exactly two events.

For `n` intervals:

```text
Number of events = 2n
```

---

## 6. Sorting the Events

The program uses:

```c
qsort(events, 2 * n, sizeof(Event), cmp);
```

The comparison function sorts events using two rules.

### Rule 1 — Sort by Coordinate

Events with smaller coordinates come first.

For example:

```text
(8, start)
(3, start)
(5, end)
```

becomes:

```text
(3, start)
(5, end)
(8, start)
```

### Rule 2 — Start Before End at the Same Coordinate

If two events have the same coordinate:

```c
return e1->type - e2->type;
```

Since:

```text
START = 0
END   = 1
```

the start event comes first.

Therefore:

```text
(5, START)
(5, END)
```

is the required ordering.

---

## 7. Sweep-Line Process

After sorting, the program scans all events from left to right.

The important variables are:

```c
int current = 0;
int maxCount = 0;
int bestPoint = -1;
```

### `current`

Stores the number of intervals currently covering the sweep position.

### `maxCount`

Stores the largest number of simultaneously covering intervals found so far.

### `bestPoint`

Stores the coordinate where the maximum was first reached.

---

## 8. Updating the Current Count

For every event:

### Start Event

If:

```c
events[i].type == 0
```

the program executes:

```c
current++;
```

because a new interval begins.

### End Event

Otherwise:

```c
current--;
```

because an interval ends.

The important point is that start events at the same coordinate are processed before end events.

---

## 9. Detecting the Maximum

The program checks:

```c
if (events[i].type == 0 && current > maxCount)
```

A new maximum is recorded only after processing a **start event**.

If the condition is satisfied:

```c
maxCount = current;
bestPoint = events[i].coord;
```

Therefore, the program records the coordinate at which the maximum number of intervals is first achieved.

---

## 10. Example

Consider:

```text
[1,5]
[2,6]
[5,8]
```

The events are:

```text
Coordinate    Event
1             START
2             START
5             START
5             END
6             END
8             END
```

Because starts are processed before ends at `5`:

```text
Coordinate    Current Count
1             1
2             2
5 START       3  ← maximum
5 END         2
6 END         1
8 END         0
```

Therefore:

```text
Maximum intervals = 3
Best point = 5
```

Indeed:

```text
5 ∈ [1,5]
5 ∈ [2,6]
5 ∈ [5,8]
```

So all three intervals contain `5`.

---

## 11. Another Example

Suppose:

```text
[1,3]
[2,5]
[4,7]
[6,8]
```

Events after sorting:

```text
1 START
2 START
3 END
4 START
5 END
6 START
7 END
8 END
```

The counts become:

```text
1 → 1
2 → 2
3 → 1
4 → 2
5 → 1
6 → 2
7 → 1
8 → 0
```

Therefore:

```text
Maximum count = 2
```

The first point where this maximum is reached is:

```text
p = 2
```

---

## 12. Why Endpoints Must Be Included

The problem explicitly states that endpoints count as being inside the interval.

For:

```text
[2,5]
```

the points:

```text
2
3
4
5
```

are all covered.

This is why the program must process:

```text
START before END
```

when they occur at the same coordinate.

If the END event were processed first, the program could incorrectly conclude that the interval ending at that point is no longer active.

---

## 13. Time Complexity

There are `2n` events.

### Event Creation

Two events are created per interval:

```text
O(n)
```

### Sorting

Sorting `2n` events requires:

```text
O(2n log(2n))
```

which simplifies to:

```text
O(n log n)
```

### Sweep

The program scans all `2n` events once:

```text
O(2n) = O(n)
```

### Total

Therefore:

```text
O(n) + O(n log n) + O(n)
```

gives:

```text
O(n log n)
```

So the algorithm meets the required complexity.

---

## 14. Space Complexity

The program stores `2n` events:

```c
Event *events = malloc(2 * n * sizeof(Event));
```

Therefore:

```text
Space Complexity = O(n)
```

---

## 15. Input Format

First enter the number of intervals:

```text
n
```

Then enter each interval as:

```text
l r
```

where:

- `l` = left/start endpoint
- `r` = right/end endpoint

### Example Input

```text
5
1 5
2 6
5 8
10 12
4 7
```

---

## 16. Expected Output

For the above input, the maximum overlap occurs at:

```text
p = 5
```

The output is:

```text
Point p = 5 lies in the maximum number of intervals: 4
```

At `p = 5`, these intervals contain the point:

```text
[1,5]
[2,6]
[5,8]
[4,7]
```

---

## 17. Important Functions

### `cmp()`

```c
int cmp(const void *a, const void *b)
```

Sorts events by:

1. Coordinate.
2. Event type when coordinates are equal.

Start events are placed before end events.

---

### `main()`

The `main()` function:

1. Reads the number of intervals.
2. Dynamically allocates memory for `2n` events.
3. Reads all intervals.
4. Creates start and end events.
5. Sorts the events.
6. Performs the sweep.
7. Tracks the maximum overlap.
8. Prints the best point and maximum count.
9. Frees allocated memory.

---

## 18. Why This Is Better Than Brute Force

A brute-force approach could examine many points and count how many intervals contain each point.

This could require approximately:

```text
O(n²)
```

or worse, depending on how candidate points are selected.

The sweep-line technique avoids repeatedly checking every interval.

Instead, each interval contributes only two events:

```text
START → +1
END   → -1
```

After sorting the events, the answer can be found with a single linear scan.

Thus:

```text
Brute Force       → O(n²)
Sweep Line        → O(n log n)
```

---

## 19. Difference Between Q4 and Q6

This problem is closely related to the previous maximum-overlap problem.

### Q4

Q4 asks for the time when the maximum number of **people** are simultaneously present.

Each person contributes:

```text
Entry → +1
Exit  → -1
```

### Q6

Q6 asks for a point covered by the maximum number of **intervals**.

Each interval contributes:

```text
Start → +1
End   → -1
```

The major additional detail in Q6 is the tie-breaking rule:

```text
START before END
```

at the same coordinate because interval endpoints are included.

---

## 20. Key Concepts

This program demonstrates:

- Sweep-line algorithm
- Event-based processing
- Interval overlap
- Maximum overlap
- Sorting
- Custom sorting with `qsort()`
- Tie-breaking rules
- Structures (`struct`)
- Dynamic memory allocation
- Endpoint-inclusive intervals
- Time-complexity analysis
- Space-complexity analysis

---

## 21. General Algorithm

The complete algorithm can be summarized as:

```text
For every interval [l, r]:
    Create START event (l, +1)
    Create END event (r, -1)

Sort events:
    First by coordinate
    If coordinates are equal:
        START before END

current = 0
maximum = 0

For every event:
    If START:
        current++
        If current > maximum:
            maximum = current
            bestPoint = coordinate
    Else:
        current--

Output bestPoint and maximum
```

---

## 22. Key Observation

The most important part of the algorithm is the ordering:

```text
Same coordinate:

START
END
```

This ensures that if an interval starts exactly where another interval ends, **both intervals are counted at that coordinate**.

For example:

```text
[1,5]
[5,10]
```

At:

```text
p = 5
```

both intervals contain the point.

Therefore, the maximum overlap is correctly detected as:

```text
2
```

at:

```text
p = 5
```

---

## 23. Conclusion

This program finds a point covered by the maximum number of intervals using the **sweep-line/event technique**.

The strategy is:

```text
Create start/end events
        ↓
Sort by coordinate
        ↓
Put START before END at equal coordinates
        ↓
Sweep from left to right
        ↓
Maintain current overlap count
        ↓
Record the point when a new maximum is reached
```

The final complexity is:

```text
Event creation = O(n)
Sorting        = O(n log n)
Sweep          = O(n)
--------------------------------
Total          = O(n log n)
```

The key difference from a normal sweep-line problem is the **tie-breaking rule**, which ensures that endpoints are correctly treated as belonging to their intervals.