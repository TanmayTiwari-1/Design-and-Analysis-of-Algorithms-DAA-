# Q4 - Longest Increasing Subsequence (strict)

## Idea
`len[i]` = length of the longest strictly increasing subsequence that **ends at** index i.

    len[i] = 1 + max( len[j] )  over j < i with a[j] < a[i]   (or 1 if none)
    answer = max over i of len[i]

`prev[i]` remembers the best j, so one actual LIS is printed.
Strictness: the test is `a[j] < a[i]`, so equal values (e.g. 2 2 2) give length 1.

## Input format (stdin)
    n
    a0 a1 ... a(n-1)
Example: `10 9 2 5 3 7 101 18` -> length **4** (e.g. 2 5 7 101).

## Complexity
* Two nested loops, i from 0..n-1 and j from 0..i-1: 0+1+...+(n-1) = n(n-1)/2 -> **Time = O(n^2)**.
* **Space = O(n)**.
* (An O(n log n) method exists using binary search on "tails" - not needed here, O(n^2) is the simplest DP.)

## Run
    gcc -O2 -o lis lis.c
    ./lis < input.txt
