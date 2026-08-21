# README — Maximum Simultaneous People Using Sweep Line

## 1. Problem Statement

Given `n` people, where person `i` is present during the time interval:

```text
[aᵢ, bᵢ]
```

the task is to find:

1. The **maximum number of people present simultaneously**.
2. The **first time** when this maximum number is reached.

The required time complexity is:

```text
O(n log n)
```

---

## 2. Main Idea

This problem is solved using the **event / sweep-line technique**.

For every person, we create two events:

- **Entry event** at `aᵢ` → `+1`
- **Exit event** at `bᵢ` → `-1`

For `n` people, this produces:

```text
2n events
```

Then:

1. Sort all events by time.
2. Sweep through the events from left to right.
3. Maintain a variable `current` representing the number of people currently present.
4. Add `+1` when someone enters.
5. Add `-1` when someone exits.
6. Whenever `current` becomes greater than the previous maximum, update the maximum and record that time.

---

## 3. Example

Suppose there are three people:

```text
Person 1: [1, 5]
Person 2: [2, 6]
Person 3: [3, 4]
```

The events are:

```text
Time    Event
1       +1
2       +1
3       +1
4       -1
5       -1
6       -1
```

Sweeping through them:

```text
Time    Change    Current People
1       +1        1
2       +1        2
3       +1        3
4       -1        2
5       -1        1
6       -1        0
```

The maximum number of simultaneously present people is:

```text
3
```

and it is first reached at:

```text
time = 3
```

---

## 4. Event Representation

The program uses the following structure:

```c
typedef struct {
    int time;
    int delta;
} Event;
```

Each event contains:

- `time` — when the event occurs.
- `delta` — how the current number of people changes.

The values of `delta` are:

```text
+1 → person enters
-1 → person exits
```

For example:

```text
(10, +1)
```

means someone enters at time `10`.

```text
(15, -1)
```

means someone exits at time `15`.

---

## 5. Creating the Events

For every person with:

```text
entry = a
exit  = b
```

the program creates:

```c
events[2 * i]     = (Event){a, +1};
events[2 * i + 1] = (Event){b, -1};
```

Therefore, each person contributes exactly two events.

For `n` people:

```text
Number of events = 2n
```

---

## 6. Sorting the Events

The events are sorted using:

```c
qsort(events, 2 * n, sizeof(Event), cmp);
```

The comparison function:

```c
int cmp(const void *a, const void *b)
```

sorts events according to their `time`.

After sorting, the events are arranged chronologically.

For example:

```text
Before sorting:
(5, -1)
(1, +1)
(8, -1)
(3, +1)

After sorting:
(1, +1)
(3, +1)
(5, -1)
(8, -1)
```

---

## 7. Sweep-Line Technique

After sorting, the program scans the events from beginning to end.

The important variables are:

```c
int current = 0;
int maxCount = 0;
int bestTime = -1;
```

### `current`

Stores the number of people currently present.

### `maxCount`

Stores the highest number of simultaneously present people found so far.

### `bestTime`

Stores the first time at which `maxCount` was reached.

---

## 8. Updating the Current Count

For every event:

```c
current += events[i].delta;
```

If the event is an entry:

```text
current = current + 1
```

If the event is an exit:

```text
current = current - 1
```

After updating `current`, the program checks:

```c
if (current > maxCount)
```

If this condition is true:

```c
maxCount = current;
bestTime = events[i].time;
```

This records the new maximum and the time when it was first reached.

---

## 9. Why Do We Store the First Time?

The condition used is:

```c
if (current > maxCount)
```

rather than:

```c
if (current >= maxCount)
```

Therefore, `bestTime` is updated only when a **new maximum** is found.

If the same maximum occurs again later, the original time is preserved.

For example:

```text
Time    Current
2       3  ← first maximum
5       3
8       3
```

The answer remains:

```text
Maximum = 3
First reached at = 2
```

---

## 10. Why the Algorithm Works

At any point in time, the number of people currently present can be determined by the cumulative sum of all entry and exit events up to that point.

