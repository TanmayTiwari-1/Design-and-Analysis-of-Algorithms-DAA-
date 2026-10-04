# Q6 - Edit Distance with Traceback

## Idea
`D[i][j]` = minimum operations to turn the first i chars of A into the first j chars of B.

    D[i][0] = i          (delete i chars)
    D[0][j] = j          (insert j chars)
    if A[i]==B[j]:  D[i][j] = D[i-1][j-1]
    else:           D[i][j] = 1 + min( D[i-1][j-1]   substitute,
                                       D[i-1][j]     delete A[i],
                                       D[i][j-1] )   insert B[j]

**Traceback:** from (m,n) go back to (0,0), choosing the move that explains the cell's value
(match / substitute / delete / insert). Operations are collected in reverse and printed in forward
order together with an alignment (`-` = gap).

## Input format (stdin)
Two strings without spaces (max 2000 chars). Example `kitten` -> `sitting` = **3**:
substitute k->s, substitute e->i, insert g. Also tested: `intention`->`execution` = 5, identical strings = 0.

## Complexity
* **Time = O(m*n)** to fill the table, **Space = O(m*n)** (full table needed for traceback), traceback O(m+n).

## Run
    gcc -O2 -o edit_distance edit_distance.c
    ./edit_distance < input.txt
