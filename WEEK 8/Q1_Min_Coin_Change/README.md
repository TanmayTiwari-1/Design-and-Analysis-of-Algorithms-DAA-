# Q1 - Minimum Coin Change

## Idea
`dp[v]` = minimum coins needed to make amount `v`.

    dp[0] = 0
    dp[v] = 1 + min( dp[v - c_i] )   over all coins c_i <= v with dp[v - c_i] reachable
    answer = dp[V]  (or -1 if dp[V] is still "infinity")

`last[v]` stores the coin used, so the actual coins are printed too.

## Input format (stdin)
    n
    c1 c2 ... cn
    V
Example (`input.txt`): `3 / 1 2 5 / 11` -> **3 coins (5+5+1)**.
Edge cases tested: coins {2,5}, V=3 -> `-1`; V=0 -> 0 coins.

## Complexity
* Table has V+1 entries; each entry tries n coins, each try is O(1).
* **Time = O(n * V)**, **Space = O(V)**.
(Note: V is the *value*, so this is pseudo-polynomial.)

## Run
    gcc -O2 -o min_coin_change min_coin_change.c
    ./min_coin_change < input.txt
