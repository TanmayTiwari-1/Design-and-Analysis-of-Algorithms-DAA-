/*
 * DAA Lab-09  |  Q3 : Minimum refuelling stops  (+ minimum initial fuel)
 * ---------------------------------------------------------------------------
 * PROBLEM  Car starts at 0 with F units of fuel (1 unit of fuel = 1 unit of
 *          distance), target at D.  Station i is at distance d_i and gives
 *          f_i fuel.  Minimise the number of stops to reach D.
 *
 * GREEDY (lazy / "reverse" decision)
 *   Drive without stopping and only decide LATER where we should have
 *   refuelled: keep a max-heap of every station already passed.  When the
 *   current reach F + (fuel taken so far) is < D, we must have stopped at one
 *   of the passed stations - pick the one with most fuel.
 *     reach = F;  while reach < D:
 *         push all stations with d_i <= reach;
 *         if heap empty -> impossible; else reach += pop_max(); stops++
 *   Exchange argument: any optimal solution that needs k stops to extend the
 *   reach beyond some point can swap a chosen passed station for the largest
 *   passed one without decreasing the reach, so the largest is always safe.
 *
 * BONUS (matches the title "Minimum Initial Fuel"): feasibility is monotone
 *   in F, so binary search on F with the greedy as oracle gives the smallest
 *   initial fuel for which the target is reachable at all.
 *
 * INPUT   D F m
 *         d_1 f_1 ... d_m f_m
 * USAGE   ./q03 < sample_input.txt   |   ./q03 --test
 * COMPLEXITY  sort O(m log m); each station pushed/popped once, O(log m)
 *             => O(m log m) time, O(m) space.  Binary search adds O(log D) factor.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXM 100005

typedef struct { long long d, f; } Station;
static Station st[MAXM];
static int heap_[MAXM], hs;

static int cmp_station(const void *a, const void *b)
{
    const Station *x = a, *y = b;
    return (x->d > y->d) - (x->d < y->d);
}
static int hless(int a, int b) { return st[a].f > st[b].f; }   /* max-heap on fuel */
static void hpush(int x)
{
    int i = hs++;
    heap_[i] = x;
    while (i > 0) {
        int p = (i - 1) / 2;
        if (!hless(heap_[i], heap_[p])) break;
        int t = heap_[i]; heap_[i] = heap_[p]; heap_[p] = t; i = p;
    }
}
static int hpop(void)
{
    int r = heap_[0];
    heap_[0] = heap_[--hs];
    int i = 0;
    for (;;) {
        int l = 2 * i + 1, rr = l + 1, s = i;
        if (l < hs && hless(heap_[l], heap_[s])) s = l;
        if (rr < hs && hless(heap_[rr], heap_[s])) s = rr;
        if (s == i) break;
        int t = heap_[i]; heap_[i] = heap_[s]; heap_[s] = t; i = s;
    }
    return r;
}

/* returns number of stops or -1; used[] receives chosen station indices */
static int min_stops(long long D, long long F, int m, int *used)
{
    long long reach = F;
    int i = 0, stops = 0;
    hs = 0;
    while (reach < D) {
        while (i < m && st[i].d <= reach) hpush(i++);
        if (hs == 0) return -1;
        int b = hpop();
        reach += st[b].f;
        if (used) used[stops] = b;
        stops++;
    }
    return stops;
}

static long long min_initial_fuel(long long D, int m)
{
    long long lo = 0, hi = D;                 /* F = D always works with 0 stops */
    while (lo < hi) {
        long long mid = (lo + hi) / 2;
        if (min_stops(D, mid, m, NULL) >= 0) hi = mid; else lo = mid + 1;
    }
    return lo;
}

/* brute force over all subsets of stations (m <= 14) */
static int brute_stops(long long D, long long F, int m)
{
    int best = -1;
    for (int mask = 0; mask < (1 << m); mask++) {
        int ok = 1;
        long long total = F;
        for (int i = 0; i < m && ok; i++) {
            if (!(mask >> i & 1)) continue;
            long long avail = F;               /* fuel from chosen stations strictly before d_i */
            for (int j = 0; j < m; j++)
                if ((mask >> j & 1) && st[j].d < st[i].d) avail += st[j].f;
            if (avail < st[i].d) ok = 0;
            total += st[i].f;
        }
        if (ok && total >= D) {
            int c = __builtin_popcount(mask);
            if (best < 0 || c < best) best = c;
        }
    }
    return best;
}

static int run_tests(void)
{
    srand(2026);
    int fails = 0, T = 1000;
    for (int t = 1; t <= T; t++) {
        int m = rand() % 11;
        long long D = 1 + rand() % 40, F = rand() % 12;
        for (int i = 0; i < m; i++) { st[i].d = 1 + rand() % D; st[i].f = 1 + rand() % 10; }
        /* keep only stations before the target */
        int k = 0;
        for (int i = 0; i < m; i++) if (st[i].d < D) st[k++] = st[i];
        m = k;
        qsort(st, m, sizeof(Station), cmp_station);
        int g = min_stops(D, F, m, NULL), b = brute_stops(D, F, m);
        long long Fm = min_initial_fuel(D, m);
        int ok = (g == b) && brute_stops(D, Fm, m) >= 0 && (Fm == 0 || brute_stops(D, Fm - 1, m) < 0);
        if (!ok) { fails++; printf("FAIL test %d: D=%lld F=%lld greedy=%d brute=%d minF=%lld\n", t, D, F, g, b, Fm); }
    }
    printf("Random tests : %d (up to 10 stations, brute force over all subsets)\n", T);
    printf("Checks       : min #stops == brute force; minimal initial fuel is feasible,\n");
    printf("               and one unit less is infeasible\n");
    printf("Failures     : %d\n", fails);
    printf("%s\n", fails ? "RESULT: SOME TESTS FAILED" : "RESULT: ALL TESTS PASSED");
    return fails != 0;
}

int main(int argc, char **argv)
{
    if (argc > 1 && !strcmp(argv[1], "--test")) return run_tests();
    long long D, F; int m;
    if (scanf("%lld %lld %d", &D, &F, &m) != 3 || m < 0 || m >= MAXM) { fprintf(stderr, "bad input\n"); return 1; }
    for (int i = 0; i < m; i++)
        if (scanf("%lld %lld", &st[i].d, &st[i].f) != 2) { fprintf(stderr, "bad station\n"); return 1; }
    qsort(st, m, sizeof(Station), cmp_station);
    static int used[MAXM];
    int s = min_stops(D, F, m, used);
    printf("Target D = %lld, initial fuel F = %lld, stations = %d\n", D, F, m);
    if (s < 0) {
        printf("Minimum number of refuelling stops: IMPOSSIBLE (-1)\n");
    } else {
        printf("Minimum number of refuelling stops: %d\n", s);
        long long reach = F;
        for (int i = 0; i < s; i++) {
            reach += st[used[i]].f;
            printf("  stop %d: station at distance %lld (+%lld fuel)  -> reach becomes %lld\n",
                   i + 1, st[used[i]].d, st[used[i]].f, reach);
        }
    }
    printf("Minimum initial fuel needed to reach D at all (any #stops): %lld\n", min_initial_fuel(D, m));
    return 0;
}
