/*
 * DAA Lab-09  |  Q2 : Huffman Coding with canonical codebook
 * ---------------------------------------------------------------------------
 * ALGORITHM
 *   1. Min-heap of (frequency, creation order).  Repeatedly remove the two
 *      lightest nodes and replace them by a parent whose weight is their sum.
 *      Code length of a symbol = depth of its leaf.  (Greedy choice: the two
 *      rarest symbols can be made sibling leaves at the deepest level.)
 *   2. CANONICAL codebook: sort symbols by (length, symbol); the first gets
 *      all zeros; every next code = previous code + 1 (binary), then padded
 *      with zeros on the right up to its own length.  Codes of equal length
 *      are therefore consecutive and lexicographically ordered.
 *
 * INPUT   n
 *         symbol_1 freq_1
 *         ...
 *         symbol_n freq_n          (symbol = token without spaces, <= 31 chars)
 * USAGE   ./q02 < sample_input.txt      |    ./q02 --test
 * COMPLEXITY  n-1 merges, each 2 delete-min + 1 insert = O(log n)
 *             => O(n log n) time, O(n) space (+ O(n log n) to sort / store codes).
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#define MAXN   1024
#define MAXSYM 32
#define MAXLEN 1030

typedef struct { long long f; int left, right, order; } Node;
static Node nd[2 * MAXN];
static int heap_[2 * MAXN], hs;

static int hless(int a, int b)
{
    if (nd[a].f != nd[b].f) return nd[a].f < nd[b].f;
    return nd[a].order < nd[b].order;
}
static void hpush(int x)
{
    int i = hs++;
    heap_[i] = x;
    while (i > 0) {
        int p = (i - 1) / 2;
        if (!hless(heap_[i], heap_[p])) break;
        int t = heap_[i]; heap_[i] = heap_[p]; heap_[p] = t;
        i = p;
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
        int t = heap_[i]; heap_[i] = heap_[s]; heap_[s] = t;
        i = s;
    }
    return r;
}

static void huffman_lengths(int n, const long long *f, int *len)
{
    if (n == 1) { len[0] = 1; return; }
    hs = 0;
    for (int i = 0; i < n; i++) {
        nd[i].f = f[i]; nd[i].left = nd[i].right = -1; nd[i].order = i;
        hpush(i);
    }
    int total = n;
    while (hs > 1) {
        int a = hpop(), b = hpop();
        nd[total].f = nd[a].f + nd[b].f;
        nd[total].left = a; nd[total].right = b; nd[total].order = total;
        hpush(total++);
    }
    static int dep[2 * MAXN];
    int root = total - 1;
    dep[root] = 0;
    for (int k = root; k >= 0; k--)               /* parents have larger index than children */
        if (nd[k].left >= 0) { dep[nd[k].left] = dep[k] + 1; dep[nd[k].right] = dep[k] + 1; }
    for (int i = 0; i < n; i++) len[i] = dep[i];
}

static char G_sym[MAXN][MAXSYM];
static int  G_len[MAXN];
static char codes[MAXN][MAXLEN];

static int cmp_canon(const void *a, const void *b)
{
    int i = *(const int *)a, j = *(const int *)b;
    if (G_len[i] != G_len[j]) return G_len[i] - G_len[j];
    return strcmp(G_sym[i], G_sym[j]);
}

/* canonical assignment; order[] receives symbol indices in canonical order */
static void canonical(int n, const int *len, int *order)
{
    for (int i = 0; i < n; i++) { G_len[i] = len[i]; order[i] = i; }
    qsort(order, n, sizeof(int), cmp_canon);
    char cur[MAXLEN];
    int curlen = 0;
    for (int k = 0; k < n; k++) {
        int s = order[k], L = len[s];
        if (k == 0) {
            memset(cur, '0', L);
        } else {
            for (int p = curlen - 1; p >= 0; p--) {            /* binary +1 */
                if (cur[p] == '0') { cur[p] = '1'; break; }
                cur[p] = '0';
            }
            for (int p = curlen; p < L; p++) cur[p] = '0';       /* pad on the right */
        }
        curlen = L;
        memcpy(codes[s], cur, L);
        codes[s][L] = 0;
    }
}

static int prefix_free(int n, const int *len)
{
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            if (i != j && len[i] <= len[j] && strncmp(codes[i], codes[j], len[i]) == 0) return 0;
    return 1;
}

