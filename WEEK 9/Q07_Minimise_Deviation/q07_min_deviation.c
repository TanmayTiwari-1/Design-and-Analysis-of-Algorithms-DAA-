/*
 * DAA Lab-09  |  Q7 : Minimise deviation in an array (two-way greedy + max-heap)
 * ---------------------------------------------------------------------------
 * Operations: odd x -> 2x (once, result is even) ; even x -> x/2.
 * Every element can therefore reach the values  o*2^j  (o = odd part), and an
 * odd element can only go up once.
 *
 * TRICK  Make the problem one-directional: double every odd element first.
 * Now each element is even or the maximum it could ever be, and from here
 * only "divide even element by 2" is needed.  Only the CURRENT MAXIMUM can
 * improve the deviation (lowering any other element cannot reduce max-min),
 * so repeatedly halve the max (max-heap) while it is even, tracking the
 * running minimum, and remember the best  max - min  seen.  When the max is
 * odd it cannot be reduced any further and we stop.
 *
 * INPUT   n
 *         a_1 ... a_n      (positive integers)
 * USAGE   ./q07 < sample_input.txt   |   ./q07 --test
 * COMPLEXITY  Each element is halved at most log2(max a) times, so there are
 *             O(n log M) heap operations of cost O(log n)
 *             => O(n log n log M) time, O(n) space.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef long long ll;
static ll *hp;
static int hn;

static void hpush(ll x)
{
    int i = hn++;
    hp[i] = x;
    while (i > 0) {
        int p = (i - 1) / 2;
        if (hp[p] >= hp[i]) break;
        ll t = hp[p]; hp[p] = hp[i]; hp[i] = t; i = p;
    }
}
static ll hpop(void)
{
    ll r = hp[0];
    hp[0] = hp[--hn];
    int i = 0;
    for (;;) {
        int l = 2 * i + 1, rr = l + 1, s = i;
        if (l < hn && hp[l] > hp[s]) s = l;
        if (rr < hn && hp[rr] > hp[s]) s = rr;
        if (s == i) break;
        ll t = hp[s]; hp[s] = hp[i]; hp[i] = t; i = s;
    }
    return r;
}

static ll min_deviation(const ll *a, int n, int verbose)
{
    hp = malloc((n + 2) * sizeof(ll));
    hn = 0;
    ll mn = -1;
    for (int i = 0; i < n; i++) {
        ll v = (a[i] & 1) ? a[i] * 2 : a[i];
        hpush(v);
        if (mn < 0 || v < mn) mn = v;
    }
    ll best = hp[0] - mn;
    if (verbose) printf("  start (odds doubled): max=%lld min=%lld deviation=%lld\n", hp[0], mn, best);
    while (hp[0] % 2 == 0) {
        ll top = hpop(), v = top / 2;
        hpush(v);
        if (v < mn) mn = v;
        if (hp[0] - mn < best) best = hp[0] - mn;
        if (verbose) printf("  halve %lld -> %lld: max=%lld min=%lld deviation=%lld (best %lld)\n", top, v, hp[0], mn, hp[0] - mn, best);
    }
    free(hp);
    return best;
}

/* exhaustive: every element picks one of its reachable values */
static ll G_best;
static ll cand[8][64];
static int ncand[8];
static void rec(int i, int n, ll mx, ll mn)
{
    if (i == n) { if (mx - mn < G_best) G_best = mx - mn; return; }
    for (int k = 0; k < ncand[i]; k++) {
        ll v = cand[i][k];
        rec(i + 1, n, i == 0 || v > mx ? v : mx, i == 0 || v < mn ? v : mn);
    }
}
static ll brute(const ll *a, int n)
{
    for (int i = 0; i < n; i++) {
        ll x = a[i];
        ncand[i] = 0;
        while (x % 2 == 0) { cand[i][ncand[i]++] = x; x /= 2; }
        cand[i][ncand[i]++] = x;
        cand[i][ncand[i]++] = 2 * x;
    }
    G_best = 1LL << 60;
    rec(0, n, 0, 0);
    return G_best;
}

static int run_tests(void)
{
    srand(2026);
    int fails = 0, T = 3000;
    for (int t = 1; t <= T; t++) {
        int n = 1 + rand() % 5;
        ll a[8];
        for (int i = 0; i < n; i++) a[i] = 1 + rand() % 60;
        ll g = min_deviation(a, n, 0), b = brute(a, n);
        if (g != b) { fails++; printf("FAIL test %d: greedy=%lld brute=%lld\n", t, g, b); }
    }
    printf("Random tests : %d (n = 1..5, values 1..60, brute force over all reachable values)\n", T);
    printf("Failures     : %d\n", fails);
    printf("%s\n", fails ? "RESULT: SOME TESTS FAILED" : "RESULT: ALL TESTS PASSED");
    return fails != 0;
}

int main(int argc, char **argv)
{
    if (argc > 1 && !strcmp(argv[1], "--test")) return run_tests();
    int n;
    if (scanf("%d", &n) != 1 || n < 1) { fprintf(stderr, "bad input\n"); return 1; }
    ll *a = malloc(n * sizeof(ll));
    for (int i = 0; i < n; i++) if (scanf("%lld", &a[i]) != 1 || a[i] < 1) { fprintf(stderr, "bad input\n"); return 1; }
    printf("A = [");
    for (int i = 0; i < n; i++) printf("%s%lld", i ? ", " : "", a[i]);
    printf("]\n");
    ll d = min_deviation(a, n, 1);
    printf("Minimum deviation = %lld\n", d);
    if (n <= 7) printf("Exhaustive check  = %lld (%s)\n", brute(a, n), brute(a, n) == d ? "matches" : "MISMATCH");
    free(a);
    return 0;
}
