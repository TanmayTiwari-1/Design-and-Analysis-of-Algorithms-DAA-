/*
 * Q1: Invert the Coin-Triangle
 * -----------------------------
 * A triangle of n rows contains T = n(n+1)/2 coins, packed on a triangular
 * lattice.  We must find the MINIMUM number of coins that need to be slid
 * to new lattice positions so that the (upward pointing) triangle becomes
 * a downward pointing triangle.
 *
 * INPUT REPRESENTATION
 * ---------------------
 * Every coin in row r (0-indexed, r = 0..n-1) and column c (0..r) is given
 * INTEGER lattice coordinates (X, R):
 *        X = 2*c - r        R = r
 * This doubles the horizontal axis so that all coin centers become
 * integer points (avoids floating point comparisons for the sqrt(3)/2
 * vertical spacing, which only ever appears as a common scale factor).
 *
 * ALGORITHM
 * ---------
 * Flipping the triangle 180 degrees corresponds to the point
 * transformation  (X, R) -> (A - X, B - R)  for the correct pair of
 * integer constants (A, B) that will re-align the rotated triangle with
 * the triangular lattice such that the OVERLAP with the original triangle
 * is as large as possible (i.e. the fewest coins need to be moved: they
 * are moved from the "leftover" original positions into the "leftover"
 * empty target positions).
 *
 * We brute-force search over all reasonable translations (A,B) and among
 * them pick the one maximising the overlap between the original coin-set
 * and the 180-degree-rotated + translated coin-set.  This is guaranteed
 * to find the true optimum for the sizes used in a lab setting, and
 * independently reproduces the well known closed-form result
 *
 *        M(n) = floor( n(n+1) / 6 ) = floor( T / 3 )
 *
 * COMPLEXITY
 * ----------
 * Let T = n(n+1)/2 be the number of coins.
 *   - Number of candidate translations tried  : O(n^2)
 *   - For every translation we scan all T coins and look each one up in
 *     the coin set                              : O(T) with a hash/array lookup,
 *                                                  O(T) per lookup with the
 *                                                  simple array-scan used here.
 *   Overall (with the simple O(T) "contains" scan used below for clarity):
 *        O(n^2 * T * T)  =  O(n^6)      (brute-force verification)
 *   The CLOSED-FORM formula gives the answer in O(1).
 *   (A hash-set based implementation would bring the search down to
 *    O(n^2 * T) = O(n^4).)
 */

#include <stdio.h>
#include <stdlib.h>

typedef struct { int X, R; } Coin;

static int contains(Coin *arr, int n, int X, int R) {
    for (int i = 0; i < n; i++)
        if (arr[i].X == X && arr[i].R == R) return 1;
    return 0;
}

int main(int argc, char **argv) {
    int n = (argc > 1) ? atoi(argv[1]) : 4;
    if (n < 2) { printf("Please use n >= 2\n"); return 0; }

    int total = n * (n + 1) / 2;
    Coin *orig = malloc(sizeof(Coin) * total);

    int idx = 0;
    for (int r = 0; r < n; r++)
        for (int c = 0; c <= r; c++) {
            orig[idx].X = 2 * c - r;
            orig[idx].R = r;
            idx++;
        }

    int bestOverlap = -1, bestA = 0, bestB = 0;
    int range = 2 * n + 2;

    for (int A = -range; A <= range; A++) {
        for (int B = -2 * range; B <= 2 * range; B++) {
            int cnt = 0;
            for (int i = 0; i < total; i++) {
                int rx = B - orig[i].X;
                int rr = A - orig[i].R;
                if (contains(orig, total, rx, rr)) cnt++;
            }
            if (cnt > bestOverlap) { bestOverlap = cnt; bestA = A; bestB = B; }
        }
    }

    int moves = total - bestOverlap;

    printf("================================================\n");
    printf(" Coin-Triangle Inversion  (n = %d rows, %d coins)\n", n, total);
    printf("================================================\n");
    printf("Best alignment translation found : A = %d, B = %d\n", bestA, bestB);
    printf("Coins that MUST move (algorithm) : %d\n", moves);
    printf("Closed-form check floor(T/3)     : %d\n", total / 3);
    printf("Match with formula?              : %s\n",
           (moves == total / 3) ? "YES" : "NO");

    printf("\nMove list  (row,col) -> (targetRow,targetCol):\n");
    int shown = 0;
    for (int i = 0; i < total; i++) {
        int rx = bestB - orig[i].X;
        int rr = bestA - orig[i].R;
        if (!contains(orig, total, rx, rr)) {
            int srcRow = orig[i].R;
            int srcCol = (orig[i].X + srcRow) / 2;
            int dstRow = rr;
            int dstCol = (rx + dstRow) / 2;
            printf("  coin %2d : (%d,%d) -> (%d,%d)\n", shown + 1, srcRow, srcCol, dstRow, dstCol);
            shown++;
        }
    }
    printf("\nTotal coins actually listed as moving : %d\n", shown);

    free(orig);
    return 0;
}
