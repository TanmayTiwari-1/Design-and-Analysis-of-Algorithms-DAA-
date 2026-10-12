/*
 * DAA Lab-09  |  Q5 : Candy distribution (bi-directional slope greedy)
 * ---------------------------------------------------------------------------
 * Rule: every child >= 1 candy; a child with a strictly higher rating than a
 * neighbour must get strictly more candies than that neighbour.
 *
 * METHOD A - two passes
 *   left -> right : c[i] = c[i-1] + 1 if r[i] > r[i-1], else 1
 *   right -> left : if r[i] > r[i+1], c[i] = max(c[i], c[i+1] + 1)
 *   Pass 1 satisfies every "left neighbour" constraint with the smallest
 *   possible values, pass 2 raises values only as much as the "right
 *   neighbour" constraints force; raising never breaks pass-1 constraints.
 *   Each c[i] is therefore the pointwise minimum of any valid assignment.
 *
 * METHOD B - slope scan, O(1) extra space
 *   Walk once; track the length of the current ascending run (up), descending
 *   run (down) and the height of the last peak.  A descent longer than the
 *   peak's height must lift the peak by one extra candy.
 *
 * INPUT   n
 *         r_1 ... r_n
 * USAGE   ./q05 < sample_input.txt   |   ./q05 --test
 * COMPLEXITY  Method A: 2 linear passes => O(n) time, O(n) space.
 *             Method B: 1 pass          => O(n) time, O(1) extra space.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef long long ll;

static ll candies_two_pass(const int *r, int n, int *c)
{
    for (int i = 0; i < n; i++) c[i] = 1;
    for (int i = 1; i < n; i++) if (r[i] > r[i - 1]) c[i] = c[i - 1] + 1;
    for (int i = n - 2; i >= 0; i--) if (r[i] > r[i + 1] && c[i] < c[i + 1] + 1) c[i] = c[i + 1] + 1;
    ll s = 0;
    for (int i = 0; i < n; i++) s += c[i];
    return s;
}

static ll candies_slope(const int *r, int n)
{
    ll total = 1;
    int up = 0, down = 0, peak = 0;
    for (int i = 1; i < n; i++) {
        if (r[i] > r[i - 1]) { up++; peak = up; down = 0; total += 1 + up; }
        else if (r[i] == r[i - 1]) { up = down = peak = 0; total += 1; }
        else { up = 0; down++; total += 1 + down; if (peak >= down) total--; }
    }
    return total;
}

static int valid(const int *r, const int *c, int n)
{
    for (int i = 0; i < n; i++) {
        if (c[i] < 1) return 0;
        if (i > 0 && r[i] > r[i - 1] && c[i] <= c[i - 1]) return 0;
        if (i + 1 < n && r[i] > r[i + 1] && c[i] <= c[i + 1]) return 0;
    }
    return 1;
}

/* exhaustive: every assignment with 1..n candies per child */
static int G_n, G_best;
static int G_r[8], G_c[8];
static void rec(int i, int sum)
{
    if (sum >= G_best) return;
    if (i == G_n) { if (valid(G_r, G_c, G_n)) G_best = sum; return; }
    for (int v = 1; v <= G_n; v++) { G_c[i] = v; rec(i + 1, sum + v); }
}
static int brute(const int *r, int n)
{
    G_n = n; G_best = 1 << 30;
    memcpy(G_r, r, n * sizeof(int));
    rec(0, 0);
    return G_best;
}

static int run_tests(void)
{
    srand(2026);
    int fails = 0, T = 1500;
    for (int t = 1; t <= T; t++) {
        int n = 1 + rand() % 7, r[8], c[8];
        for (int i = 0; i < n; i++) r[i] = 1 + rand() % 5;
        ll a = candies_two_pass(r, n, c), b = candies_slope(r, n);
        int e = brute(r, n);
        if (a != e || b != e || !valid(r, c, n)) { fails++; printf("FAIL test %d: two-pass=%lld slope=%lld brute=%d\n", t, a, b, e); }
    }
    printf("Random tests : %d (n = 1..7, ratings 1..5 so ties are frequent)\n", T);
    printf("Checks       : two-pass == slope scan == exhaustive minimum, assignment valid\n");
    printf("Failures     : %d\n", fails);
    printf("%s\n", fails ? "RESULT: SOME TESTS FAILED" : "RESULT: ALL TESTS PASSED");
    return fails != 0;
}

int main(int argc, char **argv)
{
    if (argc > 1 && !strcmp(argv[1], "--test")) return run_tests();
    int n;
    if (scanf("%d", &n) != 1 || n < 1) { fprintf(stderr, "bad input\n"); return 1; }
    int *r = malloc(n * sizeof(int)), *c = malloc(n * sizeof(int));
    for (int i = 0; i < n; i++) if (scanf("%d", &r[i]) != 1) { fprintf(stderr, "bad input\n"); return 1; }
    ll a = candies_two_pass(r, n, c), b = candies_slope(r, n);
    printf("ratings : ");
    for (int i = 0; i < n; i++) printf("%3d", r[i]);
    printf("\ncandies : ");
    for (int i = 0; i < n; i++) printf("%3d", c[i]);
    printf("\nMinimum total candies (two-pass)   = %lld\n", a);
    printf("Minimum total candies (slope scan) = %lld\n", b);
    printf("Assignment valid                   = %s\n", valid(r, c, n) ? "yes" : "NO");
    if (n <= 7) printf("Exhaustive check                   = %d\n", brute(r, n));
    free(r); free(c);
    return 0;
}
