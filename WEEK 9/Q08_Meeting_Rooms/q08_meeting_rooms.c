/*
 * DAA Lab-09  |  Q8 : Minimum number of meeting rooms
 * ---------------------------------------------------------------------------
 * Intervals are half-open [s, e): a meeting ending at time t and another
 * starting at t can share a room.
 *
 * METHOD A (count) - sort starts and ends separately and sweep with two
 *   pointers: a start before the next end opens a room (+1), otherwise an end
 *   frees one (-1).  The maximum of the running count is the answer = the
 *   maximum number of simultaneously active meetings (depth of the interval
 *   graph, which is a lower bound; the sweep also shows it is achievable).
 *
 * METHOD B (assignment) - process meetings by start time with a min-heap of
 *   (end time, room).  If the earliest-finishing room is free, reuse it,
 *   otherwise open a new room.  Gives an explicit schedule.
 *
 * INPUT   n
 *         s_1 e_1 ... s_n e_n
 * USAGE   ./q08 < sample_input.txt   |   ./q08 --test
 * COMPLEXITY  A: two sorts O(n log n) + linear sweep.   B: sort + n heap ops
 *             O(log n) each.  => O(n log n) time, O(n) space.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct { int s, e, id; } Meet;
typedef struct { int end, room; } HE;

static int cmp_int(const void *a, const void *b) { return *(const int *)a - *(const int *)b; }
static int cmp_meet(const void *a, const void *b)
{
    const Meet *x = a, *y = b;
    if (x->s != y->s) return x->s - y->s;
    return x->e - y->e;
}

static int rooms_sweep(const Meet *m, int n)
{
    int *S = malloc(n * sizeof(int)), *E = malloc(n * sizeof(int));
    for (int i = 0; i < n; i++) { S[i] = m[i].s; E[i] = m[i].e; }
    qsort(S, n, sizeof(int), cmp_int);
    qsort(E, n, sizeof(int), cmp_int);
    int i = 0, j = 0, cur = 0, best = 0;
    while (i < n) {
        if (S[i] < E[j]) { cur++; i++; if (cur > best) best = cur; }
        else { cur--; j++; }
    }
    free(S); free(E);
    return best;
}

static HE *hp;
static int hn;
static int hless(HE a, HE b) { return a.end != b.end ? a.end < b.end : a.room < b.room; }
static void hpush(HE x)
{
    int i = hn++;
    hp[i] = x;
    while (i > 0) {
        int p = (i - 1) / 2;
        if (!hless(hp[i], hp[p])) break;
        HE t = hp[i]; hp[i] = hp[p]; hp[p] = t; i = p;
    }
}
static HE hpop(void)
{
    HE r = hp[0];
    hp[0] = hp[--hn];
    int i = 0;
    for (;;) {
        int l = 2 * i + 1, rr = l + 1, s = i;
        if (l < hn && hless(hp[l], hp[s])) s = l;
        if (rr < hn && hless(hp[rr], hp[s])) s = rr;
        if (s == i) break;
        HE t = hp[i]; hp[i] = hp[s]; hp[s] = t; i = s;
    }
    return r;
}

/* returns #rooms, fills room_of[original id] */
static int rooms_assign(const Meet *in, int n, int *room_of)
{
    Meet *m = malloc(n * sizeof(Meet));
    memcpy(m, in, n * sizeof(Meet));
    qsort(m, n, sizeof(Meet), cmp_meet);
    hp = malloc((n + 1) * sizeof(HE));
    hn = 0;
    int rooms = 0;
    for (int i = 0; i < n; i++) {
        int r;
        if (hn > 0 && hp[0].end <= m[i].s) r = hpop().room;
        else r = rooms++;
        room_of[m[i].id] = r;
        hpush((HE){ m[i].e, r });
    }
    free(m); free(hp);
    return rooms;
}

static int assignment_valid(const Meet *m, int n, const int *room_of)
{
    for (int i = 0; i < n; i++)
        for (int j = i + 1; j < n; j++)
            if (room_of[i] == room_of[j] && m[i].s < m[j].e && m[j].s < m[i].e) return 0;
    return 1;
}

static int brute(const Meet *m, int n)      /* max overlap measured at every start time */
{
    int best = 0;
    for (int i = 0; i < n; i++) {
        int c = 0;
        for (int j = 0; j < n; j++) if (m[j].s <= m[i].s && m[i].s < m[j].e) c++;
        if (c > best) best = c;
    }
    return best;
}

static int run_tests(void)
{
    srand(2026);
    int fails = 0, T = 3000;
    for (int t = 1; t <= T; t++) {
        int n = 1 + rand() % 12, room_of[16];
        Meet m[16];
        for (int i = 0; i < n; i++) { m[i].s = rand() % 20; m[i].e = m[i].s + 1 + rand() % 8; m[i].id = i; }
        int a = rooms_sweep(m, n), b = rooms_assign(m, n, room_of), c = brute(m, n);
        if (a != c || b != c || !assignment_valid(m, n, room_of)) { fails++; printf("FAIL test %d: sweep=%d heap=%d brute=%d\n", t, a, b, c); }
    }
    printf("Random tests : %d (n = 1..12 meetings, touching end/start allowed)\n", T);
    printf("Checks       : sweep == heap assignment == max overlap; no two meetings in one room overlap\n");
    printf("Failures     : %d\n", fails);
    printf("%s\n", fails ? "RESULT: SOME TESTS FAILED" : "RESULT: ALL TESTS PASSED");
    return fails != 0;
}

int main(int argc, char **argv)
{
    if (argc > 1 && !strcmp(argv[1], "--test")) return run_tests();
    int n;
    if (scanf("%d", &n) != 1 || n < 1) { fprintf(stderr, "bad input\n"); return 1; }
    Meet *m = malloc(n * sizeof(Meet));
    int *room_of = malloc(n * sizeof(int));
    for (int i = 0; i < n; i++) {
        if (scanf("%d %d", &m[i].s, &m[i].e) != 2 || m[i].e <= m[i].s) { fprintf(stderr, "bad interval %d\n", i + 1); return 1; }
        m[i].id = i;
    }
    int a = rooms_sweep(m, n), b = rooms_assign(m, n, room_of);
    printf("Meetings: %d\n", n);
    printf("Minimum rooms (two-pointer sweep) = %d\n", a);
    printf("Minimum rooms (min-heap schedule) = %d\n", b);
    printf("Max overlap check                 = %d\n", brute(m, n));
    printf("Schedule:\n");
    for (int r = 0; r < b; r++) {
        printf("  Room %d:", r + 1);
        for (int i = 0; i < n; i++) if (room_of[i] == r) printf(" [%d,%d)", m[i].s, m[i].e);
        printf("\n");
    }
    printf("Schedule valid = %s\n", assignment_valid(m, n, room_of) ? "yes" : "NO");
    free(m); free(room_of);
    return 0;
}
