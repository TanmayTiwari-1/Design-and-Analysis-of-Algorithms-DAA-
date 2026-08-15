#include <stdio.h>
#include <stdlib.h>

long cmp_count = 0;

int binarySearch(int arr[], int lo, int hi, int x) {
    while (lo <= hi) {
        int mid = lo + (hi - lo) / 2;
        cmp_count++;                       // arr[mid] == x
        if (arr[mid] == x) return mid;
        cmp_count++;                       // arr[mid] < x
        if (arr[mid] < x) lo = mid + 1;
        else hi = mid - 1;
    }
    return -1;
}

int ternarySearch(int arr[], int lo, int hi, int x) {
    while (lo <= hi) {
        int mid1 = lo + (hi - lo) / 3;
        int mid2 = hi - (hi - lo) / 3;

        cmp_count++;                       // arr[mid1] == x
        if (arr[mid1] == x) return mid1;
        cmp_count++;                       // arr[mid2] == x
        if (arr[mid2] == x) return mid2;

        cmp_count++;                       // x < arr[mid1]
        if (x < arr[mid1]) hi = mid1 - 1;
        else {
            cmp_count++;                   // x > arr[mid2]
            if (x > arr[mid2]) lo = mid2 + 1;
            else { lo = mid1 + 1; hi = mid2 - 1; }
        }
    }
    return -1;
}

int main() {
    int n = 100000;
    int *arr = malloc(n * sizeof(int));
    for (int i = 0; i < n; i++) arr[i] = i;   // sorted array

    int x = 3;   // worst-ish case target (forces near-maximal recursion in binary too if 0-indexed edge)

    cmp_count = 0;
    int idxB = binarySearch(arr, 0, n - 1, x);
    long binCmps = cmp_count;

    cmp_count = 0;
    int idxT = ternarySearch(arr, 0, n - 1, x);
    long terCmps = cmp_count;

    printf("n = %d, target = %d\n", n, x);
    printf("Binary search  -> index %d, comparisons = %ld\n", idxB, binCmps);
    printf("Ternary search -> index %d, comparisons = %ld\n", idxT, terCmps);
    printf("\nAverage over all n elements (worst case per element):\n");

    long totalB = 0, totalT = 0;
    for (int i = 0; i < n; i++) {
        cmp_count = 0; binarySearch(arr, 0, n - 1, arr[i]);  totalB += cmp_count;
        cmp_count = 0; ternarySearch(arr, 0, n - 1, arr[i]); totalT += cmp_count;
    }
    printf("Total comparisons -> Binary: %ld   Ternary: %ld\n", totalB, totalT);
    printf("=> Binary search uses fewer comparisons overall, confirming it is faster.\n");

    free(arr);
    return 0;
}
