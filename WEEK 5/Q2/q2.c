/*
 * DAA LAB-5, Q2
 * Find the K'th smallest element in a list of N numbers WITHOUT sorting.
 *
 * IDEA / ALGORITHM:
 * Exactly the same Quickselect technique as Q1, generalized to any k
 * (1-based rank supplied by the user, converted to 0-based index k-1).
 *
 *   1. Randomly pick a pivot, partition the array so everything smaller
 *      than the pivot is to its left and everything larger to its right.
 *   2. If the pivot lands exactly at index (k-1), it IS the k'th
 *      smallest element -> return it.
 *   3. If (k-1) is smaller than the pivot's position, recurse left.
 *   4. Otherwise recurse right.
 *
 * Only one side of the partition is ever explored, so the array is
 * never completely sorted -- we only do "just enough" work to pin
 * down the k'th smallest value.
 *
 * COMPLEXITY ANALYSIS:
 *   Let T(n) = time for quickselect on n elements.
 *   - Each partition step costs O(n).
 *   - Average Case: pivot roughly halves the search space each time:
 *         T(n) = T(n/2) + O(n)   =>   T(n) = O(n)
 *   - Worst Case: unlucky pivot choice shrinks the array by only 1
 *     element each time (e.g., always picking the min/max):
 *         T(n) = T(n-1) + O(n)   =>   T(n) = O(n^2)
 *     Randomizing the pivot makes this practically never happen.
 *   - Deterministic O(n) worst case is possible with the
 *     "Median of Medians" pivot-selection strategy.
 *   - Space Complexity: O(1) extra space (in place); recursion stack
 *     is O(log n) on average, O(n) in the worst case.
 *
 * SUMMARY:
 *   Average Time : O(n)
 *   Worst Time   : O(n^2)  (O(n) with median-of-medians)
 *   Space        : O(1) extra / O(log n) avg recursion stack
 *
 * (Compare with sorting first: that would cost O(n log n) always,
 *  so quickselect is asymptotically better on average.)
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void swap(int *a, int *b) {
    int t = *a; *a = *b; *b = t;
}

int partition(int arr[], int low, int high) {
    int pivot = arr[high];
    int i = low - 1;
    for (int j = low; j < high; j++) {
        if (arr[j] <= pivot) {
            i++;
            swap(&arr[i], &arr[j]);
        }
    }
    swap(&arr[i + 1], &arr[high]);
    return i + 1;
}

int randomPartition(int arr[], int low, int high) {
    int randIndex = low + rand() % (high - low + 1);
    swap(&arr[randIndex], &arr[high]);
    return partition(arr, low, high);
}

/* Returns the element that would sit at 0-based index k if the
   array were fully sorted -- found without fully sorting it. */
int quickSelect(int arr[], int low, int high, int k) {
    if (low == high)
        return arr[low];

    int pivotIndex = randomPartition(arr, low, high);

    if (k == pivotIndex)
        return arr[pivotIndex];
    else if (k < pivotIndex)
        return quickSelect(arr, low, pivotIndex - 1, k);
    else
        return quickSelect(arr, pivotIndex + 1, high, k);
}

int main() {
    int n, k;
    srand(time(NULL));

    printf("Enter number of elements N: ");
    scanf("%d", &n);

    int *arr = (int *) malloc(n * sizeof(int));
    printf("Enter %d elements:\n", n);
    for (int i = 0; i < n; i++)
        scanf("%d", &arr[i]);

    printf("Enter K (1-based rank, e.g. K=1 -> smallest element): ");
    scanf("%d", &k);

    if (k < 1 || k > n) {
        printf("Invalid K. Must be between 1 and %d.\n", n);
    } else {
        int result = quickSelect(arr, 0, n - 1, k - 1);
        printf("The %d-th smallest element is: %d\n", k, result);
    }

    free(arr);
    return 0;
}
