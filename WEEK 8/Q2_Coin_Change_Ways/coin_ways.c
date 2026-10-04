/* Q2: Coin Change - number of combinations (order does not matter)
   Input : n, then n distinct coin values, then target V
   Trick : coins in the OUTER loop => each combination counted once.  */
#include <stdio.h>
#include <stdlib.h>

int main(void) {
    int n, V;
    if (scanf("%d", &n) != 1 || n <= 0) { printf("Invalid n\n"); return 1; }
    int *c = malloc(n * sizeof(int));
    for (int i = 0; i < n; i++)
        if (scanf("%d", &c[i]) != 1 || c[i] <= 0) { printf("Coins must be positive integers\n"); return 1; }
    if (scanf("%d", &V) != 1 || V < 0) { printf("Invalid V\n"); return 1; }

    unsigned long long *ways = calloc(V + 1, sizeof(unsigned long long));
    ways[0] = 1;                         /* one way to make 0: use no coins */
    for (int i = 0; i < n; i++)          /* consider coin i ...             */
        for (int v = c[i]; v <= V; v++)  /* ... for every reachable amount  */
            ways[v] += ways[v - c[i]];
    printf("Number of combinations = %llu\n", ways[V]);
    free(c); free(ways);
    return 0;
}
