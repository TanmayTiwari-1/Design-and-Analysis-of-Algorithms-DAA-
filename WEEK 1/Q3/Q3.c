#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// (i) optimized: early exit if a pass has zero swaps
long bubble_sort_optimized(int *arr, int n) {
    long comparisons = 0;
    int swapped;
    for (int i = 0; i < n - 1; i++) {
        swapped = 0;
        for (int j = 0; j < n - 1 - i; j++) {
            comparisons++;
            if (arr[j] > arr[j + 1]) {
                int tmp = arr[j]; arr[j] = arr[j + 1]; arr[j + 1] = tmp;
                swapped = 1;
            }
        }
        if (!swapped) break; // array already sorted, stop early
    }
    return comparisons;
}


long bubble_sort_unoptimized(int *arr, int n) {
    long comparisons = 0;
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - 1 - i; j++) {
            comparisons++;
            if (arr[j] > arr[j + 1]) {
                int tmp = arr[j]; arr[j] = arr[j + 1]; arr[j + 1] = tmp;
            }
        }
       
    }
    return comparisons;
}

void fill_random(int *arr, int n) {
    for (int i = 0; i < n; i++) arr[i] = rand() % 100000;
}

void copy_array(int *dst, int *src, int n) {
    for (int i = 0; i < n; i++) dst[i] = src[i];
}

int main(void) {
    srand((unsigned)time(NULL));

    int sizes[] = {10, 50, 100, 200, 400, 800, 1200, 1600, 2000};
    int num_sizes = sizeof(sizes) / sizeof(sizes[0]);

    FILE *fp = fopen("bubble_sort_data.csv", "w");
    if (!fp) { perror("fopen"); return 1; }
    fprintf(fp, "n,comparisons_optimized,comparisons_unoptimized\n");

    printf("%-8s %-22s %-22s\n", "n", "Optimized (comparisons)", "Unoptimized (comparisons)");
    for (int s = 0; s < num_sizes; s++) {
        int n = sizes[s];
        int *base = malloc(n * sizeof(int));
        int *a1 = malloc(n * sizeof(int));
        int *a2 = malloc(n * sizeof(int));
        fill_random(base, n);

        copy_array(a1, base, n);
        copy_array(a2, base, n);

        long c1 = bubble_sort_optimized(a1, n);
        long c2 = bubble_sort_unoptimized(a2, n);

        printf("%-8d %-22ld %-22ld\n", n, c1, c2);
        fprintf(fp, "%d,%ld,%ld\n", n, c1, c2);

        free(base); free(a1); free(a2);
    }

    fclose(fp);
  
    printf("\n--- Best case: already-sorted input ---\n");
    printf("%-8s %-22s %-22s\n", "n", "Optimized (comparisons)", "Unoptimized (comparisons)");
    FILE *fp2 = fopen("bubble_sort_sorted_data.csv", "w");
    fprintf(fp2, "n,comparisons_optimized,comparisons_unoptimized\n");
    for (int s = 0; s < num_sizes; s++) {
        int n = sizes[s];
        int *a1 = malloc(n * sizeof(int));
        int *a2 = malloc(n * sizeof(int));
        for (int i = 0; i < n; i++) { a1[i] = i; a2[i] = i; } // already sorted
        long c1 = bubble_sort_optimized(a1, n);
        long c2 = bubble_sort_unoptimized(a2, n);
        printf("%-8d %-22ld %-22ld\n", n, c1, c2);
        fprintf(fp2, "%d,%ld,%ld\n", n, c1, c2);
        free(a1); free(a2);
    }
    fclose(fp2);

    printf("\nData written to bubble_sort_data.csv (random input) and\n");
    printf("bubble_sort_sorted_data.csv (already-sorted input) -- plot n vs comparisons.\n");
    printf("Conclusion: on random data both versions are close to n(n-1)/2\n");
    printf("comparisons (O(n^2)) since swaps rarely stop early. The\n");
    printf("optimized version only pays off when the input is ALREADY\n");
    printf("sorted or nearly sorted, giving it O(n) best-case behaviour\n");
    printf("versus the unoptimized version's O(n^2) even in that case.\n");

    return 0;
}
