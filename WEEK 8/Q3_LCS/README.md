# Q3 - Longest Common Subsequence

## Idea
`L[i][j]` = LCS length of the first i chars of X and first j chars of Y.

    L[i][0] = L[0][j] = 0
    if X[i]==Y[j]:  L[i][j] = L[i-1][j-1] + 1
    else:           L[i][j] = max(L[i-1][j], L[i][j-1])

**Reconstruction:** start at (m,n). If characters match, take the char and move diagonally;
otherwise move to the neighbour with the larger value. Chars are collected back-to-front.

## Input format (stdin)
Two strings without spaces, separated by a space/newline (max 2000 chars each).
Example: `AGGTAB` and `GXTXAYB` -> length **4**, string **GTAB**.

## Complexity
* (m+1)(n+1) cells, O(1) each: **Time = O(m*n)**.
* Table storage: **Space = O(m*n)** (traceback needs the full table). Traceback itself is O(m+n).

## Run
    gcc -O2 -o lcs lcs.c
    ./lcs < input.txt
