/*
 * Q5: Hitting a Moving Target
 * -----------------------------
 * n hiding spots in a line, numbered 1..n.  The target is at some unknown
 * spot; it is invisible.  Between every two consecutive shots the target
 * MUST move to an adjacent spot (spot i -> i-1 or i+1, staying in [1,n]).
 * We fire one shot per round at a chosen spot.  Question: does an
 * algorithm exist that GUARANTEES a hit eventually, and if so what is it?
 *
 * ANSWER: YES, such an algorithm exists (for n >= 2), based on a PARITY
 * argument:
 *   - The target's position parity (odd/even) flips after every move.
 *   - We do not know its initial position OR its initial parity, so we
 *     need two passes to cover both possibilities:
 *         Pass 1 (increasing): shoot spots 2,3,4,...,n-1
 *         Pass 2 (decreasing): shoot spots n-1,n-2,...,2
 *   - Total shots = 2*(n-2).
 *   (For n = 2 the puzzle is trivial: alternately shoot 1,2,1,2,... )
 *
 * WHY IT WORKS / HOW WE VERIFY IT
 * ---------------------------------
 * Rather than trust the parity argument on faith, the program VERIFIES it
 * constructively: it tracks S = the set of spots the target could
 * possibly occupy on the current day, GIVEN that it has evaded every
 * shot fired so far (i.e. it started somewhere in {1..n} and always
 * chooses a move that avoids being caught, in the worst adversarial
 * case).  Update rule per round:
 *      1. remove the shot spot from S               (can't be there)
 *      2. if S becomes empty -> every possible evading walk has been
 *         eliminated => a hit is GUARANTEED by this round.
 *      3. otherwise the target moves: S <- {neighbours of elements of S}
 * If S is driven to empty by the end of the fixed shooting sequence
 * above, the algorithm is proven correct for that n.
 *
 * INPUT REPRESENTATION
 * ---------------------
 * n = number of hiding spots.  S is stored as a boolean array of size
 * n+1 (1-indexed).
 *
 * COMPLEXITY
 * ----------
 *   Shots fired : 2*(n-2)  =  O(n)
 *   Work per shot (update S) : O(n)
 *   Total time  : O(n^2)     Space : O(n)
 */

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {
    int n = (argc > 1) ? atoi(argv[1]) : 10;
    if (n < 2) { printf("Use n >= 2\n"); return 0; }

    printf("=========================================\n");
    printf(" Hitting a Moving Target  (n = %d hiding spots)\n", n);
    printf("=========================================\n");

    if (n == 2) {
        printf("Trivial case n=2: shooting sequence 1,2,1,2,... guarantees\n");
        printf("a hit within at most 2 shots (only two spots exist).\n");
        return 0;
    }

    int len = 2 * (n - 2);
    int *seq = malloc(sizeof(int) * len);
    int k = 0;
    for (int p = 2; p <= n - 1; p++) seq[k++] = p;   /* pass 1: increasing */
    for (int p = n - 1; p >= 2; p--) seq[k++] = p;   /* pass 2: decreasing */

    printf("Shooting sequence used (length %d) : ", len);
    for (int i = 0; i < len; i++) printf("%d ", seq[i]);
    printf("\n\nExistence proof / verification by tracking the set S of\n");
    printf("spots consistent with the target having evaded every shot:\n\n");

    int *S = malloc(sizeof(int) * (n + 1));
    for (int i = 1; i <= n; i++) S[i] = 1;

    int guaranteedAt = -1;
    for (int t = 0; t < len; t++) {
        int shot = seq[t];
        S[shot] = 0;

        int remaining = 0;
        for (int i = 1; i <= n; i++) if (S[i]) remaining++;

        printf("Shot %2d (spot %2d) -> possible-evader-spots remaining: %d\n",
               t + 1, shot, remaining);

        if (remaining == 0) { guaranteedAt = t + 1; break; }

        int *newS = calloc(n + 1, sizeof(int));
        for (int i = 1; i <= n; i++) {
            if (S[i]) {
                if (i - 1 >= 1) newS[i - 1] = 1;
                if (i + 1 <= n) newS[i + 1] = 1;
            }
        }
        free(S);
        S = newS;
    }

    printf("\nResult: ");
    if (guaranteedAt != -1)
        printf("guaranteed hit by shot #%d out of %d fired (algorithm CONFIRMED correct for n=%d).\n",
               guaranteedAt, len, n);
    else
        printf("possibility set never emptied -- sequence failed for n=%d (should not happen for n>=3).\n", n);

    free(S);
    free(seq);
    return 0;
}
