/*
 * DAA Lab-09  |  Q10 : Shortest common superstring - greedy vs optimum
 * ---------------------------------------------------------------------------
 * The shortest superstring problem is NP-hard.  The classic GREEDY algorithm:
 *     1. drop strings that are duplicates / substrings of another string;
 *     2. while more than one string remains, merge the ordered pair (a, b)
 *        with the LARGEST suffix(a) = prefix(b) overlap;
 *     3. the last remaining string is the answer.
 * The Greedy Superstring Conjecture says |greedy| <= 2 * |optimal| (a
 * 2-approximation; the best PROVEN bound is larger).  See README.md for the
 * status of the conjecture; this program does NOT prove or refute it - it is
 * an experimental workbench:
 *   - exact optimum for small inputs: bitmask DP over visiting orders,
 *     dp[mask][last] = shortest superstring of the strings in mask ending
 *     with string `last` (valid because the reduced set is substring-free,
 *     so an optimal superstring is a permutation merged by max overlaps);
 *   - random search for instances where greedy is worse than optimal;
 *   - the classical family  c(ab)^k , (ba)^k , (ab)^k c  whose ratio -> 2.
 *
 * INPUT   n
 *         s_1 ... s_n         (tokens without spaces)
 * USAGE   ./q10 < sample_input.txt   |   ./q10 --test
 * COMPLEXITY  (N = total length, L = longest string)
 *   Greedy : overlap matrix O(n^2 L^2) naive; each of the n-1 merges recomputes
 *            one row/column and scans n^2 entries => O(n^3 + n^2 N^2) worst case
 *            for this simple implementation (linear-time implementations
 *            using Aho-Corasick / suffix structures are known).
 *   Optimum: O(2^n * n^2) time, O(2^n * n) space (n <= 15 here).
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXS 64
#define MAXL 4096

static int overlap(const char *a, const char *b)
{
    int la = (int)strlen(a), lb = (int)strlen(b), m = la < lb ? la : lb;
    for (int k = m; k > 0; k--) if (memcmp(a + la - k, b, k) == 0) return k;
    return 0;
}

/* remove duplicates and strings contained in another; returns new count */
static int reduce(char **in, int n, char **out)
{
    int m = 0;
    for (int i = 0; i < n; i++) {
        int drop = 0;
        for (int j = 0; j < n && !drop; j++) {
            if (i == j || !strstr(in[j], in[i])) continue;
            if (strcmp(in[i], in[j]) != 0 || j < i) drop = 1;
        }
        if (!drop) out[m++] = in[i];
    }
    return m;
}

static char *greedy(char **in, int m, int verbose)
{
    static int ov[MAXS][MAXS];
    char *s[MAXS];
    int cnt = m;
    for (int i = 0; i < m; i++) s[i] = strdup(in[i]);
    for (int i = 0; i < cnt; i++)
        for (int j = 0; j < cnt; j++) ov[i][j] = (i == j) ? -1 : overlap(s[i], s[j]);
    while (cnt > 1) {
        int bi = 0, bj = 1, best = -1;
        for (int i = 0; i < cnt; i++)
            for (int j = 0; j < cnt; j++)
                if (i != j && ov[i][j] > best) { best = ov[i][j]; bi = i; bj = j; }
        char *merged = malloc(strlen(s[bi]) + strlen(s[bj]) + 1);
        strcpy(merged, s[bi]);
        strcat(merged, s[bj] + best);
        if (verbose) printf("  merge \"%s\" + \"%s\"  overlap %d  -> \"%s\"\n", s[bi], s[bj], best, merged);
        free(s[bi]); free(s[bj]);
        s[bi] = merged;
        int last = cnt - 1;
        if (bj != last) {
            s[bj] = s[last];
            for (int k = 0; k < cnt; k++) { ov[bj][k] = ov[last][k]; ov[k][bj] = ov[k][last]; }
            if (bi == last) bi = bj;
        }
        cnt--;
        for (int k = 0; k < cnt; k++) {
            if (k == bi) { ov[bi][bi] = -1; continue; }
            ov[bi][k] = overlap(s[bi], s[k]);
            ov[k][bi] = overlap(s[k], s[bi]);
        }
    }
    return s[0];
}

static char *optimum(char **s, int m)
{
    if (m > 15) return NULL;
    int ov[MAXS][MAXS], len[MAXS];
    for (int i = 0; i < m; i++) {
        len[i] = (int)strlen(s[i]);
        for (int j = 0; j < m; j++) ov[i][j] = (i == j) ? 0 : overlap(s[i], s[j]);
    }
    int N = 1 << m, INF = 1 << 29;
    int *dp = malloc((size_t)N * m * sizeof(int)), *par = malloc((size_t)N * m * sizeof(int));
    for (long i = 0; i < (long)N * m; i++) { dp[i] = INF; par[i] = -1; }
    for (int i = 0; i < m; i++) dp[(1 << i) * m + i] = len[i];
    for (int mask = 1; mask < N; mask++)
        for (int last = 0; last < m; last++) {
            int cur = dp[mask * m + last];
            if (cur >= INF) continue;
            for (int nx = 0; nx < m; nx++) {
                if (mask >> nx & 1) continue;
                int nm = mask | (1 << nx), v = cur + len[nx] - ov[last][nx];
                if (v < dp[nm * m + nx]) { dp[nm * m + nx] = v; par[nm * m + nx] = last; }
            }
        }
    int bl = 0, full = N - 1;
    for (int i = 1; i < m; i++) if (dp[full * m + i] < dp[full * m + bl]) bl = i;
    int ord[MAXS], k = m, mask = full, cur = bl;
    while (cur >= 0) { ord[--k] = cur; int p = par[mask * m + cur]; mask ^= 1 << cur; cur = p; }
    char *res = malloc(dp[full * m + bl] + 1);
    strcpy(res, s[ord[0]]);
    for (int i = 1; i < m; i++) strcat(res, s[ord[i]] + ov[ord[i - 1]][ord[i]]);
    free(dp); free(par);
    return res;
}

