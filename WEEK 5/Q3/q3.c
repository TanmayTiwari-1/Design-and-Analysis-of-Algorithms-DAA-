/*
 * DAA LAB-5, Q3
 * Implement Quick Sort of N random elements stored in a file.
 *
 * WORKFLOW:
 *   1. Generate N random integers and WRITE them to "input.txt".
 *   2. READ the N numbers back from "input.txt" into an array.
 *   3. Sort the array using Quick Sort.
 *   4. WRITE the sorted array to "output.txt" and also print it.
 *
 * QUICK SORT ALGORITHM:
 *   - Pick a pivot (here: last element, Lomuto scheme).
 *   - Partition array into "< pivot" and ">= pivot" groups; pivot
 *     lands in its final sorted position.
 *   - Recursively quicksort the left and right sub-arrays.
 *
 * COMPLEXITY ANALYSIS:
 *   Let T(n) be time for quicksort on n elements; partition = O(n).
 *   - Best Case (pivot always splits array evenly):
 *         T(n) = 2T(n/2) + O(n)  =>  T(n) = O(n log n)
 *   - Average Case (random data, as generated here):
 *         T(n) = O(n log n)   (expected, provable via randomized analysis)
 *   - Worst Case (already sorted/reverse sorted data with a bad pivot
 *     choice, e.g. always picking last element on sorted input):
 *         T(n) = T(n-1) + O(n)  =>  T(n) = O(n^2)
 *   - Space Complexity: O(1) extra (in-place swaps),
 *     O(log n) recursion stack on average, O(n) worst case.
 *   - File I/O adds O(n) time to read and O(n) time to write.
 *
 * SUMMARY:
 *   Best/Average Time : O(n log n)
 *   Worst Time         : O(n^2)
 *   Space              : O(1) extra / O(log n) avg recursion stack
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
        if (arr[j] < pivot) {
            i++;
            swap(&arr[i], &arr[j]);
        }
    }
    swap(&arr[i + 1], &arr[high]);
    return i + 1;
}

void quickSort(int arr[], int low, int high) {
    if (low < high) {
        int pi = partition(arr, low, high);
        quickSort(arr, low, pi - 1);
        quickSort(arr, pi + 1, high);
    }
}

/* Step 1: generate N random numbers and store them in a file */
void generateRandomFile(const char *filename, int n, int maxVal) {
    FILE *fp = fopen(filename, "w");
    if (!fp) { perror("Error opening file"); exit(1); }

    srand(time(NULL));
    for (int i = 0; i < n; i++)
        fprintf(fp, "%d\n", rand() % maxVal);

    fclose(fp);
}

/* Step 2: read N numbers from a file into an array */
int readFromFile(const char *filename, int arr[], int n) {
    FILE *fp = fopen(filename, "r");
    if (!fp) { perror("Error opening file"); exit(1); }

    int count = 0;
    while (count < n && fscanf(fp, "%d", &arr[count]) == 1)
        count++;

    fclose(fp);
    return count;
}

/* Step 4: write a sorted array back to a file */
void writeToFile(const char *filename, int arr[], int n) {
    FILE *fp = fopen(filename, "w");
    if (!fp) { perror("Error opening file"); exit(1); }

    for (int i = 0; i < n; i++)
        fprintf(fp, "%d\n", arr[i]);

    fclose(fp);
}

int main() {
    int n;
    printf("Enter number of random elements N to generate: ");
    scanf("%d", &n);

    int *arr = (int *) malloc(n * sizeof(int));

    /* 1. generate random numbers into input.txt */
    generateRandomFile("input.txt", n, 10000);
    printf("Generated %d random numbers into input.txt\n", n);

    /* 2. read them back */
    int count = readFromFile("input.txt", arr, n);

    printf("Unsorted elements read from file:\n");
    for (int i = 0; i < count; i++)
        printf("%d ", arr[i]);
    printf("\n");

    /* 3. quick sort */
    quickSort(arr, 0, count - 1);

    /* 4. write sorted output to file and display */
    writeToFile("output.txt", arr, count);

    printf("Sorted elements (also written to output.txt):\n");
    for (int i = 0; i < count; i++)
        printf("%d ", arr[i]);
    printf("\n");

    free(arr);
    return 0;
}
