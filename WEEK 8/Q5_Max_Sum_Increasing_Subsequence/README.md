# Q5 - Maximum Sum Increasing Subsequence

## Idea
Same as LIS but we add **values** instead of counting elements.
`sum[i]` = maximum sum of a strictly increasing subsequence ending at index i.

    sum[i] = a[i] + max( sum[j] )  over j < i with a[j] < a[i]   (or just a[i] if none)
    answer = max over i of sum[i]

Note: the answer is NOT always the longest increasing subsequence.

## Input format (stdin)
    n
    a0 ... a(n-1)     (positive integers)
Example: `1 101 2 3 100 4 5` -> **106** (1+2+3+100). The program prints the subsequence in reverse order.

## Complexity
* Nested loops: n(n-1)/2 comparisons -> **Time = O(n^2)**, **Space = O(n)**.
* Sums are stored in `long long`.

## Run
    gcc -O2 -o msis msis.c
    ./msis < input.txt
