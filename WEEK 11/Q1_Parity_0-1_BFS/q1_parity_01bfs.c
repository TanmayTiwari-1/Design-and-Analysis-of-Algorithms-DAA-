
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

typedef long long ll;
#define INF (LLONG_MAX / 4)

int main(void)
{
    int n, m, S;
    ll C;
    if (scanf("%d %d %d %lld", &n, &m, &S, &C) != 4) {
        fprintf(stderr, "error: bad header (expected: n m S C)\n");
        return 1;
    }
    if (n <= 0 || m < 0 || S < 1 || S > n || C < 0) {
        fprintf(stderr, "error: need n>0, m>=0, 1<=S<=n, C>=0\n");
        return 1;
    }

    int *eu = malloc(sizeof(int) * (m > 0 ? m : 1));
    int *ev = malloc(sizeof(int) * (m > 0 ? m : 1));
    int *ew = malloc(sizeof(int) * (m > 0 ? m : 1));
    int *start = calloc(n + 2, sizeof(int));
    if (!eu || !ev || !ew || !start) { fprintf(stderr, "out of memory\n"); return 1; }

    for (int i = 0; i < m; i++) {
        if (scanf("%d %d %d", &eu[i], &ev[i], &ew[i]) != 3) {
            fprintf(stderr, "error: could not read edge %d\n", i + 1);
            return 1;
        }
        if (eu[i] < 1 || eu[i] > n || ev[i] < 1 || ev[i] > n || (ew[i] != 0 && ew[i] != 1)) {
            fprintf(stderr, "error: edge %d invalid (vertices in 1..n, weight in {0,1})\n", i + 1);
            return 1;
        }
        start[eu[i] + 1]++;
    }
    /* CSR build */
    for (int i = 1; i <= n + 1; i++) start[i] += start[i - 1];
    int *to = malloc(sizeof(int) * (m > 0 ? m : 1));
    int *wt = malloc(sizeof(int) * (m > 0 ? m : 1));
    int *fill = malloc(sizeof(int) * (n + 2));
    for (int i = 0; i <= n + 1; i++) fill[i] = start[i];
    for (int i = 0; i < m; i++) {
        int pos = fill[eu[i]]++;
        to[pos] = ev[i];
        wt[pos] = ew[i];
    }

    /* state id = v*2 + parity */
    int NS = 2 * (n + 1);
    ll *dist = malloc(sizeof(ll) * NS);
    char *done = calloc(NS, 1);
    for (int i = 0; i < NS; i++) dist[i] = INF;

    /* Deque as an array: front pushes <= 2m+1, back pushes <= 2m+1. */
    int cap = 2 * (2 * m + 2) + 2;
    int *dq = malloc(sizeof(int) * cap);
    int head = 2 * m + 2, tail = head; /* elements in [head, tail) */

    dist[S * 2 + 0] = 0;
    dq[tail++] = S * 2 + 0;

    while (head < tail) {
        int st = dq[head++];
        if (done[st]) continue;
        done[st] = 1;
        int u = st >> 1, p = st & 1;
        for (int e = start[u]; e < start[u + 1]; e++) {
            int v = to[e], w = wt[e];
            int ns = v * 2 + (p ^ w);
            if (dist[st] + w < dist[ns]) {
                dist[ns] = dist[st] + w;
                if (w == 0) dq[--head] = ns; /* 0-edge -> front */
                else        dq[tail++] = ns; /* 1-edge -> back  */
            }
        }
    }

    printf("Vertex  Cost        Details (best path: W, parity of k1)\n");
    printf("------  ----------  ------------------------------------\n");
    for (int v = 1; v <= n; v++) {
        ll d0 = dist[v * 2], d1 = dist[v * 2 + 1];
        ll c0 = (d0 >= INF) ? INF : d0;
        ll c1 = (d1 >= INF) ? INF : d1 + C;
        if (c0 >= INF && c1 >= INF) {
            printf("%6d  %-10s  no path from S\n", v, "UNREACHABLE");
        } else if (c0 <= c1) {
            printf("%6d  %-10lld  W=%lld, k1 even\n", v, c0, d0);
        } else {
            printf("%6d  %-10lld  W=%lld, k1 odd (+C=%lld)\n", v, c1, d1, C);
        }
    }

    free(eu); free(ev); free(ew); free(start); free(to); free(wt);
    free(fill); free(dist); free(done); free(dq);
    return 0;
}