static int contains_all(const char *sup, char **in, int n)
{
    for (int i = 0; i < n; i++) if (!strstr(sup, in[i])) return 0;
    return 1;
}

static void family(int k, char *a, char *b, char *c)
{
    strcpy(a, "c"); strcpy(b, ""); strcpy(c, "");
    for (int i = 0; i < k; i++) { strcat(a, "ab"); strcat(b, "ba"); strcat(c, "ab"); }
    strcat(c, "c");
}

static int run_tests(void)
{
    srand(2026);
    int T = 2000, fails = 0, worse = 0;
    double maxr = 1.0;
    char worst[512] = "";
    for (int t = 0; t < T; t++) {
        int n = 2 + rand() % 6, sig = 2 + rand() % 2;
        char *raw[8], *red[8];
        for (int i = 0; i < n; i++) {
            int l = 2 + rand() % 5;
            raw[i] = malloc(l + 1);
            for (int j = 0; j < l; j++) raw[i][j] = (char)('a' + rand() % sig);
            raw[i][l] = 0;
        }
        int m = reduce(raw, n, red);
        char *g = greedy(red, m, 0), *o = optimum(red, m);
        if (!contains_all(g, raw, n) || !contains_all(o, raw, n) || strlen(g) < strlen(o)) {
            fails++; printf("FAIL test %d\n", t);
        }
        if (strlen(g) > strlen(o)) {
            worse++;
            double r = (double)strlen(g) / strlen(o);
            if (r > maxr) {
                maxr = r;
                worst[0] = 0;
                for (int i = 0; i < m; i++) { strcat(worst, red[i]); strcat(worst, " "); }
                sprintf(worst + strlen(worst), "(greedy %zu, optimum %zu)", strlen(g), strlen(o));
            }
        }
        free(g); free(o);
        for (int i = 0; i < n; i++) free(raw[i]);
    }
    printf("Random tests                     : %d (2-7 strings over {a,b} or {a,b,c}, length 2-6)\n", T);
    printf("Validity failures                : %d (outputs contain every input; greedy >= optimum)\n", fails);
    printf("Instances where greedy > optimum : %d\n", worse);
    printf("Worst ratio found                : %.4f\n", maxr);
    printf("  instance: %s\n", worst);
    printf("\nClassical family  c(ab)^k, (ba)^k, (ab)^k c   (optimum = 2k+4)\n");
    printf("%4s %8s %8s %8s\n", "k", "greedy", "optimum", "ratio");
    int ks[] = { 2, 3, 5, 10, 20, 50 };
    for (int q = 0; q < 6; q++) {
        char a[300], b[300], c[300], *in[3] = { a, b, c }, *red[3];
        family(ks[q], a, b, c);
        int m = reduce(in, 3, red);
        char *g = greedy(red, m, 0), *o = optimum(red, m);
        if (!contains_all(g, in, 3) || !contains_all(o, in, 3)) fails++;
        printf("%4d %8zu %8zu %8.4f\n", ks[q], strlen(g), strlen(o), (double)strlen(g) / strlen(o));
        free(g); free(o);
    }
    printf("%s\n", fails ? "RESULT: SOME TESTS FAILED" : "RESULT: ALL TESTS PASSED");
    return fails != 0;
}

int main(int argc, char **argv)
{
    if (argc > 1 && !strcmp(argv[1], "--test")) return run_tests();
    int n;
    if (scanf("%d", &n) != 1 || n < 1 || n > MAXS) { fprintf(stderr, "bad input\n"); return 1; }
    char *raw[MAXS], *red[MAXS];
    for (int i = 0; i < n; i++) {
        raw[i] = malloc(MAXL);
        if (scanf("%4095s", raw[i]) != 1) { fprintf(stderr, "bad input\n"); return 1; }
    }
    int m = reduce(raw, n, red);
    printf("Input strings: %d, after removing duplicates/substrings: %d\n", n, m);
    printf("Greedy merging:\n");
    char *g = greedy(red, m, 1);
    printf("Greedy superstring  : %s   (length %zu, contains all: %s)\n", g, strlen(g), contains_all(g, raw, n) ? "yes" : "NO");
    char *o = optimum(red, m);
    if (o) {
        printf("Optimal superstring : %s   (length %zu, contains all: %s)\n", o, strlen(o), contains_all(o, raw, n) ? "yes" : "NO");
        printf("Approximation ratio |greedy|/|optimum| = %.4f\n", (double)strlen(g) / strlen(o));
    } else {
        printf("Optimum skipped (more than 15 strings after reduction).\n");
    }
    return 0;
}
