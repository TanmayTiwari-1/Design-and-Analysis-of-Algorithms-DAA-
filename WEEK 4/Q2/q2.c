/* Q2: Given sets S1, S2 (each size n) and a number x, find whether some
   a in S1 and b in S2 exist with a + b == x, in O(n log n).

   Idea: We need b = x - a for some a in S1. So:
     1. Sort S2.                                   -> O(n log n)
     2. For every a in S1, binary-search for (x-a) in sorted S2. -> O(log n) each
   Total: O(n log n) + O(n log n) = O(n log n).
*/
#include <stdio.h>
#include <stdlib.h>

int cmp(const void *a, const void *b) { return (*(int*)a - *(int*)b); }

/* standard binary search, returns 1 if target found in arr[0..n-1] */
int binarySearch(int arr[], int n, int target) {
    int lo = 0, hi = n - 1;
    while (lo <= hi) {
        int mid = lo + (hi - lo) / 2;
        if (arr[mid] == target) return 1;
        if (arr[mid] < target) lo = mid + 1;
        else hi = mid - 1;
    }
    return 0;
}

/* Returns 1 and sets *foundA/*foundB if a pair is found, else 0. */
int findPairSum(int S1[], int S2[], int n, int x, int *foundA, int *foundB) {
    int *sortedS2 = malloc(n * sizeof(int));
    for (int i = 0; i < n; i++) sortedS2[i] = S2[i];
    qsort(sortedS2, n, sizeof(int), cmp);

    for (int i = 0; i < n; i++) {
        int need = x - S1[i];
        if (binarySearch(sortedS2, n, need)) {
            *foundA = S1[i];
            *foundB = need;
            free(sortedS2);
            return 1;
        }
    }
    free(sortedS2);
    return 0;
}

int main(void) {
    int n, x;
    printf("Enter n (size of each set): ");
    scanf("%d", &n);

    int *S1 = malloc(n * sizeof(int));
    int *S2 = malloc(n * sizeof(int));

    printf("Enter %d elements of S1: ", n);
    for (int i = 0; i < n; i++) scanf("%d", &S1[i]);
    printf("Enter %d elements of S2: ", n);
    for (int i = 0; i < n; i++) scanf("%d", &S2[i]);
    printf("Enter target sum x: ");
    scanf("%d", &x);

    int a, b;
    if (findPairSum(S1, S2, n, x, &a, &b))
        printf("Pair found: %d (from S1) + %d (from S2) = %d\n", a, b, x);
    else
        printf("No pair from S1 and S2 adds up to %d\n", x);

    free(S1); free(S2);
    return 0;
}
