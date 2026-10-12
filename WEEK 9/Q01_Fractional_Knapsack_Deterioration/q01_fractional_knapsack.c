/*
 * DAA Lab-09  |  Q1 : Fractional Knapsack with Deterioration Rate
 * ---------------------------------------------------------------------------
 * MODEL (made precise)
 *   n items, base value v_i, weight w_i, decay rate lam_i > 0, base density
 *   d_i = v_i / w_i.  Items are consumed one after another at a rate of one
 *   weight-unit per time-unit, so the clock when a unit is taken equals the
 *   total weight consumed so far.  A unit of item i taken at time t is worth
 *   d_i - lam_i * t.  The knapsack capacity W bounds the total weight (= time).
 *   Fractions of items are allowed.
 *
 * KEY FACTS
 *   (1) ORDER.  For fixed amounts x_i the total value is
 *           sum_i x_i d_i  -  integral of  t * lam(t) dt ,
 *       where lam(t) is the decay rate of the item consumed at time t.  The
 *       first term does not depend on the order; the second is minimised by
 *       pairing small t with large lam (rearrangement inequality / adjacent
 *       exchange: swapping i,j changes value by (lam_i-lam_j)*(t2-t1) >= 0).
 *       => consume items in DESCENDING decay rate, each as one contiguous block.
 *   (2) AMOUNTS.  With items sorted by lam descending, let P_j = x_1+...+x_j,
 *       delta_j = lam_j - lam_{j+1} >= 0 and a_j = d_j - d_{j+1}
 *       (lam_{n+1} = d_{n+1} = 0).  Abel summation gives
 *           value = sum_j [ a_j P_j - delta_j P_j^2 / 2 ]      (concave, separable in P)
 *       subject to 0 <= P_j - P_{j-1} <= w_j and P_n <= W.
 *       This chain problem is solved exactly by DP over concave functions:
 *           F_j(p) = f_j(p) + max_{p-w_j <= q <= p} F_{j-1}(q).
 *       F_j' is piecewise linear and non-increasing; the sliding-window max
 *       just inserts a zero-slope piece of length w_j at the arg-max of F_{j-1},
 *       shifting the right part by w_j.  Adding f_j' adds a linear function.
 *       Each stage adds <= 2 pieces, so the DP is O(n^2); the optimum is
 *       recovered by back-tracking the arg-max points.
 *
 * INPUT  (stdin)           n  W
 *                          v_1 w_1 lam_1
 *                          ...
 *                          v_n w_n lam_n
 * USAGE  ./q01 < sample_input.txt        solve one instance
 *        ./q01 --test                    random validation vs brute force
 * COMPLEXITY  sort O(n log n) + DP O(n^2)  => O(n^2) time, O(n) space.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define MAXN 512
#define EPS  1e-12
#define GRID 0.1

typedef struct { int id; double v, w, lam, d; } Item;
typedef struct { double s, e, v0, sl; } Seg;   /* g(p) = v0 + sl*(p-s) on [s,e] */

static int cmp_item(const void *a, const void *b)
{
    const Item *x = a, *y = b;
    if (x->lam != y->lam) return (x->lam < y->lam) ? 1 : -1;   /* lam descending */
    if (x->d != y->d)     return (x->d   < y->d)   ? 1 : -1;
    return x->id - y->id;
}

/* arg-max of the concave function whose derivative is the piece list g */
static double argmax_pt(const Seg *g, int ns)
{
    double m = 0.0;
    for (int k = 0; k < ns; k++) {
        double v0 = g[k].v0;
        double v1 = v0 + g[k].sl * (g[k].e - g[k].s);
        if (v0 <= EPS) return m;
        if (v1 > EPS) m = g[k].e;
        else return g[k].s + v0 / (-g[k].sl);
    }
    return m;
}

/* it[] sorted by lam desc.  Fills x[] (amount taken of it[i]). */
static void solve(const Item *it, int n, double W, double *x)
{
    static Seg A[2 * MAXN + 8], B[2 * MAXN + 8];
    static double mprev[MAXN + 1];
    Seg *cur = A, *nxt = B;
    int ns = 0;

    for (int j = 1; j <= n; j++) {
        double w  = it[j - 1].w;
        double aj = it[j - 1].d   - (j < n ? it[j].d   : 0.0);
        double dj = it[j - 1].lam - (j < n ? it[j].lam : 0.0);
        double m  = argmax_pt(cur, ns);
        mprev[j - 1] = m;
        int nn = 0;
        for (int k = 0; k < ns; k++) {                 /* part left of m   */
            if (cur[k].s >= m) break;
            Seg s = cur[k];
            if (s.e > m) s.e = m;
            if (s.e - s.s > 0) nxt[nn++] = s;
        }
        if (w > 0) nxt[nn++] = (Seg){ m, m + w, 0.0, 0.0 };   /* inserted flat piece */
        for (int k = 0; k < ns; k++) {                 /* part right of m, shifted by w */
            if (cur[k].e <= m) continue;
            Seg s = cur[k];
            double start = s.s, end = s.e;
            if (start < m) { s.v0 += s.sl * (m - start); start = m; }
            s.s = start + w;
            s.e = end + w;
            nxt[nn++] = s;
        }
        for (int k = 0; k < nn; k++) {                 /* add f_j'(p) = a_j - delta_j p */
            nxt[k].v0 += aj - dj * nxt[k].s;
            nxt[k].sl -= dj;
        }
        Seg *t = cur; cur = nxt; nxt = t; ns = nn;
    }
    double p = argmax_pt(cur, ns);
    if (p > W) p = W;
    for (int j = n; j >= 1; j--) {                     /* back-track */
        double lo = p - it[j - 1].w, q = mprev[j - 1];
        if (q < lo) q = lo;
        if (q > p)  q = p;
        x[j - 1] = p - q;
        p = q;
    }
}

