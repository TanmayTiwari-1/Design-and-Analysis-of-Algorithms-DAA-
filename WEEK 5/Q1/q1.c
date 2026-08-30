/*
 * DAA LAB-5, Q1
 * Find the median of a list of N numbers WITHOUT sorting the list.
 *
 * IDEA:
 * The median is just the (N/2)-th order statistic (for odd N) or the
 * average of the (N/2 - 1)-th and (N/2)-th order statistics (even N).
 * We can find the k-th smallest element without fully sorting the array
 * by using the QUICKSELECT algorithm, which is a variant of quicksort
 * that only recurses into the half of the array that contains the
 * element we are looking for.
 *
 * ALGORITHM (Randomized Quickselect):
 * 1. Pick a random pivot from the array.
 * 2. Partition the array around the pivot (Lomuto partition), so that
 *    elements smaller than pivot are on the left and larger on the right.
 * 3. Let the pivot's final position be 'p'.
 *      - If p == k, pivot IS the answer, return it.
 *      - If k < p, recurse on the left part.
 *      - If k > p, recurse on the right part.
 * This way we never touch the part of the array we don't need,
 * so the whole array never gets fully sorted.
 *
 * COMPLEXITY ANALYSIS:
 * Let T(n) be the time to run quickselect on n elements.
 * - Partitioning takes O(n).
 * - Best/Average case: random pivot splits the array roughly in half,
 *   so only ONE half is recursed into (unlike quicksort which recurses
 *   into both halves):
 *        T(n) = T(n/2) + O(n)  =>  T(n) = O(n)   (Average Case)
 *   (This can be shown formally using the Master theorem intuition
 *    for a "decreasing geometric series": n + n/2 + n/4 + ... = O(n))
 * - Worst case: pivot always ends up smallest/largest element
 *   (e.g. already sorted array with first-element pivot):
 *        T(n) = T(n-1) + O(n)  =>  T(n) = O(n^2)  (Worst Case)
 *   Using a RANDOM pivot makes the worst case extremely unlikely.
 * - Worst case can be brought down to O(n) using the deterministic
 *   "Median of Medians" pivot selection algorithm (not required here,
 *   but worth mentioning).
 * - Space Complexity: O(1) extra (in-place, ignoring recursion stack),
 *   recursion depth O(log n) average, O(n) worst case.
 *
 * SUMMARY:
 *   Average Time : O(n)
 *   Worst Time   : O(n^2)   (O(n) with median-of-medians)
 *   Space        : O(1) extra / O(log n) avg recursion stack
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void swap(int *a, int *b) {
    int t = *a; *a = *b; *b = t;
}

/* Lomuto partition scheme: places arr[high] (pivot) at its correct
   sorted position and returns that position. */
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

/* Randomized partition: choose a random index as pivot, swap it to the
   end, then do a normal Lomuto partition. This avoids worst-case
   behaviour on already-sorted or adversarial inputs. */
int randomPartition(int arr[], int low, int high) {
    int randIndex = low + rand() % (high - low + 1);
    swap(&arr[randIndex], &arr[high]);
    return partition(arr, low, high);
}

/* quickSelect returns the element that would be at index k
   (0-based) if the array WERE sorted, without sorting the whole array. */
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

double findMedian(int arr[], int n) {
    if (n % 2 == 1) {
        /* odd: middle element is at index n/2 */
        return (double) quickSelect(arr, 0, n - 1, n / 2);
    } else {
        /* even: average of elements at index n/2 - 1 and n/2 */
        /* Note: quickSelect partially rearranges arr each call, but
           that is fine since we don't care about order, only value. */
        int a = quickSelect(arr, 0, n - 1, n / 2 - 1);
        int b = quickSelect(arr, 0, n - 1, n / 2);
        return (a + b) / 2.0;
    }
}

int main() {
    int n;
    srand(time(NULL));

    printf("Enter number of elements N: ");
    scanf("%d", &n);

    int *arr = (int *) malloc(n * sizeof(int));
    printf("Enter %d elements:\n", n);
    for (int i = 0; i < n; i++)
        scanf("%d", &arr[i]);

    double median = findMedian(arr, n);
    printf("Median = %.2lf\n", median);

    free(arr);
    return 0;
}