/* exact optimum by DP over subsets: cost(S) = sum(S) + min over splits (cost(A)+cost(B)) */
static long long optimal_cost_bruteforce(int n, const long long *f)
{
    int N = 1 << n;
    long long *sum = calloc(N, sizeof(long long)), *cost = calloc(N, sizeof(long long));
    for (int m = 1; m < N; m++) sum[m] = sum[m & (m - 1)] + f[__builtin_ctz(m)];
    for (int m = 1; m < N; m++) {
        if ((m & (m - 1)) == 0) { cost[m] = 0; continue; }
        long long best = LLONG_MAX;
        int low = m & -m;
        for (int sub = (m - 1) & m; sub > 0; sub = (sub - 1) & m) {
            if (!(sub & low)) continue;
            long long c = cost[sub] + cost[m ^ sub];
            if (c < best) best = c;
        }
        cost[m] = best + sum[m];
    }
    long long r = cost[N - 1];
    free(sum); free(cost);
    return r;
}

static long long weighted_length(int n, const long long *f, const int *len)
{
    long long s = 0;
    for (int i = 0; i < n; i++) s += f[i] * len[i];
    return s;
}

static int run_tests(void)
{
    srand(2026);
    int fails = 0, T = 500;
    for (int t = 1; t <= T; t++) {
        int n = 2 + rand() % 9;
        long long f[16]; int len[16], ord[16];
        for (int i = 0; i < n; i++) { f[i] = 1 + rand() % 9; snprintf(G_sym[i], MAXSYM, "s%02d", i); }
        huffman_lengths(n, f, len);
        canonical(n, len, ord);
        long long mine = weighted_length(n, f, len), opt = optimal_cost_bruteforce(n, f);
        long double kraft = 0, p2 = 1;
        for (int i = 0; i < n; i++) { p2 = 1; for (int k = 0; k < len[i]; k++) p2 /= 2; kraft += p2; }
        int ok = (mine == opt) && prefix_free(n, len) && kraft == 1.0L;
        for (int k = 1; k < n && ok; k++) {            /* canonical: non-decreasing length, increasing code */
            int a = ord[k - 1], b = ord[k];
            if (len[a] > len[b]) ok = 0;
            if (len[a] == len[b] && strcmp(codes[a], codes[b]) >= 0) ok = 0;
        }
        if (!ok) { fails++; printf("FAIL test %d (n=%d): huffman=%lld optimum=%lld\n", t, n, mine, opt); }
    }
    printf("Random tests                       : %d (n = 2..10, many tied frequencies)\n", T);
    printf("Checks per test                    : optimal cost == subset-DP optimum,\n");
    printf("                                     prefix-free, Kraft sum == 1, canonical order\n");
    printf("Failures                           : %d\n", fails);
    printf("%s\n", fails ? "RESULT: SOME TESTS FAILED" : "RESULT: ALL TESTS PASSED");
    return fails != 0;
}

int main(int argc, char **argv)
{
    if (argc > 1 && !strcmp(argv[1], "--test")) return run_tests();
    int n;
    if (scanf("%d", &n) != 1 || n < 1 || n > MAXN) { fprintf(stderr, "bad input\n"); return 1; }
    long long f[MAXN]; int len[MAXN], ord[MAXN];
    long long total = 0;
    for (int i = 0; i < n; i++) {
        if (scanf("%31s %lld", G_sym[i], &f[i]) != 2 || f[i] <= 0) { fprintf(stderr, "bad symbol %d\n", i + 1); return 1; }
        total += f[i];
    }
    huffman_lengths(n, f, len);
    canonical(n, len, ord);
    printf("Canonical Huffman codebook (sorted by length, then symbol)\n");
    printf("%-10s %8s %7s  %s\n", "Symbol", "Freq", "Length", "Code");
    for (int k = 0; k < n; k++) {
        int s = ord[k];
        printf("%-10s %8lld %7d  %s\n", G_sym[s], f[s], len[s], codes[s]);
    }
    long long wpl = weighted_length(n, f, len);
    printf("Total frequency                  = %lld\n", total);
    printf("Sum f*len (total encoded bits)   = %lld\n", wpl);
    printf("Expected code length             = %.4f bits/symbol\n", (double)wpl / total);
    printf("Prefix-free check                = %s\n", prefix_free(n, len) ? "OK" : "VIOLATED");
    if (n <= 12)
        printf("Optimum by exhaustive subset DP  = %lld (%s)\n", optimal_cost_bruteforce(n, f),
               optimal_cost_bruteforce(n, f) == wpl || n == 1 ? "matches" : "MISMATCH");
    return 0;
}
