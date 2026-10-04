# Q2 - Coin Change: total number of ways

## Idea
`ways[v]` = number of combinations that sum to `v`.

    ways[0] = 1
    for each coin c (outer loop):
        for v = c .. V:   ways[v] += ways[v - c]

Coins are the **outer** loop, so a combination is only ever built in one coin order:
1+2 and 2+1 are counted once. (Swapping the loops would count permutations - a common mistake.)

## Input format (stdin)
    n
    c1 c2 ... cn      (distinct, positive)
    V
Example: coins {1,2,5}, V=5 -> **4** ways (5, 2+2+1, 2+1+1+1, 1+1+1+1+1).
Also tested: coins {2,5,3}, V=10 -> 4; coins {2}, V=3 -> 0.

## Complexity
* n coins x (V) amounts, O(1) each: **Time = O(n * V)**, **Space = O(V)**.
* Answers use `unsigned long long`; the count can overflow for very large V.

## Run
    gcc -O2 -o coin_ways coin_ways.c
    ./coin_ways < input.txt
