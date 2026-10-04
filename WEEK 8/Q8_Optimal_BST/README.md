# Q8 - Optimal Binary Search Tree

## Idea (CLRS notation)
Keys k1..kn have probabilities p1..pn; dummy keys d0..dn have q0..qn (sum of all = 1).

    e[i][j] = expected search cost of the optimal tree on keys ki..kj
    w[i][j] = p_i + ... + p_j + q_(i-1) + ... + q_j

    empty tree:   e[i][i-1] = w[i][i-1] = q_(i-1)
    w[i][j] = w[i][j-1] + p_j + q_j
    e[i][j] = min over r=i..j of ( e[i][r-1] + e[r+1][j] + w[i][j] )     root[i][j] = best r

Fill by increasing range length. Answer = `e[1][n]`. The tree is rebuilt from `root[][]` recursively.

## Input format (stdin)
    n
    p1 ... pn
    q0 ... qn         (n+1 values, n <= 50)
Example (`input.txt`, the classic CLRS example): expected cost **2.75**, root k2.

## Complexity
* O(n^2) table cells, each tries up to n roots: **Time = O(n^3)**, **Space = O(n^2)**.
  (Knuth's optimisation would give O(n^2), not required here.)

## Run
    gcc -O2 -o obst obst.c
    ./obst < input.txt