static double eval_schedule(const Item *it, const double *x, int n)
{
    double T = 0, val = 0;
    for (int i = 0; i < n; i++) {
        val += x[i] * (it[i].d - it[i].lam * T) - 0.5 * it[i].lam * x[i] * x[i];
        T += x[i];
    }
    return val;
}

/* ---- brute force: every order, amounts on a grid of step GRID ---------- */
static double G_best;
static void bf(const Item *it, int n, int used, double T, double val, double W)
{
    if (val > G_best) G_best = val;
    for (int i = 0; i < n; i++) {
        if (used & (1 << i)) continue;
        for (int k = 0;; k++) {
            double a = k * GRID;
            if (a > it[i].w + 1e-9 || T + a > W + 1e-9) break;
            if (a > it[i].w) a = it[i].w;
            bf(it, n, used | (1 << i), T + a,
               val + a * (it[i].d - it[i].lam * T) - 0.5 * it[i].lam * a * a, W);
        }
    }
}

static double run_case(Item *items, int n, double W, int verbose, int *feasible)
{
    Item it[MAXN];
    double x[MAXN];
    memcpy(it, items, n * sizeof(Item));
    qsort(it, n, sizeof(Item), cmp_item);
    solve(it, n, W, x);
    double val = eval_schedule(it, x, n), tot = 0;
    int ok = 1;
    for (int i = 0; i < n; i++) {
        tot += x[i];
        if (x[i] < -1e-9 || x[i] > it[i].w + 1e-9) ok = 0;
    }
    if (tot > W + 1e-9) ok = 0;
    if (feasible) *feasible = ok;
    if (verbose) {
        printf("Consumption order (decay rate descending):\n");
        printf(" pos item     v      w    lam      d |   take  fraction   start     end      gain\n");
        double T = 0;
        for (int i = 0; i < n; i++) {
            double gain = x[i] * (it[i].d - it[i].lam * T) - 0.5 * it[i].lam * x[i] * x[i];
            printf(" %3d %4d %5.1f %6.1f %6.2f %6.2f | %6.3f  %7.3f %7.3f %7.3f %9.3f\n",
                   i + 1, it[i].id + 1, it[i].v, it[i].w, it[i].lam, it[i].d,
                   x[i], it[i].w > 0 ? x[i] / it[i].w : 0.0, T, T + x[i], gain);
            T += x[i];
        }
        printf("Total weight used = %.3f / %.3f\n", tot, W);
        printf("Maximum total value = %.4f\n", val);
    }
    return val;
}

static int run_tests(void)
{
    srand(2026);
    int fails = 0, T = 300;
    double maxLoss = 0, maxExcess = 0;
    for (int t = 1; t <= T; t++) {
        int n = 1 + rand() % 3;
        Item it[4];
        for (int i = 0; i < n; i++) {
            it[i].id = i;
            it[i].w = 1 + rand() % 4;
            it[i].d = 1 + rand() % 9;
            it[i].v = it[i].d * it[i].w;
            it[i].lam = 0.1 * (1 + rand() % 20);
        }
        double W = 1 + rand() % 8;
        int feas;
        double alg = run_case(it, n, W, 0, &feas);
        G_best = 0;
        bf(it, n, 0, 0.0, 0.0, W);
        double gap = G_best - alg;                 /* brute (grid) cannot beat the exact optimum */
        if (gap > maxExcess) maxExcess = gap;
        if (-gap > maxLoss) maxLoss = -gap;
        if (!feas || gap > 1e-9 || -gap > 0.1) {
            fails++;
            printf("FAIL test %d: n=%d W=%.1f alg=%.4f brute=%.4f\n", t, n, W, alg, G_best);
        }
    }
    printf("Random tests            : %d\n", T);
    printf("Failures                : %d\n", fails);
    printf("Max (brute - algorithm) : %.2e   (must be <= 0: algorithm never worse)\n", maxExcess);
    printf("Max (algorithm - brute) : %.4f   (grid-resolution loss of brute force, < 0.1)\n", maxLoss);
    printf("%s\n", fails ? "RESULT: SOME TESTS FAILED" : "RESULT: ALL TESTS PASSED");
    return fails != 0;
}

int main(int argc, char **argv)
{
    if (argc > 1 && !strcmp(argv[1], "--test")) return run_tests();
    int n; double W;
    if (scanf("%d %lf", &n, &W) != 2 || n < 1 || n > MAXN) { fprintf(stderr, "bad input\n"); return 1; }
    Item it[MAXN];
    for (int i = 0; i < n; i++) {
        it[i].id = i;
        if (scanf("%lf %lf %lf", &it[i].v, &it[i].w, &it[i].lam) != 3 || it[i].w <= 0) {
            fprintf(stderr, "bad item %d\n", i + 1); return 1;
        }
        it[i].d = it[i].v / it[i].w;
    }
    printf("n = %d, capacity W = %.2f\n", n, W);
    run_case(it, n, W, 1, NULL);
    return 0;
}