For example:

```text
+1 +1 +1 -1 +1 -1
```

gives:

```text
1 → 2 → 3 → 2 → 3 → 2
```

The largest cumulative value represents the maximum number of people present simultaneously.

Therefore, after sorting the events chronologically, a single left-to-right scan is sufficient.

---

## 11. Time Complexity

There are `2n` events.

### Creating Events

Two events are created for every person:

```text
O(n)
```

### Sorting

Sorting `2n` events takes:

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

Thus, the algorithm satisfies the required complexity.

---

## 12. Space Complexity

The program stores `2n` events:

```c
Event *events = malloc(2 * n * sizeof(Event));
```

Therefore:

```text
Space Complexity = O(n)
```

The sorting and sweep do not require another data structure proportional to `n`.

---

## 13. Input Format

First enter the number of people:

```text
n
```

Then enter `n` pairs:

```text
entry exit
```

Each pair represents the interval during which one person is present.

### Example Input

```text
5
1 5
2 7
3 6
4 8
9 12
```

---

## 14. Example Output

For the above input, the output will be:

```text
Maximum simultaneous people present = 4, first reached at time = 4
```

At time `4`, the first four people are simultaneously present.

---

## 15. Important Assumption

The problem states that **all times are distinct**.

Therefore, no two entry or exit events occur at exactly the same time.

Because of this, the program does not need any special rule for deciding whether an entry should be processed before an exit when both occur at the same time.

If equal times were allowed, the event ordering would need to be defined carefully depending on whether intervals are interpreted as closed `[aᵢ, bᵢ]`, open, or half-open intervals.

---

## 16. Important Functions

### `cmp()`

```c
int cmp(const void *a, const void *b)
```

Compares two `Event` structures based on their time.

It is passed to `qsort()` to arrange events chronologically.

---

### `main()`

The `main()` function:

1. Reads `n`.
2. Allocates memory for `2n` events.
3. Reads each person's entry and exit time.
4. Creates the corresponding `+1` and `-1` events.
5. Sorts all events.
6. Performs the sweep-line scan.
7. Tracks the maximum simultaneous count.
8. Prints the maximum and first time it occurs.
9. Frees the allocated memory.

---

## 17. Why Not Check Every Time Interval Directly?

A simple brute-force approach might compare every person's interval with every other person's interval or examine every possible time.

Such approaches can require:

```text
O(n²)
```

or worse.

The event-based approach avoids repeatedly checking all people.

Instead, every person's interval is represented by only two events:

```text
Entry → +1
Exit  → -1
```

After sorting the events, one scan gives the answer efficiently.

---

## 18. Key Concepts

This program demonstrates:

- Sweep-line algorithm
- Event-based processing
- Sorting
- `qsort()` in C
- Structures (`struct`)
- Dynamic memory allocation
- Prefix/cumulative counting
- Interval processing
- Finding maximum overlap
- Time-complexity analysis
- Space-complexity analysis

---

## 19. General Pattern

The sweep-line method can be summarized as:

```text
For every interval:
    Create an entry event (+1)
    Create an exit event (-1)

Sort all events by time

current = 0
maximum = 0

For each event in sorted order:
    current += event.delta

    if current > maximum:
        maximum = current
        record current time
```

This pattern is useful for many interval problems involving:

- Maximum number of people present
- Maximum number of active processes
- Maximum number of overlapping meetings
- Maximum simultaneous network connections
- Maximum overlapping reservations

---

## 20. Conclusion

The program efficiently finds the maximum number of people simultaneously present using the **event / sweep-line technique**.

The key idea is to convert every interval into two events:

```text
Entry → +1
Exit  → -1
```

After sorting the `2n` events, a single sweep maintains the number of currently active people and records the first time the maximum is reached.

The final complexity is:

```text
Event creation = O(n)
Sorting        = O(n log n)
Sweep          = O(n)
--------------------------------
Total          = O(n log n)
```

The algorithm is therefore an efficient `O(n log n)` solution to the maximum-overlap interval problem.