/*
 * DAA Lab-11, Q3: Bidirectional BFS for the shortest transformation sequence
 *
 * Graph: one node per word, edge between words at Hamming distance exactly 1.
 *
 * Building neighbours WITHOUT the O(N^2 * L) pairwise comparison:
 *   Two words differ in exactly one position i  <=>  they agree after replacing
 *   position i by a wildcard.  For every position i we encode each word as a
 *   base-27 integer (letters 1..26, wildcard 0; 27^10 < 2^63), blank out
 *   position i, and sort the words by that key.  Words with equal key form a
 *   "group" = a clique of mutual neighbours.  A group is stored as a contiguous
 *   range in the sorted order, so a word's neighbours via position i are simply
 *   the range [gbeg, gend).
 *
 * Search: bidirectional BFS, always expanding (a whole level of) the side with
 *   the smaller frontier.  Each (position, group) is scanned at most once per
 *   direction, so the work per direction is O(N*L) instead of O(N*L*group size).
 *   After a full level expansion in which the searches touched, the minimum
 *   candidate  dist_s[u] + 1 + dist_other[v]  is the exact answer.
 *   Parent arrays give the actual word path.
 *
 * Input format:
 *     N L
 *     w_1 ... w_N          (dictionary D, lowercase, length L)
 *     A B                  (start and target)
 *   B must be in D.  A need not be in D (it is added as an extra start node).
 *
 * Complexity: build  O(L * N log N)   (L sorts of N 64-bit keys)
 *             search O(N * L)         (worst case, no early exit)
 *             space  O(N * L)
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

typedef struct { uint64_t key; int idx; } Pair;

static int cmp_pair(const void *a, const void *b)
{
    const Pair *x = a, *y = b;
    if (x->key != y->key) return x->key < y->key ? -1 : 1;
    return x->idx - y->idx;
}

static int valid_word(const char *w, int L)
{
    if ((int)strlen(w) != L) return 0;
    for (int i = 0; i < L; i++) if (w[i] < 'a' || w[i] > 'z') return 0;
    return 1;
}

int main(void)
{
    int N, L;
    if (scanf("%d %d", &N, &L) != 2 || N < 1 || L < 1 || L > 12) {
        fprintf(stderr, "error: bad header (expected: N L, 1<=L<=12)\n");
        return 1;
    }
    int V = N + 1;                                  /* slot N reserved for A if A not in D */
    char *words = malloc((size_t)V * (L + 1));
    char buf[256];
    for (int i = 0; i < N; i++) {
        if (scanf("%255s", buf) != 1 || !valid_word(buf, L)) {
            fprintf(stderr, "error: dictionary word %d invalid (need %d lowercase letters)\n", i + 1, L);
            return 1;
        }
        strcpy(words + (size_t)i * (L + 1), buf);
    }
    char A[256], B[256];
    if (scanf("%255s %255s", A, B) != 2 || !valid_word(A, L) || !valid_word(B, L)) {
        fprintf(stderr, "error: A and B must be lowercase words of length %d\n", L);
        return 1;
    }

    int ia = -1, ib = -1;
    for (int i = 0; i < N; i++) {
        if (ia < 0 && strcmp(words + (size_t)i * (L + 1), A) == 0) ia = i;
        if (ib < 0 && strcmp(words + (size_t)i * (L + 1), B) == 0) ib = i;
    }
    if (strcmp(A, B) == 0) {
        printf("Minimum mutations k = 0\nPath: %s\n", A);
        return 0;
    }
    if (ib < 0) {
        printf("Minimum mutations k = -1\n(target word is not in the dictionary)\n");
        return 0;
    }
    if (ia < 0) {                                   /* A not in D: add it as a start-only node */
        ia = N;
        strcpy(words + (size_t)N * (L + 1), A);
    } else {
        V = N;                                      /* A already in D */
    }

    /* base-27 encoding */
    uint64_t pw[16];
    pw[0] = 1;
    for (int i = 1; i < 16; i++) pw[i] = pw[i - 1] * 27;
    uint64_t *full = malloc(sizeof(uint64_t) * V);
    for (int w = 0; w < V; w++) {
        const char *s = words + (size_t)w * (L + 1);
        uint64_t c = 0;
        for (int j = 0; j < L; j++) c += (uint64_t)(s[j] - 'a' + 1) * pw[j];
        full[w] = c;
    }

    /* group structure for each position */
    size_t LV = (size_t)L * V;
    int *order = malloc(sizeof(int) * LV);          /* order[i*V + k] = word at sorted rank k */
    int *gbeg = malloc(sizeof(int) * LV);           /* gbeg[i*V + w]  = group start (rank)    */
    int *gend = malloc(sizeof(int) * LV);           /* gend[i*V + w]  = group end   (rank)    */
    Pair *pr = malloc(sizeof(Pair) * V);
    for (int i = 0; i < L; i++) {
        for (int w = 0; w < V; w++) {
            int ch = words[(size_t)w * (L + 1) + i] - 'a' + 1;
            pr[w].key = full[w] - (uint64_t)ch * pw[i];
            pr[w].idx = w;
        }
        qsort(pr, V, sizeof(Pair), cmp_pair);
        int k = 0;
        while (k < V) {
            int e = k;
            while (e < V && pr[e].key == pr[k].key) e++;
            for (int t = k; t < e; t++) {
                order[(size_t)i * V + t] = pr[t].idx;
                gbeg[(size_t)i * V + pr[t].idx] = k;
                gend[(size_t)i * V + pr[t].idx] = e;
            }
            k = e;
        }
    }

    /* bidirectional BFS: side 0 from A, side 1 from B */
    int *dist[2], *par[2], *cur[2], *nxt;
    char *gvis[2];
    int fs[2];
    for (int s = 0; s < 2; s++) {
        dist[s] = malloc(sizeof(int) * V);
        par[s] = malloc(sizeof(int) * V);
        cur[s] = malloc(sizeof(int) * V);
        gvis[s] = calloc(LV, 1);
        for (int w = 0; w < V; w++) { dist[s][w] = -1; par[s][w] = -1; }
    }
    nxt = malloc(sizeof(int) * V);
    dist[0][ia] = 0; cur[0][0] = ia; fs[0] = 1;
    dist[1][ib] = 0; cur[1][0] = ib; fs[1] = 1;

    int best = -1, bu = -1, bv = -1, bs = 0;
    while (fs[0] > 0 && fs[1] > 0 && best < 0) {
        int s = (fs[0] <= fs[1]) ? 0 : 1;
        int nn = 0;
        for (int x = 0; x < fs[s]; x++) {
            int u = cur[s][x];
            for (int i = 0; i < L; i++) {
                int b = gbeg[(size_t)i * V + u], e = gend[(size_t)i * V + u];
                if (gvis[s][(size_t)i * V + b]) continue;
                gvis[s][(size_t)i * V + b] = 1;
                for (int k = b; k < e; k++) {
                    int v = order[(size_t)i * V + k];
                    if (v == u) continue;
                    if (dist[s][v] < 0) {
                        dist[s][v] = dist[s][u] + 1;
                        par[s][v] = u;
                        nxt[nn++] = v;
                    }
                    if (dist[1 - s][v] >= 0) {
                        int cand = dist[s][u] + 1 + dist[1 - s][v];
                        if (best < 0 || cand < best) { best = cand; bu = u; bv = v; bs = s; }
                    }
                }
            }
        }
        memcpy(cur[s], nxt, sizeof(int) * nn);
        fs[s] = nn;
    }

    if (best < 0) {
        printf("Minimum mutations k = -1\n(no valid transformation sequence exists)\n");
    } else {
        int f = (bs == 0) ? bu : bv;                /* node on A's side of the meeting edge */
        int g = (bs == 0) ? bv : bu;                /* node on B's side                     */
        int *path = malloc(sizeof(int) * (best + 2));
        int len = 0;
        for (int x = f; x != -1; x = par[0][x]) path[len++] = x;
        for (int i = 0, j = len - 1; i < j; i++, j--) { int t = path[i]; path[i] = path[j]; path[j] = t; }
        for (int x = g; x != -1; x = par[1][x]) path[len++] = x;
        printf("Minimum mutations k = %d\nPath: ", best);
        for (int i = 0; i < len; i++)
            printf("%s%s", words + (size_t)path[i] * (L + 1), i + 1 < len ? " -> " : "\n");
        free(path);
    }
    return 0;
}
