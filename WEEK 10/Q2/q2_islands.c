/* Q2: Dynamic Connected Components in a Grid (Number of Islands II)
 * Cell (r,c) is flattened to id = r*C + c and kept in a DSU.
 * Each new land cell: count++, then every successful union with a
 * land neighbour (4 directions) does count--.
 * Input : R C K, then K lines "r c" (0-indexed)
 * Output: number of islands after every operation
 * Time  : O(R*C + K alpha(R*C))   Space: O(R*C)
 */
#include <stdio.h>
#include <stdlib.h>

int *p;

int find(int x) {                 /* path halving */
    while (p[x] != x) { p[x] = p[p[x]]; x = p[x]; }
    return x;
}

int main(void) {
    int R, C, K;
    if (scanf("%d %d %d", &R, &C, &K) != 3) return 1;
    p = malloc((size_t)R * C * sizeof(int));
    char *land = calloc((size_t)R * C, 1);
    int dr[4] = {-1, 1, 0, 0}, dc[4] = {0, 0, -1, 1};
    int count = 0;

    for (int i = 1; i <= K; i++) {
        int r, c;
        scanf("%d %d", &r, &c);
        int id = r * C + c;
        if (!land[id]) {
            land[id] = 1;
            p[id] = id;
            count++;
            for (int k = 0; k < 4; k++) {
                int nr = r + dr[k], nc = c + dc[k];
                if (nr < 0 || nr >= R || nc < 0 || nc >= C) continue;
                int nid = nr * C + nc;
                if (!land[nid]) continue;
                int a = find(id), b = find(nid);
                if (a != b) { p[a] = b; count--; }
            }
        }
        printf("After op %d (%d,%d): islands = %d\n", i, r, c, count);
    }
    return 0;
}
