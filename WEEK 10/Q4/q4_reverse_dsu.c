/* Q4: Reverse Time-Step DSU
 * Deleting vertices is hard for DSU, so process the deletion order
 * backwards and ADD vertices instead.
 *   res[i] = max component sum after deleting p1..pi
 *          = best value after re-adding p_{i+1}..p_V
 * Weights are assumed >= 0 (so merging never lowers the maximum).
 * Input : V E, V weights, E lines "u v", then permutation p1..pV
 * Output: V lines, answer after each deletion
 * Time  : O((V+E) alpha(V))   Space: O(V+E)
 */
#include <stdio.h>
#include <stdlib.h>

int *p, *head, *nxt, *to;
long long *sum;

int find(int x) {
    while (p[x] != x) { p[x] = p[p[x]]; x = p[x]; }
    return x;
}

int main(void) {
    int V, E;
    if (scanf("%d %d", &V, &E) != 2) return 1;
    p = malloc((V + 1) * sizeof(int));
    head = malloc((V + 1) * sizeof(int));
    nxt = malloc(2 * E * sizeof(int));
    to = malloc(2 * E * sizeof(int));
    sum = malloc((V + 1) * sizeof(long long));
    long long *w = malloc((V + 1) * sizeof(long long));
    char *active = calloc(V + 1, 1);
    int *ord = malloc((V + 1) * sizeof(int));
    long long *res = malloc((V + 1) * sizeof(long long));

    for (int i = 1; i <= V; i++) { scanf("%lld", &w[i]); head[i] = -1; p[i] = i; }
    int cnt = 0;
    for (int i = 0; i < E; i++) {                  /* adjacency list */
        int u, v;
        scanf("%d %d", &u, &v);
        to[cnt] = v; nxt[cnt] = head[u]; head[u] = cnt++;
        to[cnt] = u; nxt[cnt] = head[v]; head[v] = cnt++;
    }
    for (int i = 1; i <= V; i++) scanf("%d", &ord[i]);

    long long best = 0;
    res[V] = 0;                                    /* everything deleted */
    for (int i = V - 1; i >= 1; i--) {
        int x = ord[i + 1];                        /* re-add this vertex */
        active[x] = 1;
        p[x] = x;
        sum[x] = w[x];
        for (int e = head[x]; e != -1; e = nxt[e]) {
            int y = to[e];
            if (!active[y]) continue;
            int a = find(x), b = find(y);
            if (a != b) { p[a] = b; sum[b] += sum[a]; }
        }
        long long s = sum[find(x)];
        if (s > best) best = s;
        res[i] = best;
    }
    for (int i = 1; i <= V; i++)
        printf("After deleting vertex %d: max component sum = %lld\n", ord[i], res[i]);
    return 0;
}
