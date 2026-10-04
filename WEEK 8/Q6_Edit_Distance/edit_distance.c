/* Q6: Edit Distance (Levenshtein) with traceback
   Input : strings A and B (no spaces), one per line
   Ops   : insert, delete, substitute (each cost 1)                     */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define MAXN 2000

static int min3(int a, int b, int c) { int m = a < b ? a : b; return m < c ? m : c; }

int main(void) {
    static char A[MAXN + 1], B[MAXN + 1];
    if (scanf("%2000s %2000s", A, B) != 2) { printf("Need two strings\n"); return 1; }
    int m = strlen(A), n = strlen(B);
    int *D = malloc((m + 1) * (n + 1) * sizeof(int));
#define T(i, j) D[(i) * (n + 1) + (j)]

    for (int i = 0; i <= m; i++) T(i, 0) = i;        /* delete all i chars */
    for (int j = 0; j <= n; j++) T(0, j) = j;        /* insert all j chars */
    for (int i = 1; i <= m; i++)
        for (int j = 1; j <= n; j++)
            T(i, j) = (A[i-1] == B[j-1]) ? T(i-1, j-1)
                    : 1 + min3(T(i-1, j-1), T(i-1, j), T(i, j-1));  /* sub, del, ins */

    printf("Edit distance = %d\n\nTraceback (A -> B):\n", T(m, n));

    /* traceback from (m,n) to (0,0); ops are collected in reverse order */
    char *op = malloc(m + n + 1);   /* M, S, D, I */
    int  *ai = malloc((m + n + 1) * sizeof(int)), *bj = malloc((m + n + 1) * sizeof(int));
    int k = 0, i = m, j = n;
    while (i > 0 || j > 0) {
        if (i > 0 && j > 0 && A[i-1] == B[j-1] && T(i, j) == T(i-1, j-1)) { op[k] = 'M'; ai[k] = i; bj[k] = j; i--; j--; }
        else if (i > 0 && j > 0 && T(i, j) == T(i-1, j-1) + 1)            { op[k] = 'S'; ai[k] = i; bj[k] = j; i--; j--; }
        else if (i > 0 && T(i, j) == T(i-1, j) + 1)                        { op[k] = 'D'; ai[k] = i; bj[k] = j; i--; }
        else                                                                { op[k] = 'I'; ai[k] = i; bj[k] = j; j--; }
        k++;
    }
    int step = 0;
    for (int t = k - 1; t >= 0; t--) {
        char a = ai[t] > 0 ? A[ai[t]-1] : ' ', b = bj[t] > 0 ? B[bj[t]-1] : ' ';
        if (op[t] == 'M') printf("  Match       '%c'\n", a);
        if (op[t] == 'S') printf("  Step %d: Substitute '%c' -> '%c'  (A[%d])\n", ++step, a, b, ai[t]);
        if (op[t] == 'D') printf("  Step %d: Delete     '%c'        (A[%d])\n", ++step, a, ai[t]);
        if (op[t] == 'I') printf("  Step %d: Insert     '%c'        (after A[%d])\n", ++step, b, ai[t]);
    }
    /* aligned view */
    printf("\nAlignment:\n  A: ");
    for (int t = k - 1; t >= 0; t--) putchar(op[t] == 'I' ? '-' : A[ai[t]-1]);
    printf("\n  B: ");
    for (int t = k - 1; t >= 0; t--) putchar(op[t] == 'D' ? '-' : B[bj[t]-1]);
    printf("\n");
    free(D); free(op); free(ai); free(bj);
    return 0;
}
