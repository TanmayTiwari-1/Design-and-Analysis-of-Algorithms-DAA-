
#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    int n, m, K, R;
    if (scanf("%d %d %d %d", &n, &m, &K, &R) != 4) {
        fprintf(stderr, "error: bad header (expected: n m K R)\n");
        return 1;
    }
    if (n <= 0 || m < 0 || K < 1 || R < 0) {
        fprintf(stderr, "error: need n>0, m>=0, K>=1, R>=0\n");
        return 1;
    }

    int *hub = malloc(sizeof(int) * (K + 1));
    for (int i = 1; i <= K; i++) {
        if (scanf("%d", &hub[i]) != 1 || hub[i] < 1 || hub[i] > n) {
            fprintf(stderr, "error: hub %d invalid\n", i);
            return 1;
        }
    }

    int *eu = malloc(sizeof(int) * (m > 0 ? m : 1));
    int *ev = malloc(sizeof(int) * (m > 0 ? m : 1));
    int *start = calloc(n + 2, sizeof(int));
    for (int i = 0; i < m; i++) {
        if (scanf("%d %d", &eu[i], &ev[i]) != 2 ||
            eu[i] < 1 || eu[i] > n || ev[i] < 1 || ev[i] > n) {
            fprintf(stderr, "error: edge %d invalid\n", i + 1);
            return 1;
        }
        start[eu[i] + 1]++;
        start[ev[i] + 1]++;
    }
    for (int i = 1; i <= n + 1; i++) start[i] += start[i - 1];
    int *adj = malloc(sizeof(int) * (2 * m > 0 ? 2 * m : 1));
    int *fill = malloc(sizeof(int) * (n + 2));
    for (int i = 0; i <= n + 1; i++) fill[i] = start[i];
    for (int i = 0; i < m; i++) {
        adj[fill[eu[i]]++] = ev[i];
        adj[fill[ev[i]]++] = eu[i];
    }

    int *dist = malloc(sizeof(int) * (n + 1));
    int *owner = malloc(sizeof(int) * (n + 1));
    int *q = malloc(sizeof(int) * (n + 1));
    int qh = 0, qt = 0;
    for (int v = 0; v <= n; v++) { dist[v] = -1; owner[v] = -1; }

    /* hubs inserted in index order; a repeated hub vertex keeps its smallest index */
    for (int i = 1; i <= K; i++) {
        int h = hub[i];
        if (dist[h] == -1) {
            dist[h] = 0;
            owner[h] = i;
            q[qt++] = h;
        }
    }

    while (qh < qt) {
        int u = q[qh++];
        if (dist[u] == R) continue; /* radius reached: do not expand */
        for (int e = start[u]; e < start[u + 1]; e++) {
            int v = adj[e];
            if (dist[v] == -1) {
                dist[v] = dist[u] + 1;
                owner[v] = owner[u];
                q[qt++] = v;
            } else if (dist[v] == dist[u] + 1 && owner[u] < owner[v]) {
                owner[v] = owner[u]; /* same distance, smaller hub index wins */
            }
        }
    }

    printf("Vertex  Owner  Distance\n");
    printf("------  -----  --------\n");
    for (int v = 1; v <= n; v++) {
        if (owner[v] == -1) printf("%6d  %5d  %8s\n", v, -1, "-");
        else                printf("%6d  %5d  %8d\n", v, owner[v], dist[v]);
    }

    free(hub); free(eu); free(ev); free(start); free(adj); free(fill);
    free(dist); free(owner); free(q);
    return 0;
}
