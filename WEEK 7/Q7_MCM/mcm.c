/*
 * Q7: Matrix Chain Multiplication (MCM)
 * ----------------------------------------
 * Given the dimensions of a chain of matrices A1 x A2 x ... x An, find
 * the minimum number of scalar multiplications needed to compute the
 * product, and the optimal parenthesization achieving it.
 *
 * INPUT REPRESENTATION
 * ---------------------
 * An array p[0..n] of n+1 integers, where matrix Ai has dimensions
 * p[i-1] x p[i]  (so matrix i is compatible with matrix i+1).
 *
 * ALGORITHM  (Dynamic Programming)
 * -----------------------------------
 *   dp[i][j] = minimum scalar multiplications to compute Ai..Aj
 *   dp[i][i] = 0
 *   dp[i][j] = min over i<=k<j of  dp[i][k] + dp[k+1][j] + p[i-1]*p[k]*p[j]
 * We also store split[i][j] = the k achieving the minimum, to reconstruct
 * the optimal parenthesization.
 *
 * COMPLEXITY
 * ----------
 *   Time  : O(n^3)   (O(n^2) sub-problems, O(n) choices of split point k)
 *   Space : O(n^2)   (dp and split tables)
 */

#include <stdio.h>
#include <stdlib.h>

int split[50][50];

void printParens(int i, int j) {
    if (i == j) { printf("A%d", i); return; }
    printf("(");
    printParens(i, split[i][j]);
    printParens(split[i][j] + 1, j);
    printf(")");
}

int main(int argc, char **argv) {
    /* default example dims from the classic textbook instance */
    int defaultP[] = {30, 35, 15, 5, 10, 20, 25};
    int n;
    int *p;

    if (argc > 1) {
        n = argc - 1;
        p = malloc(sizeof(int) * (n + 1));
        for (int i = 0; i <= n - 1; i++) p[i] = atoi(argv[i + 1]);
        n = n - 1; /* number of matrices = number of dims - 1 */
    } else {
        n = (int)(sizeof(defaultP) / sizeof(int)) - 1;
        p = malloc(sizeof(int) * (n + 1));
        for (int i = 0; i <= n; i++) p[i] = defaultP[i];
    }

    long long dp[50][50];
    for (int i = 1; i <= n; i++) dp[i][i] = 0;

    for (int len = 2; len <= n; len++) {
        for (int i = 1; i <= n - len + 1; i++) {
            int j = i + len - 1;
            dp[i][j] = -1;
            for (int k = i; k < j; k++) {
                long long cost = dp[i][k] + dp[k + 1][j] +
                                  (long long)p[i - 1] * p[k] * p[j];
                if (dp[i][j] == -1 || cost < dp[i][j]) {
                    dp[i][j] = cost;
                    split[i][j] = k;
                }
            }
        }
    }

    printf("=========================================\n");
    printf(" Matrix Chain Multiplication (%d matrices)\n", n);
    printf("=========================================\n");
    printf("Dimensions p[0..%d]: ", n);
    for (int i = 0; i <= n; i++) printf("%d ", p[i]);
    printf("\n\n");

    printf("Matrix Ai dimensions:\n");
    for (int i = 1; i <= n; i++) printf("  A%d : %d x %d\n", i, p[i - 1], p[i]);

    printf("\nMinimum number of scalar multiplications : %lld\n", dp[1][n]);
    printf("Optimal parenthesization                 : ");
    printParens(1, n);
    printf("\n");

    if (n == 6) {
        printf("\n(Classic instance check: dims 30,35,15,5,10,20,25 should\n");
        printf(" give the well-known answer 15125.)\n");
    }

    free(p);
    return 0;
}
