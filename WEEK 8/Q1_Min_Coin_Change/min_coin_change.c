/* Q1: Minimum Coin Change (bottom-up DP)
   Input : n, then n coin values, then target V
   Output: minimum #coins (or -1) and the coins used              */
#include <stdio.h>
#include <stdlib.h>
#define INF 1000000000

int main(void) {
    int n, V;
    if (scanf("%d", &n) != 1 || n <= 0) { printf("Invalid n\n"); return 1; }
    int *c = malloc(n * sizeof(int));
    for (int i = 0; i < n; i++)
        if (scanf("%d", &c[i]) != 1 || c[i] <= 0) { printf("Coins must be positive integers\n"); return 1; }
    if (scanf("%d", &V) != 1 || V < 0) { printf("Invalid V\n"); return 1; }

    int *dp   = malloc((V + 1) * sizeof(int));  /* dp[v] = min coins for amount v */
    int *last = malloc((V + 1) * sizeof(int));  /* last[v] = coin used to reach v */
    dp[0] = 0; last[0] = -1;
    for (int v = 1; v <= V; v++) {
        dp[v] = INF; last[v] = -1;
        for (int i = 0; i < n; i++)
            if (c[i] <= v && dp[v - c[i]] != INF && dp[v - c[i]] + 1 < dp[v]) {
                dp[v] = dp[v - c[i]] + 1;
                last[v] = c[i];
            }
    }
    if (dp[V] == INF) { printf("-1\n"); }
    else {
        printf("Minimum coins = %d\nCoins used: ", dp[V]);
        for (int v = V; v > 0; v -= last[v]) printf("%d ", last[v]);
        printf("\n");
    }
    free(c); free(dp); free(last);
    return 0;
}
