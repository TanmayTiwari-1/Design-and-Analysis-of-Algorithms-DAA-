/*
 * Q2: Super Egg Testing Experiment (Generalized Egg Drop)
 * ---------------------------------------------------------
 * Given E identical eggs and an F-storey building, find the MINIMUM number
 * of trials that GUARANTEES (worst case) finding the highest floor from
 * which an egg can be dropped without breaking (0 means it breaks even
 * from floor 1, i.e. the safe floor could be 0).
 *
 * INPUT REPRESENTATION
 * ---------------------
 * Two integers E (number of eggs) and F (number of floors).
 *
 * ALGORITHM  (Dynamic Programming)
 * ---------------------------------
 * Classic formulation:  dp[e][f] = minimum number of trials needed with
 * e eggs and f floors.
 *      dp[e][f] = 1 + min_{1<=k<=f} max( dp[e-1][k-1] , dp[e][f-k] )
 * This is O(E*F^2).
 *
 * FASTER DP used here -> "floors distinguishable by t trials & e eggs":
 *      best[t][e] = max number of floors that can be resolved
 *                    with e eggs and t trials
 *      best[t][e] = best[t-1][e-1] + best[t-1][e] + 1
 *        (drop on some floor: if it breaks -> use (e-1) eggs, (t-1)
 *         trials below;  if it survives -> use e eggs, (t-1) trials above;
 *         plus the floor just tested)
 *      base cases: best[0][e] = 0 for all e ; best[t][0] = 0 for all t
 * We increase t until best[t][E] >= F -- that minimal t is the answer.
 *
 * COMPLEXITY
 * ----------
 *   Time  : O(E * F)   (t only needs to go up to O(F) in the worst case,
 *                        and for each t we do O(E) work)
 *   Space : O(E * F) for the DP table (can be reduced to O(E) with a
 *           rolling array).
 */

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {
    int E = (argc > 1) ? atoi(argv[1]) : 2;   /* eggs   */
    int F = (argc > 2) ? atoi(argv[2]) : 100; /* floors */

    if (E < 1 || F < 1) { printf("E and F must be >= 1\n"); return 0; }

    /* best[t][e] = max floors distinguishable with e eggs, t trials     */
    int maxT = F + 1; /* upper bound: with 1 egg you need F trials anyway */
    int **best = malloc((maxT + 1) * sizeof(int *));
    for (int t = 0; t <= maxT; t++) {
        best[t] = calloc(E + 1, sizeof(int));
    }

    int answer = -1;
    for (int t = 1; t <= maxT && answer == -1; t++) {
        for (int e = 1; e <= E; e++) {
            best[t][e] = best[t - 1][e - 1] + best[t - 1][e] + 1;
        }
        if (best[t][E] >= F) answer = t;
    }

    printf("=========================================\n");
    printf(" Generalized Egg Drop  (E = %d eggs, F = %d floors)\n", E, F);
    printf("=========================================\n");
    printf("Minimum number of trials guaranteed to find the\n");
    printf("critical floor in the worst case : %d\n\n", answer);

    printf("Table: max floors resolvable with e eggs, t trials\n");
    printf("t \\ e :");
    for (int e = 1; e <= E; e++) printf("%6d", e);
    printf("\n");
    for (int t = 1; t <= answer; t++) {
        printf("%5d :", t);
        for (int e = 1; e <= E; e++) printf("%6d", best[t][e]);
        printf("\n");
    }

    /* Special call-out for the classic instance E=2 */
    if (E == 2) {
        printf("\n(Classic instance check: with 2 eggs and 100 floors,\n");
        printf(" the well-known textbook answer is 14 trials.)\n");
    }

    for (int t = 0; t <= maxT; t++) free(best[t]);
    free(best);
    return 0;
}
