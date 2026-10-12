/*
 * DAA Lab-09  |  Q9 : Hu-Tucker algorithm (optimal alphabetic binary tree)
 * ---------------------------------------------------------------------------
 * PROBLEM  Leaves keep the order w_1..w_n; minimise sum w_i * depth_i over all
 * binary trees whose left-to-right leaf order is 1..n.
 *
 * PHASE 1 - COMBINATION (greedy simulation)
 *   Work on a sequence of nodes.  Original leaves are "square" (terminal),
 *   merged nodes are "circle".  Two nodes are COMPATIBLE if no square node
 *   lies strictly between them.  Repeat: among all compatible pairs pick the
 *   one with the smallest weight sum (ties: leftmost first node, then leftmost
 *   second node); replace the pair by one circle node of weight w_i + w_j,
 *   placed at the position of the LEFT node.  Every leaf below the new node
 *   gets depth + 1.  After n-1 merges we know the depth of each leaf.
 * PHASE 2 - RECOMBINATION
 *   The depths of phase 1 are in general NOT a Huffman tree but always
 *   admit an alphabetic tree: scan leaves left to right with a stack and
 *   merge the two top entries whenever their depths are equal.  This builds
 *   the tree (and the codes) with the same leaf depths, hence same cost.
 *
 * INPUT   n
 *         w_1 ... w_n      (positive integers, in leaf order)
 * USAGE   ./q09 < sample_input.txt   |   ./q09 --test
 * COMPLEXITY  Phase 1: n-1 rounds; a round scans every compatible pair,
 *             O(n^2) pairs worst case, plus O(n) to update depths
 *             => O(n^3) worst case in this simple simulation (usually far less
 *             because a scan stops at the next square node).  Phase 2: O(n).
 *             Hu-Tucker's priority-queue implementation achieves O(n log n);
 *             the validation DP below is O(n^3) time, O(n^2) space.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#define MAXN 256
typedef long long ll;

static void phase1(const ll *w, int n, int *dep)
{
    ll cw[MAXN];
    int sq[MAXN], who[MAXN], leafNode[MAXN];
    int cnt = n, nextid = n;
    for (int i = 0; i < n; i++) { cw[i] = w[i]; sq[i] = 1; who[i] = i; leafNode[i] = i; dep[i] = 0; }
    while (cnt > 1) {
        ll best = LLONG_MAX;
        int bi = -1, bj = -1;
        for (int i = 0; i < cnt; i++)
            for (int j = i + 1; j < cnt; j++) {
                ll s = cw[i] + cw[j];
                if (s < best) { best = s; bi = i; bj = j; }
                if (sq[j]) break;                      /* squares block further partners */
            }
        int a = who[bi], b = who[bj];
        for (int l = 0; l < n; l++)
            if (leafNode[l] == a || leafNode[l] == b) { dep[l]++; leafNode[l] = nextid; }
        cw[bi] = best; sq[bi] = 0; who[bi] = nextid++;
        for (int k = bj; k < cnt - 1; k++) { cw[k] = cw[k + 1]; sq[k] = sq[k + 1]; who[k] = who[k + 1]; }
        cnt--;
    }
}

static int L[2 * MAXN], R[2 * MAXN];
static int build_tree(int n, const int *dep)
{
    int stN[MAXN], stD[MAXN], sp = 0, nid = n;
    for (int i = 0; i < n; i++) {
        stN[sp] = i; stD[sp] = dep[i]; sp++;
        while (sp >= 2 && stD[sp - 1] == stD[sp - 2]) {
            L[nid] = stN[sp - 2]; R[nid] = stN[sp - 1];
            sp--;
            stN[sp - 1] = nid++;
            stD[sp - 1]--;
        }
    }
    return (sp == 1 && stD[0] == 0) ? stN[0] : -1;
}

static char codes[MAXN][MAXN + 2];
static void assign_codes(int node, int n, char *buf, int len)
{
    if (node < n) { buf[len] = 0; strcpy(codes[node], len ? buf : "0"); return; }
    buf[len] = '0'; assign_codes(L[node], n, buf, len + 1);
    buf[len] = '1'; assign_codes(R[node], n, buf, len + 1);
}

