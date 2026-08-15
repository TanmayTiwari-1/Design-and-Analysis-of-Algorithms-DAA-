#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int weighings = 0;

/* returns: -1 if left pan lighter, 0 if equal, +1 if right pan heavier(left lighter is -1... )
   We define: compare sum of weights[lo..mid] vs weights[mid+1..hi] */
int weigh(double weight[], int l1, int r1, int l2, int r2) {
    weighings++;
    double sum1 = 0, sum2 = 0;
    for (int i = l1; i <= r1; i++) sum1 += weight[i];
    for (int i = l2; i <= r2; i++) sum2 += weight[i];
    if (sum1 < sum2) return -1;   // left group lighter
    if (sum1 > sum2) return 1;    // right group lighter
    return 0;                     // balanced
}

/* Recursively search for the defective (lighter) coin in weight[lo..hi].
   Returns index of defective coin, or -1 if none found in this range. */
int findLighter(double weight[], int lo, int hi) {
    int size = hi - lo + 1;
    if (size == 1) {
        return lo;  // sole candidate; caller decides how to confirm
    }

    int half = size / 2;
    int mid = lo + half - 1;          // L = [lo, mid]
    int leftover = -1;
    int rEnd = mid + half;            // R = [mid+1, mid+half]

    if (size % 2 == 1) leftover = hi; // odd size: last coin set aside

    int result = weigh(weight, lo, mid, mid + 1, rEnd);

    if (result == 0) {
        // L and R are both genuine
        return (leftover != -1) ? leftover : -1;  // only leftover can be defective
    } else if (result == -1) {
        return findLighter(weight, lo, mid);       // L is lighter
    } else {
        return findLighter(weight, mid + 1, rEnd);  // R is lighter
    }
}

int main(void) {
    int n;
    printf("Enter number of coins: ");
    scanf("%d", &n);
    double *w = malloc(n * sizeof(double));
    printf("Enter %d coin weights: ", n);
    for (int i = 0; i < n; i++) scanf("%lf", &w[i]);

    int candidate = findLighter(w, 0, n - 1);

    // Final confirmation: candidate is defective only if it is strictly
    // lighter than any other (genuine) coin. Compare it once against
    // any other coin in the array (guaranteed genuine after elimination).
    int isDefective = 0;
    if (candidate != -1) {
        for (int i = 0; i < n; i++) {
            if (i == candidate) continue;
            weighings++;
            if (w[candidate] < w[i]) { isDefective = 1; }
            break; // one confirming weighing is enough
        }
    }

    if (isDefective)
        printf("Defective (lighter) coin found at index %d (weight = %.4f)\n", candidate, w[candidate]);
    else
        printf("No defective coin found. All coins have the required weight.\n");

    printf("Total weighings used: %d (n = %d, log2(n) ~= %.2f)\n", weighings, n, log2((double)n));

    free(w);
    return 0;
}
