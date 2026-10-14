/* Q1: Online Graph-Bipartiteness Testing
 * DSU with parity: d[x] = colour difference between x and its parent.
 * Edge (u,v) is legal iff u and v can get DIFFERENT colours.
 * Input : V E, then E lines "u v" (vertices 1..V)
 * Output: ADDED / REJECTED for every arriving edge
 * Time  : O((V+E) alpha(V))   Space: O(V)
 */
#include <stdio.h>
#include <stdlib.h>

int *p, *d, *sz;

int find(int x) {                 /* path compression, keeps parity to root */
    if (p[x] == x) return x;
    int op = p[x];
    int r = find(op);
    d[x] ^= d[op];                /* d[op] is now parity of op to the root */
    p[x] = r;
    return r;
}

int main(void) {
    int V, E;
    if (scanf("%d %d", &V, &E) != 2) return 1;
    p = malloc((V + 1) * sizeof(int));
    d = calloc(V + 1, sizeof(int));
    sz = malloc((V + 1) * sizeof(int));
    for (int i = 1; i <= V; i++) { p[i] = i; sz[i] = 1; }

    int accepted = 0;
    for (int i = 1; i <= E; i++) {
        int u, v;
        scanf("%d %d", &u, &v);
        int ru = find(u), rv = find(v);
        int ok;
        if (ru == rv) {
            ok = (d[u] != d[v]);          /* same colour => odd cycle */
        } else {
            if (sz[ru] < sz[rv]) { int t = ru; ru = rv; rv = t; }
            p[rv] = ru;                   /* attach smaller under larger */
            d[rv] = d[u] ^ d[v] ^ 1;      /* force u,v to differ */
            sz[ru] += sz[rv];
            ok = 1;
        }
        accepted += ok;
        printf("Edge %d (%d,%d): %s\n", i, u, v, ok ? "ADDED" : "REJECTED");
    }
    printf("Accepted %d of %d edges; final graph is bipartite.\n", accepted, E);
    return 0;
}
