# Divide-and-Conquer Defective (Lighter) Coin Finder (C)

A C program that finds a single **defective (lighter) coin** among `n` coins using a **divide-and-conquer weighing strategy**, simulating a balance-scale search rather than comparing individual coins one by one.

## Overview

This is a classic "counterfeit coin" problem variant: given `n` coins where at most one coin is lighter than the rest, find it using as few balance-scale weighings as possible. The program:

- Splits the coin range into two (roughly) equal halves, `L` and `R` (with a leftover coin set aside if `n` is odd).
- "Weighs" the sum of `L` against the sum of `R` on a simulated balance scale.
- Recurses into whichever side is lighter, or — if balanced — concludes the defective coin (if any) must be the leftover coin.
- Once a single candidate coin remains, performs one final confirming weighing against another (now-guaranteed-genuine) coin to verify it is actually lighter.

## Files

| File | Description |
|------|--------------|
| `find_lighter_coin.c` | Main source file containing the full implementation |

## How It Works

1. **`weigh(weight, l1, r1, l2, r2)`** — Simulates placing coins `weight[l1..r1]` on one pan and `weight[l2..r2]` on the other. Sums both ranges and returns:
   - `-1` if the left group is lighter
   - `1` if the right group is lighter (i.e., left is heavier)
   - `0` if balanced
   
   Each call increments the global `weighings` counter.

2. **`findLighter(weight, lo, hi)`** — The core recursive routine:
   - **Base case** (`size == 1`): only one coin remains in range; it becomes the sole candidate (not yet confirmed defective).
   - **Recursive case**:
     - Splits `[lo, hi]` into `L = [lo, mid]` and `R = [mid+1, mid+half]`, setting aside a `leftover` coin if the range size is odd.
     - Weighs `L` against `R`.
     - If balanced, both `L` and `R` are genuine, so only the `leftover` coin (if any) could be defective — its index is returned, or `-1` if there is no leftover.
     - If `L` is lighter, recurses into `L`.
     - If `R` is lighter, recurses into `R`.

3. **`main()`**:
   - Reads `n` coin weights into an array.
   - Runs `findLighter()` to narrow down to a single candidate index (or `-1`).
   - Performs a **final confirming weighing**: compares the candidate's weight against one other coin (guaranteed genuine by elimination) to verify it is actually lighter.
   - Prints whether a defective coin was found (with its index and weight), and reports the total number of weighings used versus the theoretical `log2(n)` reference.

## Building

```bash
gcc -O2 -o find_lighter_coin find_lighter_coin.c -lm
```

## Running

```bash
./find_lighter_coin
```

Example session:

```
Enter number of coins: 8
Enter 8 coin weights: 5 5 5 5 5 4.8 5 5
Defective (lighter) coin found at index 5 (weight = 4.8000)
Total weighings used: 4 (n = 8, log2(n) ~= 3.00)
```

## Notes & Limitations

- **Assumes at most one defective coin**: The algorithm's correctness relies on there being zero or one lighter coin among the `n` inputs. If multiple coins are lighter, the weighing logic (which compares group *sums*) can be misled, since a lighter subgroup might contain more than one defective coin or the sums could coincidentally balance.
- **"Weighing" a group is not how real balance scales work**: In practice, a physical balance scale can only compare two groups of *equal size*, and can't literally sum arbitrary-length weight ranges — this is a simulation for algorithmic analysis, using `sum1` vs `sum2`, rather than a true two-pan balance constraint. Real coin-weighing puzzles typically require both pans to hold the same number of coins.
- **Odd-sized ranges**: When `size` is odd, the last coin (`hi`) is set aside as `leftover` and only investigated if `L` and `R` balance. This means the leftover coin is never included in a weighing until the final confirmation step, so it takes a "free ride" through the recursion.
- **`isDefective` loop only checks the first other coin**: The `for` loop in `main()` breaks after a single comparison (`i == candidate ? continue : compare-then-break`), so exactly one confirming weighing is performed — by design, but it means the result depends on comparing against whichever coin happens to be at index `0` (or index `1` if the candidate is at index `0`).
- **No handling for `n <= 0`**: A non-positive `n` will cause undefined behavior (`malloc(0)` or empty input reads).
- **Weighings counter is global**: Not thread-safe / not reentrant if used concurrently.
- **Floating-point weights**: Coin weights are `double`s; exact equality checks (`sum1 == sum2` implicitly via the `if`/`else if`/`else` chain) can be affected by floating-point rounding error, particularly for larger coin counts.

## Complexity

| Operation | Complexity |
|-----------|------------|
| Time (weighings) | O(log n) |
| Time (per weighing, summing ranges) | O(n) in the worst case, making total work O(n log n) |
| Space | O(log n) (recursion stack) |

## License

Free to use and modify for educational or personal purposes.
