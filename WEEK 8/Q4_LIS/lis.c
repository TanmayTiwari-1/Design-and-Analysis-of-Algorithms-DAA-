/* Q4: Longest (strictly) Increasing Subsequence, O(n^2) DP + reconstruction
   Input : n, then n integers                                          */
#include <stdio.h>
#include <stdlib.h>

int main(void) {
    int n;
    if (scanf("%d", &n) != 1 || n <= 0) { printf("Invalid n\n"); return 1; }
    int *a = malloc(n * sizeof(int)), *len = malloc(n * sizeof(int)), *prev = malloc(n * sizeof(int));
    for (int i = 0; i < n; i++)
        if (scanf("%d", &a[i]) != 1) { printf("Bad input\n"); return 1; }

    int best = 0;                                   /* index where best LIS ends */
    for (int i = 0; i < n; i++) {
        len[i] = 1; prev[i] = -1;                   /* a[i] alone */
        for (int j = 0; j < i; j++)
            if (a[j] < a[i] && len[j] + 1 > len[i]) { len[i] = len[j] + 1; prev[i] = j; }
        if (len[i] > len[best]) best = i;
    }
    printf("LIS length = %d\nOne LIS: ", len[best]);
    int *seq = malloc(len[best] * sizeof(int)), k = len[best];
    for (int i = best; i != -1; i = prev[i]) seq[--k] = a[i];
    for (k = 0; k < len[best]; k++) printf("%d ", seq[k]);
    printf("\n");
    free(a); free(len); free(prev); free(seq);
    return 0;
}
