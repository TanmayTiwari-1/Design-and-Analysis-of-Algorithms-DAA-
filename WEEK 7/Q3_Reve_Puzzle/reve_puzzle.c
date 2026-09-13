/*
 * Q3: Reve's Puzzle (Tower of Hanoi with 4 pegs, n disks)
 * ---------------------------------------------------------
 * INPUT REPRESENTATION
 * ---------------------
 * n = number of disks (1..N), 4 pegs labelled 1,2,3,4.
 * Disks are represented implicitly by size (1 = smallest .. n = largest);
 * we never need to store peg contents explicitly to COUNT moves, but we do
 * maintain peg stacks (arrays) to VALIDATE every move (never place a
 * larger disk on a smaller one) while generating the full move list.
 *
 * ALGORITHM  (Frame-Stewart algorithm)
 * --------------------------------------
 * Let H3(m) = 2^m - 1  = minimum moves to solve the classic 3-peg Hanoi.
 * Let R4(n) = minimum moves to solve the 4-peg version.
 *   R4(0) = 0
 *   R4(n) = min over 1<=k<n of  [ 2*R4(k) + H3(n-k) ]
 * Strategy for the optimal k:
 *   1. Move the top k disks from the source peg to a spare peg, using
 *      all 4 pegs                                    -> R4(k) moves
 *   2. Move the remaining (n-k) (larger) disks from source to destination
 *      using ordinary 3-peg Hanoi (the peg holding the k small disks is
 *      "busy" and cannot be used)                      -> H3(n-k) moves
 *   3. Move the k disks from the spare peg to the destination peg, again
 *      using all 4 pegs                                -> R4(k) moves
 * The optimal split k(n) is found by DP; for n = 8 the optimum gives
 * exactly 33 moves (the value quoted in the puzzle).
 *
 * COMPLEXITY
 * ----------
 *  Building the DP table R4[0..n] and optimal-k table : O(n^2) time,
 *                                                        O(n) space.
 *  Generating & printing/validating the actual move list: O(R4(n)) moves,
 *  and 3-peg sub-Hanoi recursion is the usual O(2^m).
 */

#include <stdio.h>
#include <stdlib.h>

long long moveCount = 0;
int stackTop[5];      /* index of top disk (0 = empty) for pegs 1..4     */
int pegDisk[5][100];  /* pegDisk[p][i] = size of i-th disk from bottom   */
int verbose = 0;

void doMove(int from, int to) {
    int disk = pegDisk[from][stackTop[from]];
    /* validation */
    if (stackTop[to] > 0 && pegDisk[to][stackTop[to]] < disk) {
        printf("ERROR: illegal move! disk %d onto smaller disk at peg %d\n", disk, to);
        exit(1);
    }
    stackTop[from]--;
    stackTop[to]++;
    pegDisk[to][stackTop[to]] = disk;
    moveCount++;
    if (verbose) printf("Move disk %d : peg %d -> peg %d\n", disk, from, to);
}

/* classic 3-peg hanoi, using pegs {from,to,via}, moving n smallest disks
   currently sitting on top of peg 'from' */
void hanoi3(int n, int from, int to, int via) {
    if (n == 0) return;
    hanoi3(n - 1, from, via, to);
    doMove(from, to);
    hanoi3(n - 1, via, to, from);
}

long long R4[100];
int optK[100];

void frameStewart(int n, int from, int to, int aux1, int aux2) {
    if (n == 0) return;
    if (n == 1) { doMove(from, to); return; }
    int k = optK[n];
    frameStewart(k, from, aux1, to, aux2);      /* move top k disks out of the way using 4 pegs */
    hanoi3(n - k, from, to, aux2);              /* move remaining n-k disks with 3 pegs         */
    frameStewart(k, aux1, to, from, aux2);      /* bring the k disks back onto the destination  */
}

int main(int argc, char **argv) {
    int n = (argc > 1) ? atoi(argv[1]) : 8;
    verbose = (argc > 2) ? atoi(argv[2]) : 0;

    if (n < 1 || n > 60) { printf("Use 1 <= n <= 60\n"); return 0; }

    /* --- DP: compute R4[n] and optimal split k --- */
    R4[0] = 0;
    R4[1] = 1;      /* base case: one disk needs exactly one move */
    optK[1] = 1;
    for (int m = 2; m <= n; m++) {
        long long best = -1; int bestk = 1;
        for (int k = 1; k < m; k++) {
            long long h3 = (1LL << (m - k)) - 1;
            long long cost = 2 * R4[k] + h3;
            if (best == -1 || cost < best) { best = cost; bestk = k; }
        }
        R4[m] = best;
        optK[m] = bestk;
    }

    printf("=========================================\n");
    printf(" Reve's Puzzle : 4 pegs, %d disks (Frame-Stewart)\n", n);
    printf("=========================================\n");
    printf("DP table of minimum moves R4(m) for m = 1..%d:\n", n);
    for (int m = 1; m <= n; m++)
        printf("  R4(%2d) = %-4lld  (optimal split k = %d)\n", m, R4[m], optK[m]);

    /* --- simulate/generate the actual move sequence and validate it --- */
    for (int p = 1; p <= 4; p++) stackTop[p] = 0;
    for (int d = n; d >= 1; d--) { stackTop[1]++; pegDisk[1][stackTop[1]] = d; }

    moveCount = 0;
    frameStewart(n, 1, 4, 2, 3);

    printf("\nSimulated/validated total moves for n = %d : %lld\n", n, moveCount);
    printf("Matches DP prediction R4(%d)               : %s\n",
           n, (moveCount == R4[n]) ? "YES" : "NO");

    if (n == 8) {
        printf("\n(Classic instance check: 8 disks on 4 pegs should take\n");
        printf(" exactly 33 moves, as stated in the puzzle.)\n");
    }

    /* verify final state: all disks on peg 4 in correct order */
    int ok = (stackTop[4] == n);
    for (int i = 1; i <= n && ok; i++) if (pegDisk[4][i] != n - i + 1) ok = 0;
    printf("Final configuration correct (all disks on peg 4) : %s\n", ok ? "YES" : "NO");

    return 0;
}
