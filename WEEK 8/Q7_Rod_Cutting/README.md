# Q7 - Rod Cutting with Reconstruction

## Idea
`r[j]` = best revenue for a rod of length j; `cut[j]` = length of the first piece in an optimal cut.

    r[0] = 0
    r[j] = max over i=1..j of ( p[i] + r[j-i] ),   cut[j] = the i that gives the max

**Reconstruction:** j = n; print cut[j]; j -= cut[j]; repeat until j = 0.

## Input format (stdin)
    n
    p1 p2 ... pn
Example (`input.txt`): n=8, prices `1 5 8 9 10 17 17 20` -> revenue **22**, pieces **2 6**.
Also tested: n=4, prices `3 5 6 7` -> 12 with pieces 1 1 1 1. Ties are broken by the smallest first piece.

## Complexity
* Inner loop runs j times for each j = 1..n: 1+2+...+n = n(n+1)/2 -> **Time = O(n^2)**, **Space = O(n)**.

## Run
    gcc -O2 -o rod_cutting rod_cutting.c
    ./rod_cutting < input.txt
