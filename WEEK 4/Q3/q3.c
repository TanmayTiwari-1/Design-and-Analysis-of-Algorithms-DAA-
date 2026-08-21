/* Q3: Given set S of n integers and integer T, test whether k of the
   integers add up to T, in O(n^(k-1) * log n).

   Idea (generalises the classic "2-sum" and "3-sum" tricks):
     1. Sort S.                                       -> O(n log n)
     2. Choose the first (k-2) numbers by brute-force nested loops
        (all combinations of k-2 positions) -> O(n^(k-2)) combinations.
     3. For each such choice, let R = T - (sum of the chosen k-2 numbers).
        Find TWO more numbers (from the remaining part of the sorted
        array) that add up to R, using the classic two-pointer scan
        (one pointer from the low end, one from the high end) -> O(n).

   Total = O(n log n) [sorting] + O(n^(k-2)) * O(n) [two-pointer scan]
         = O(n^(k-1))  (the sorting term is dominated for k >= 3).
   Adding the log n factor as an upper bound (as the question allows)
   gives O(n^(k-1) * log n).

   Implementation: pick the first (k-2) indices recursively (i_1 < i_2 <
   ... < i_{k-2}), then run the two-pointer step on the remaining range.
*/
#include <stdio.h>
#include <stdlib.h>

int cmp(const void *a, const void *b) { return (*(int*)a - *(int*)b); }

int n, k;
int *arr;
int *chosenIdx;   // indices of the (k-2) fixed numbers
int foundCombo[32];
int found = 0;

/* Two-pointer search for a pair summing to 'target' using only indices
   in the RANGE [lo, hi] of the sorted array (both pointers start at the
   ends of that range and move inward). O(hi-lo). */
int twoPointerFind(int target, int lo, int hi, int *outI, int *outJ) {
    while (lo < hi) {
        int s = arr[lo] + arr[hi];
        if (s == target) { *outI = lo; *outJ = hi; return 1; }
        if (s < target) lo++;
        else hi--;
    }
    return 0;
}

/* Recursively choose k-2 fixed indices (strictly increasing, to avoid
   re-using the same element / duplicate combos), then two-pointer the rest. */
int chooseFixed(int startIdx, int chosenCount, long sumSoFar, int T) {
    if (chosenCount == k - 2) {
        // remaining two numbers must come from indices AFTER the last chosen one
        int lo = startIdx, hi = n - 1;
        int i, j;
        if (twoPointerFind(T - sumSoFar, lo, hi, &i, &j)) {
            for (int t = 0; t < chosenCount; t++) foundCombo[t] = chosenIdx[t];
            foundCombo[chosenCount] = i;
            foundCombo[chosenCount + 1] = j;
            found = 1;
            return 1;
        }
        return 0;
    }
    for (int i = startIdx; i < n; i++) {
        chosenIdx[chosenCount] = i;
        if (chooseFixed(i + 1, chosenCount + 1, sumSoFar + arr[i], T)) return 1;
    }
    return 0;
}

int main(void) {
    int T;
    printf("Enter n (size of set): ");
    scanf("%d", &n);
    arr = malloc(n * sizeof(int));
    printf("Enter %d integers: ", n);
    for (int i = 0; i < n; i++) scanf("%d", &arr[i]);
    printf("Enter k (how many numbers should sum up): ");
    scanf("%d", &k);
    printf("Enter target sum T: ");
    scanf("%d", &T);

    qsort(arr, n, sizeof(int), cmp);
    chosenIdx = malloc(k * sizeof(int));

    if (k < 2) {
        printf("k must be >= 2\n");
    } else if (k == 2) {
        int i, j;
        if (twoPointerFind(T, 0, n - 1, &i, &j))
            printf("Found: %d + %d = %d\n", arr[i], arr[j], T);
        else
            printf("No %d numbers add up to %d\n", k, T);
    } else {
        chooseFixed(0, 0, 0, T);
        if (found) {
            printf("Found %d numbers that sum to %d: ", k, T);
            for (int t = 0; t < k; t++) printf("%d ", arr[foundCombo[t]]);
            printf("\n");
        } else {
            printf("No %d numbers add up to %d\n", k, T);
        }
    }

    free(arr); free(chosenIdx);
    return 0;
}
