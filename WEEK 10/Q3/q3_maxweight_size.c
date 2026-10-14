/* Q3: Maximum Weight Component with Size Constraint (<= K)
 * DSU stores size and total vertex weight at each root.
 * After every edge insertion, scan the roots and report the largest
 * weight among components with size <= K (0 if none). Weights >= 0.
 * Input : V K, then V vertex weights, then E, then E lines "u v w"
 * Time  : O(E*V) with the simple scan (E alpha(V) for the unions)
 * Space : O(V)
 */
#include <stdio.h>
#include <stdlib.h>

int *p, *sz;
long long *sum;

int find(int x) {
    while (p[x] != x) { p[x] = p[p[x]]; x = p[x]; }
    return x;
}

int main(void) {
    int V, K, E;
    if (scanf("%d %d", &V, &K) != 2) return 1;
    p = malloc((V + 1) * sizeof(int));
    sz = malloc((V + 1) * sizeof(int));
    sum = malloc((V + 1) * sizeof(long long));
    for (int i = 1; i <= V; i++) {
        scanf("%lld", &sum[i]);
        p[i] = i; sz[i] = 1;
    }
    scanf("%d", &E);
    for (int i = 1; i <= E; i++) {
        int u, v, w;
        scanf("%d %d %d", &u, &v, &w);      /* edge weight w is not needed */
        int a = find(u), b = find(v);
        if (a != b) {
            if (sz[a] < sz[b]) { int t = a; a = b; b = t; }
            p[b] = a;
            sz[a] += sz[b];
            sum[a] += sum[b];
        }
        long long best = 0;
        for (int x = 1; x <= V; x++)
            if (p[x] == x && sz[x] <= K && sum[x] > best) best = sum[x];
        printf("After edge %d (%d,%d,%d): max weight = %lld\n", i, u, v, w, best);
    }
    return 0;
}
