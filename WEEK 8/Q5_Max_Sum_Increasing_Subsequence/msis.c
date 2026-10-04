/* Q5: Maximum Sum Increasing Subsequence (strictly increasing), O(n^2) DP
   Input : n, then n positive integers                                  */
#include <stdio.h>
#include <stdlib.h>

int main(void) {
    int n;
    if (scanf("%d", &n) != 1 || n <= 0) { printf("Invalid n\n"); return 1; }
    int *a = malloc(n * sizeof(int)), *prev = malloc(n * sizeof(int));
    long long *sum = malloc(n * sizeof(long long));   /* sum[i] = best sum of an inc. subseq ENDING at i */
    for (int i = 0; i < n; i++)
        if (scanf("%d", &a[i]) != 1 || a[i] <= 0) { printf("Need positive integers\n"); return 1; }

    int best = 0;
    for (int i = 0; i < n; i++) {
        sum[i] = a[i]; prev[i] = -1;
        for (int j = 0; j < i; j++)
            if (a[j] < a[i] && sum[j] + a[i] > sum[i]) { sum[i] = sum[j] + a[i]; prev[i] = j; }
        if (sum[i] > sum[best]) best = i;
    }
    printf("Maximum sum = %lld\nSubsequence (reversed): ", sum[best]);
    for (int i = best; i != -1; i = prev[i]) printf("%d ", a[i]);
    printf("\n");
    free(a); free(prev); free(sum);
    return 0;
}
