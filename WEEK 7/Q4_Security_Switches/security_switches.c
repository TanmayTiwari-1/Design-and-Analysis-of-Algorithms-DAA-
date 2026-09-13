/*
 * Q4: Security Switches Puzzle  (a.k.a. the "Chinese Rings" / Baguenaudier
 * puzzle in disguise)
 * ---------------------------------------------------------------------
 * n switches, numbered 1 (rightmost) .. n (leftmost).  Rules:
 *   - switch 1 (rightmost) can always be toggled.
 *   - switch i (i>1) can be toggled iff switch (i-1) is ON and every
 *     switch to the right of (i-1), i.e. switches 1..i-2, are OFF.
 * All switches start ON; goal: turn them all OFF in the minimum number
 * of single-switch toggles.
 *
 * INPUT REPRESENTATION
 * ---------------------
 * The global state is an n-bit integer "mask": bit (i-1) = 1 means
 * switch i is ON.  All 2^n states therefore fit into a single machine
 * word for the n used in a lab (n <= ~22 is instantaneous).
 *
 * ALGORITHM  (Breadth-First Search on the state graph)
 * -------------------------------------------------------
 * Nodes  = the 2^n on/off configurations.
 * Edges  = legal single-switch toggles (as defined by the rule above).
 * We run a BFS from the all-ON state (2^n - 1) to the all-OFF state (0);
 * BFS on an unweighted graph always finds the SHORTEST path, i.e. the
 * minimum number of moves -- this is an exact, provably optimal answer
 * (not just a formula lookup).
 *
 * The BFS path length is then compared against the well known closed
 * form for this puzzle (identical in structure to the Chinese Rings
 * puzzle):
 *        M(n) = floor( 2^(n+1) / 3 )
 *
 * COMPLEXITY
 * ----------
 *   States  : 2^n
 *   Edges checked per state : O(n)
 *   Time  : O(n * 2^n)
 *   Space : O(2^n)          (visited[] + parent[] arrays)
 * This is exact/exponential, appropriate for a combinatorial state-space
 * search; for large n one would use the closed-form formula O(1) instead
 * (also printed below for comparison).
 */

#include <stdio.h>
#include <stdlib.h>

int n;

/* Is toggling switch i (1-indexed) legal in state 'mask'? */
int legal(int mask, int i) {
    if (i == 1) return 1;
    int rightBit = (mask >> (i - 2)) & 1;      /* switch i-1 */
    if (!rightBit) return 0;
    int belowMask = mask & ((1 << (i - 2)) - 1); /* switches 1..i-2 */
    return (belowMask == 0);
}

int main(int argc, char **argv) {
    n = (argc > 1) ? atoi(argv[1]) : 4;
    if (n < 1 || n > 22) { printf("Use 1 <= n <= 22\n"); return 0; }

    int N = 1 << n;
    int *dist = malloc(sizeof(int) * N);
    int *parentState = malloc(sizeof(int) * N);
    int *parentMove  = malloc(sizeof(int) * N);
    for (int s = 0; s < N; s++) dist[s] = -1;

    int *queue = malloc(sizeof(int) * N);
    int qh = 0, qt = 0;

    int start = N - 1;   /* all ON  */
    int goal  = 0;        /* all OFF */

    dist[start] = 0;
    queue[qt++] = start;

    while (qh < qt) {
        int cur = queue[qh++];
        if (cur == goal) break;
        for (int i = 1; i <= n; i++) {
            if (legal(cur, i)) {
                int nxt = cur ^ (1 << (i - 1));
                if (dist[nxt] == -1) {
                    dist[nxt] = dist[cur] + 1;
                    parentState[nxt] = cur;
                    parentMove[nxt]  = i;
                    queue[qt++] = nxt;
                }
            }
        }
    }

    printf("=========================================\n");
    printf(" Security Switches Puzzle  (n = %d switches)\n", n);
    printf("=========================================\n");
    printf("Minimum number of toggles (BFS, provably optimal) : %d\n", dist[goal]);

    long long formula = ((1LL << (n + 1)) / 3);
    printf("Closed-form check floor(2^(n+1)/3)                : %lld\n", formula);
    printf("Match?                                            : %s\n",
           ((long long)dist[goal] == formula) ? "YES" : "NO");

    /* reconstruct and print the optimal move sequence */
    printf("\nOptimal move sequence (switch numbers toggled in order):\n");
    int *path = malloc(sizeof(int) * (dist[goal] + 1));
    int cur = goal, cnt = 0;
    while (cur != start) {
        path[cnt++] = parentMove[cur];
        cur = parentState[cur];
    }
    for (int i = cnt - 1; i >= 0; i--) printf("%d ", path[i]);
    printf("\n");

    free(dist); free(parentState); free(parentMove); free(queue); free(path);
    return 0;
}
