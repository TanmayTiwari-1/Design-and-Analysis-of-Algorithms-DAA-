/*
 * DAA Lab-09  |  Q6 : Reorganise string so equal characters are >= K apart
 * ---------------------------------------------------------------------------
 * GREEDY  Fill positions 0..n-1 left to right.  At position i a character c
 * is AVAILABLE if i - last[c] >= K (it has "cooled down").  Among available
 * characters take the one with the most copies left (ties: the one used
 * longest ago, then smallest code).  If nothing is available the answer is
 * impossible -> empty string.
 * Why most-frequent first: a character with many copies left has the fewest
 * free slots to spare, so postponing it can only make failure more likely;
 * the exhaustive check in --test confirms the rule on thousands of cases.
 *
 * INPUT   S K        (S is a token without spaces)
 * USAGE   ./q06 < sample_input.txt   |   ./q06 --test
 * COMPLEXITY  For each of the n positions we scan the sigma = 256 counters:
 *             O(n*sigma) = O(n) time for a fixed alphabet, O(sigma + n) space.
 *             With a max-heap plus a cool-down queue the same greedy runs in
 *             O(n log sigma).  K <= 1 means no restriction (S itself is valid).
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int reorganise(const char *s, int K, char *out)
{
    int n = (int)strlen(s), cnt[256], last[256];
    memset(cnt, 0, sizeof cnt);
    for (int i = 0; i < n; i++) cnt[(unsigned char)s[i]]++;
    for (int c = 0; c < 256; c++) last[c] = -1000000000;
    if (K <= 1) { strcpy(out, s); return 1; }
    for (int i = 0; i < n; i++) {
        int best = -1;
        for (int c = 0; c < 256; c++) {
            if (cnt[c] == 0 || i - last[c] < K) continue;
            if (best < 0 || cnt[c] > cnt[best] || (cnt[c] == cnt[best] && last[c] < last[best])) best = c;
        }
        if (best < 0) { out[0] = 0; return 0; }
        out[i] = (char)best;
        cnt[best]--;
        last[best] = i;
    }
    out[n] = 0;
    return 1;
}

static int valid(const char *orig, const char *res, int K)
{
    int n = (int)strlen(orig);
    if ((int)strlen(res) != n) return 0;
    int a[256] = {0}, b[256] = {0};
    for (int i = 0; i < n; i++) { a[(unsigned char)orig[i]]++; b[(unsigned char)res[i]]++; }
    if (memcmp(a, b, sizeof a)) return 0;
    for (int i = 0; i < n; i++)
        for (int j = i + 1; j < n && j - i < K; j++)
            if (res[i] == res[j]) return 0;
    return 1;
}

static int cmp_char(const void *a, const void *b) { return *(const char *)a - *(const char *)b; }

static int next_perm(char *a, int n)
{
    int i = n - 2;
    while (i >= 0 && a[i] >= a[i + 1]) i--;
    if (i < 0) return 0;
    int j = n - 1;
    while (a[j] <= a[i]) j--;
    char t = a[i]; a[i] = a[j]; a[j] = t;
    for (int l = i + 1, r = n - 1; l < r; l++, r--) { t = a[l]; a[l] = a[r]; a[r] = t; }
    return 1;
}

/* exhaustive: does ANY arrangement exist? */
static int exists(const char *s, int K)
{
    char p[16];
    int n = (int)strlen(s);
    strcpy(p, s);
    qsort(p, n, 1, cmp_char);
    do { if (valid(s, p, K)) return 1; } while (next_perm(p, n));
    return 0;
}

static int run_tests(void)
{
    srand(2026);
    int fails = 0, T = 3000, possible = 0;
    for (int t = 1; t <= T; t++) {
        int n = 1 + rand() % 8, sig = 1 + rand() % 4, K = 1 + rand() % 5;
        char s[16], out[16];
        for (int i = 0; i < n; i++) s[i] = (char)('a' + rand() % sig);
        s[n] = 0;
        int g = reorganise(s, K, out), e = exists(s, K);
        possible += e;
        if (g != e || (g && !valid(s, out, K))) { fails++; printf("FAIL test %d: S=%s K=%d greedy=%d exists=%d\n", t, s, K, g, e); }
    }
    printf("Random tests : %d (|S| <= 8, alphabet <= 4, K = 1..5; %d of them solvable)\n", T, possible);
    printf("Checks       : greedy succeeds iff some arrangement exists (all permutations tried),\n");
    printf("               and every output is a valid rearrangement\n");
    printf("Failures     : %d\n", fails);
    printf("%s\n", fails ? "RESULT: SOME TESTS FAILED" : "RESULT: ALL TESTS PASSED");
    return fails != 0;
}

int main(int argc, char **argv)
{
    if (argc > 1 && !strcmp(argv[1], "--test")) return run_tests();
    static char s[100002], out[100002];
    int K;
    if (scanf("%100000s %d", s, &K) != 2) { fprintf(stderr, "bad input\n"); return 1; }
    int ok = reorganise(s, K, out);
    printf("S = \"%s\", K = %d\n", s, K);
    if (ok) printf("Result : \"%s\"   (valid: %s)\n", out, valid(s, out, K) ? "yes" : "NO");
    else    printf("Result : \"\"   (impossible -> empty string)\n");
    if (strlen(s) <= 8) printf("Exhaustive check: arrangement %s\n", exists(s, K) ? "exists" : "does not exist");
    return 0;
}
