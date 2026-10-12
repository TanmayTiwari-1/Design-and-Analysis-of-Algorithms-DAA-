/*
 * DAA Lab-09  |  Q4 : Minimum cost to connect sticks
 * ---------------------------------------------------------------------------
 * Connecting sticks x and y costs x + y and yields one stick of length x + y.
 * This is exactly Huffman merging: a stick that took part in j merges is
 * "paid" j times, so total cost = sum L_i * depth_i of a binary merge tree,
 * minimised by always merging the two shortest sticks (min-heap).
 *
 * INPUT   n
 *         L_1 ... L_n
 * USAGE   ./q04 < sample_input.txt   |   ./q04 --test
 * COMPLEXITY  build heap O(n); n-1 rounds x (2 delete-min + 1 insert, O(log n))
 *             => O(n log n) time, O(n) space.
 *             (After sorting, a two-queue variant runs the merge phase in O(n).)
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
        if (hp[p] <= hp[i]) break;
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
        if (l < hn && hp[l] < hp[s]) s = l;
        if (rr < hn && hp[rr] < hp[s]) s = rr;
        if (s == i) break;
        ll t = hp[s]; hp[s] = hp[i]; hp[i] = t; i = s;
    }
    return r;
}

static ll min_cost(const ll *a, int n, int verbose)
{
    hp = malloc((n + 1) * sizeof(ll));
    hn = 0;
    for (int i = 0; i < n; i++) hpush(a[i]);
    ll cost = 0;
    int step = 0;
    while (hn > 1) {
        ll x = hpop(), y = hpop();
        cost += x + y;
        hpush(x + y);
        if (verbose) printf("  merge %2d: %lld + %lld -> %lld   (running cost %lld)\n", ++step, x, y, x + y, cost);
    }
    free(hp);
    return cost;
}

static ll brute(const ll *a, int n)
{
    if (n == 1) return 0;
    ll best = -1, b[16];
    for (int i = 0; i < n; i++)
        for (int j = i + 1; j < n; j++) {
            int k = 0;
            for (int t = 0; t < n; t++) if (t != i && t != j) b[k++] = a[t];
            b[k++] = a[i] + a[j];
            ll c = a[i] + a[j] + brute(b, n - 1);
            if (best < 0 || c < best) best = c;
        }
    return best;
}

static int run_tests(void)
{
    srand(2026);
    int fails = 0, T = 1000;
    for (int t = 1; t <= T; t++) {
        int n = 1 + rand() % 7;
        ll a[8];
        for (int i = 0; i < n; i++) a[i] = 1 + rand() % 20;
        ll g = min_cost(a, n, 0), b = brute(a, n);
        if (g != b) { fails++; printf("FAIL test %d: n=%d greedy=%lld brute=%lld\n", t, n, g, b); }
    }
    printf("Random tests : %d (n = 1..7, brute force tries every merge order)\n", T);
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
    for (int i = 0; i < n; i++) if (scanf("%lld", &a[i]) != 1) { fprintf(stderr, "bad input\n"); return 1; }
    printf("n = %d sticks\n", n);
    ll c = min_cost(a, n, 1);
    printf("Minimum total cost = %lld\n", c);
    if (n <= 9) printf("Brute-force check  = %lld (%s)\n", brute(a, n), brute(a, n) == c ? "matches" : "MISMATCH");
    free(a);
    return 0;
}
