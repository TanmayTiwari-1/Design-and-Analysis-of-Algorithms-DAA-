/*
 * DAA LAB-5, Q4
 * Implement Heap Sort to sort N randomly generated elements stored
 * in a file. Do the complexity analysis.
 *
 * WORKFLOW:
 *   1. Generate N random integers and WRITE them to "input.txt".
 *   2. READ the N numbers back from "input.txt" into an array.
 *   3. Sort the array using Heap Sort.
 *   4. WRITE the sorted array to "output.txt" and also print it.
 *
 * HEAP SORT ALGORITHM:
 *   1. BUILD a max-heap out of the input array (heapify every non-leaf
 *      node from the bottom up).
 *   2. Repeatedly SWAP the root (largest element) with the last
 *      element of the heap, shrink the heap size by 1, and
 *      "sift down" (heapify) the new root to restore the max-heap
 *      property. Doing this n times places every element into its
 *      correct sorted position, from largest to smallest.
 *
 * COMPLEXITY ANALYSIS:
 *   Let n = number of elements.
 *   - Building the initial heap (bottom-up heapify):
 *         O(n)      <-- NOT O(n log n); a tighter amortized analysis
 *                        shows building a heap bottom-up costs O(n).
 *   - Extracting the max and re-heapifying is done n times, and each
 *     heapify call on a heap of size k costs O(log k):
 *         Sum_{k=1}^{n} O(log k) = O(n log n)
 *   - Total Time Complexity (Best, Average, AND Worst case are all
 *     the same for heap sort, unlike quicksort):
 *         T(n) = O(n) + O(n log n) = O(n log n)   for ALL cases
 *   - Space Complexity: O(1) extra (sorts in-place using the same
 *     array as the heap), i.e. no extra array needed (unlike merge
 *     sort). Heap sort is NOT stable.
 *   - File I/O adds O(n) time to read and O(n) time to write.
 *
 * SUMMARY:
 *   Best/Average/Worst Time : O(n log n)  (always, no bad-input case)
 *   Space                   : O(1) extra, in-place
 *   Stability               : Not stable
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void swap(int *a, int *b) {
    int t = *a; *a = *b; *b = t;
}

/* Sift the node at index i (0-based) down to restore the max-heap
   property, assuming the subtrees rooted at its children are
   already valid max-heaps. 'n' is the current heap size. */
void heapify(int arr[], int n, int i) {
    int largest = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;

    if (left < n && arr[left] > arr[largest])
        largest = left;

    if (right < n && arr[right] > arr[largest])
        largest = right;

    if (largest != i) {
        swap(&arr[i], &arr[largest]);
        heapify(arr, n, largest);   /* recurse into the affected subtree */
    }
}

void heapSort(int arr[], int n) {
    /* Step 1: Build max heap. Start from the last non-leaf node
       (index n/2 - 1) and heapify every node up to the root. */
    for (int i = n / 2 - 1; i >= 0; i--)
        heapify(arr, n, i);

    /* Step 2: Repeatedly extract the max (root) and place it at
       the end, then re-heapify the reduced heap. */
    for (int i = n - 1; i > 0; i--) {
        swap(&arr[0], &arr[i]);     /* move current max to the end */
        heapify(arr, i, 0);         /* heapify the reduced heap */
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

    /* 3. heap sort */
    heapSort(arr, count);

    /* 4. write sorted output to file and display */
    writeToFile("output.txt", arr, count);

    printf("Sorted elements (also written to output.txt):\n");
    for (int i = 0; i < count; i++)
        printf("%d ", arr[i]);
    printf("\n");

    free(arr);
    return 0;
}
