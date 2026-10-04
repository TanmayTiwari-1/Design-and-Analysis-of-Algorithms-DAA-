/* Q3: Longest Common Subsequence with reconstruction
   Input : two strings X and Y (no spaces), one per line              */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define MAXN 2000

static int max(int a, int b) { return a > b ? a : b; }

int main(void) {
    static char X[MAXN + 1], Y[MAXN + 1];
    if (scanf("%2000s %2000s", X, Y) != 2) { printf("Need two strings\n"); return 1; }
    int m = strlen(X), n = strlen(Y);
    int *L = calloc((m + 1) * (n + 1), sizeof(int));   /* L[i][j] at L[i*(n+1)+j] */
#define T(i, j) L[(i) * (n + 1) + (j)]

    for (int i = 1; i <= m; i++)
        for (int j = 1; j <= n; j++)
            T(i, j) = (X[i-1] == Y[j-1]) ? T(i-1, j-1) + 1 : max(T(i-1, j), T(i, j-1));

    int len = T(m, n);
    char *res = malloc(len + 1);
    res[len] = '\0';
    int i = m, j = n, k = len - 1;
    while (i > 0 && j > 0) {                 /* traceback */
        if (X[i-1] == Y[j-1]) { res[k--] = X[i-1]; i--; j--; }
        else if (T(i-1, j) >= T(i, j-1)) i--;
        else j--;
    }
    printf("LCS length = %d\nLCS string = %s\n", len, len ? res : "(empty)");
    free(L); free(res);
    return 0;
}
