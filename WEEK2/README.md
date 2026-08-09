# Merging k Sorted Arrays

## Problem

Given `k` sorted arrays, each containing `n` elements, the goal is to
combine them into one sorted array containing `kn` elements.

Two different approaches are implemented and compared.

---

## Method 1 — Sequential Merging

The arrays are merged one at a time:

A1 + A2 → result  
result + A3 → result  
result + A4 → result  
...  
result + Ak → final array

### Time Complexity

The work performed is approximately:

T(n,k) = 2n + 3n + 4n + ... + kn

Therefore:

**T(n,k) = Θ(nk²)**

---

## Method 2 — Pairwise Merging

The arrays are merged in pairs.

For example:

```text
Level 1:
A1 A2   A3 A4   A5 A6   A7 A8

Level 2:
  A1A2     A3A4     A5A6     A7A8

Level 3:
      A1A2A3A4       A5A6A7A8

Final:
          A1...A8

At every level, a total of kn elements are processed.

There are log₂(k) levels.

### Time Complexity

T(n,k) = Θ(nk log k)
