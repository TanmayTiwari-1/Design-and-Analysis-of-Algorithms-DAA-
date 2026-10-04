/* Q8: Optimal Binary Search Tree (CLRS formulation)
   Input : n, then p1..pn, then q0..qn (n+1 values)
   e[i][j] = expected cost of optimal tree on keys ki..kj
   w[i][j] = p_i+..+p_j + q_{i-1}+..+q_j                               */
#include <stdio.h>
#include <stdlib.h>

#define MAXK 50
static double e[MAXK + 2][MAXK + 1], w[MAXK + 2][MAXK + 1];
static int root[MAXK + 2][MAXK + 1];

static void print_tree(int i, int j, int parent, const char *side) {
    if (i > j) {                                  /* empty range -> dummy key d_j */
        printf("  d%d is the %s child of k%d\n", j, side, parent);
        return;
    }
    int r = root[i][j];
    if (parent == 0) printf("  k%d is the root\n", r);
    else             printf("  k%d is the %s child of k%d\n", r, side, parent);
    print_tree(i, r - 1, r, "left");
    print_tree(r + 1, j, r, "right");
}

int main(void) {
    int n;
    double p[MAXK + 1], q[MAXK + 1];
    if (scanf("%d", &n) != 1 || n <= 0 || n > MAXK) { printf("n must be in 1..%d\n", MAXK); return 1; }
    for (int i = 1; i <= n; i++) if (scanf("%lf", &p[i]) != 1) return 1;
    for (int i = 0; i <= n; i++) if (scanf("%lf", &q[i]) != 1) return 1;

    for (int i = 1; i <= n + 1; i++) { e[i][i-1] = q[i-1]; w[i][i-1] = q[i-1]; }  /* empty trees */
    for (int len = 1; len <= n; len++)
        for (int i = 1; i <= n - len + 1; i++) {
            int j = i + len - 1;
            w[i][j] = w[i][j-1] + p[j] + q[j];
            e[i][j] = 1e18;
            for (int r = i; r <= j; r++) {        /* try every key as root */
                double t = e[i][r-1] + e[r+1][j] + w[i][j];
                if (t < e[i][j]) { e[i][j] = t; root[i][j] = r; }
            }
        }
    printf("Minimum expected search cost = %.4f\n\nOptimal tree structure:\n", e[1][n]);
    print_tree(1, n, 0, "");
    return 0;
}