static ll optimal_alphabetic_dp(const ll *w, int n)
{
    static ll C[MAXN][MAXN], P[MAXN + 1];
    P[0] = 0;
    for (int i = 0; i < n; i++) P[i + 1] = P[i] + w[i];
    for (int i = 0; i < n; i++) C[i][i] = 0;
    for (int len = 2; len <= n; len++)
        for (int i = 0; i + len - 1 < n; i++) {
            int j = i + len - 1;
            ll best = LLONG_MAX;
            for (int k = i; k < j; k++) if (C[i][k] + C[k + 1][j] < best) best = C[i][k] + C[k + 1][j];
            C[i][j] = best + P[j + 1] - P[i];
        }
    return C[0][n - 1];
}

static ll cost_of(const ll *w, const int *dep, int n)
{
    ll s = 0;
    for (int i = 0; i < n; i++) s += w[i] * dep[i];
    return s;
}

static int prefix_free(int n)
{
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            if (i != j && strncmp(codes[i], codes[j], strlen(codes[i])) == 0) return 0;
    return 1;
}

static int run_tests(void)
{
    srand(2026);
    int fails = 0, T = 3000;
    for (int t = 1; t <= T; t++) {
        int n = 1 + rand() % 14, dep[MAXN];
        ll w[MAXN];
        int hi = (t % 2) ? 9 : 100;
        for (int i = 0; i < n; i++) w[i] = 1 + rand() % hi;
        phase1(w, n, dep);
        int root = build_tree(n, dep);
        ll c = cost_of(w, dep, n), opt = optimal_alphabetic_dp(w, n);
        int ok = (root >= 0) && (c == opt);
        if (ok) { char buf[MAXN + 2]; assign_codes(root, n, buf, 0); ok = prefix_free(n); }
        if (!ok) { fails++; printf("FAIL test %d: n=%d hu-tucker=%lld optimum=%lld root=%d\n", t, n, c, opt, root); }
    }
    printf("Random tests : %d (n = 1..14, weights 1..9 [many ties] and 1..100)\n", T);
    printf("Checks       : Hu-Tucker cost == optimal alphabetic tree by O(n^3) interval DP,\n");
    printf("               phase-2 tree exists, leaf order preserved, codes prefix-free\n");
    printf("Failures     : %d\n", fails);
    printf("%s\n", fails ? "RESULT: SOME TESTS FAILED" : "RESULT: ALL TESTS PASSED");
    return fails != 0;
}

int main(int argc, char **argv)
{
    if (argc > 1 && !strcmp(argv[1], "--test")) return run_tests();
    int n;
    if (scanf("%d", &n) != 1 || n < 1 || n > MAXN) { fprintf(stderr, "bad input\n"); return 1; }
    ll w[MAXN]; int dep[MAXN];
    for (int i = 0; i < n; i++) if (scanf("%lld", &w[i]) != 1 || w[i] < 1) { fprintf(stderr, "bad input\n"); return 1; }
    phase1(w, n, dep);
    int root = build_tree(n, dep);
    if (root < 0) { printf("phase 2 failed (should not happen)\n"); return 1; }
    char buf[MAXN + 2];
    assign_codes(root, n, buf, 0);
    printf("Optimal alphabetic tree (leaf order preserved)\n");
    printf("%5s %8s %7s  %s\n", "Leaf", "Weight", "Depth", "Code");
    for (int i = 0; i < n; i++) printf("%5d %8lld %7d  %s\n", i + 1, w[i], dep[i], codes[i]);
    ll c = cost_of(w, dep, n);
    printf("Cost  sum w_i*depth_i (Hu-Tucker) = %lld\n", c);
    printf("Cost  by O(n^3) interval DP       = %lld (%s)\n", optimal_alphabetic_dp(w, n),
           optimal_alphabetic_dp(w, n) == c ? "matches" : "MISMATCH");
    printf("Prefix-free = %s\n", prefix_free(n) ? "yes" : "NO");
    return 0;
}
