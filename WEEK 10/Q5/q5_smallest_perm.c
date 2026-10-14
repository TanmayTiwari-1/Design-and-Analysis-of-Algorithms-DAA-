/* Q5: Lexicographically Smallest Permutation via Allowed Swaps
 * Positions joined by swaps form connected components (DSU). Inside one
 * component any rearrangement is reachable, so put its values in sorted
 * order onto its positions in increasing order.
 * Implementation: sort pairs (root, position) and pairs (root, value);
 * both sorted lists group identically, so the k-th entries match up.
 * Input : n, A[1..n], m, then m lines "u v" (positions 1..n)
 * Output: the smallest permutation
 * Time  : O(n log n + m alpha(n))   Space: O(n)
 */
#include <stdio.h>
#include <stdlib.h>

typedef struct { int a, b; } Pair;
int *p;

int find(int x) {
    while (p[x] != x) { p[x] = p[p[x]]; x = p[x]; }
    return x;
}

int cmp(const void *x, const void *y) {
    const Pair *s = x, *t = y;
    if (s->a != t->a) return s->a < t->a ? -1 : 1;
    if (s->b != t->b) return s->b < t->b ? -1 : 1;
    return 0;
}

int main(void) {
    int n, m;
    if (scanf("%d", &n) != 1) return 1;
    int *A = malloc((n + 1) * sizeof(int));
    p = malloc((n + 1) * sizeof(int));
    for (int i = 1; i <= n; i++) { scanf("%d", &A[i]); p[i] = i; }
    scanf("%d", &m);
    for (int i = 0; i < m; i++) {
        int u, v;
        scanf("%d %d", &u, &v);
        int a = find(u), b = find(v);
        if (a != b) p[a] = b;
    }
    Pair *pos = malloc(n * sizeof(Pair)), *val = malloc(n * sizeof(Pair));
    for (int i = 1; i <= n; i++) {
        int r = find(i);
        pos[i - 1] = (Pair){r, i};
        val[i - 1] = (Pair){r, A[i]};
    }
    qsort(pos, n, sizeof(Pair), cmp);
    qsort(val, n, sizeof(Pair), cmp);
    int *ans = malloc((n + 1) * sizeof(int));
    for (int k = 0; k < n; k++) ans[pos[k].b] = val[k].b;
    for (int i = 1; i <= n; i++) printf("%d%c", ans[i], i == n ? '\n' : ' ');
    return 0;
}
