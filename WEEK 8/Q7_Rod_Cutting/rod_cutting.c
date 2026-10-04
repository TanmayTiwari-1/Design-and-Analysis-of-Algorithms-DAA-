/* Q7: Rod Cutting with reconstruction
   Input : n, then prices p1..pn (pi = price of a piece of length i)    */
#include <stdio.h>
#include <stdlib.h>

int main(void) {
    int n;
    if (scanf("%d", &n) != 1 || n <= 0) { printf("Invalid n\n"); return 1; }
    int *p = malloc((n + 1) * sizeof(int));
    for (int i = 1; i <= n; i++)
        if (scanf("%d", &p[i]) != 1) { printf("Bad input\n"); return 1; }

    long long *r = malloc((n + 1) * sizeof(long long));  /* r[j] = best revenue for length j */
    int *cut = malloc((n + 1) * sizeof(int));            /* cut[j] = first piece of an optimal cut */
    r[0] = 0;
    for (int j = 1; j <= n; j++) {
        r[j] = -1;
        for (int i = 1; i <= j; i++)
            if (p[i] + r[j - i] > r[j]) { r[j] = p[i] + r[j - i]; cut[j] = i; }
    }
    printf("Maximum revenue = %lld\nPieces: ", r[n]);
    for (int j = n; j > 0; j -= cut[j]) printf("%d ", cut[j]);
    printf("\n");
    free(p); free(r); free(cut);
    return 0;
}
