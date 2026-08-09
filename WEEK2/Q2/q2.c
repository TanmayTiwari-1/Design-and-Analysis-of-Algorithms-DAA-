#include <stdio.h>
#include <stdlib.h>
#include <time.h>

long long comparisons;

/* ============================================================
   STANDARD MERGE SORT (2-way): divide into halves
   T(n) = 2T(n/2) + O(n)  =>  T(n) = O(n log n)
   ============================================================ */
void merge2(int *arr, int lo, int mid, int hi, int *tmp) {
    int i = lo, j = mid + 1, k = lo;
    while (i <= mid && j <= hi) {
        comparisons++;
        if (arr[i] <= arr[j]) tmp[k++] = arr[i++];
        else                  tmp[k++] = arr[j++];
    }
    while (i <= mid) tmp[k++] = arr[i++];
    while (j <= hi)  tmp[k++] = arr[j++];
    for (int x = lo; x <= hi; x++) arr[x] = tmp[x];
}
void merge_sort(int *arr, int lo, int hi, int *tmp) {
    if (lo >= hi) return;
    int mid = lo + (hi - lo) / 2;
    merge_sort(arr, lo, mid, tmp);
    merge_sort(arr, mid + 1, hi, tmp);
    merge2(arr, lo, mid, hi, tmp);
}

/* ============================================================
   MODIFIED MERGE SORT (3-way): divide into thirds
   T(n) = 3T(n/3) + O(n)  =>  T(n) = O(n log n)  (same order, smaller constant)
   ============================================================ */
void merge3(int *arr, int lo, int m1, int m2, int hi, int *tmp) {
    int i = lo, j = m1 + 1, k = m2 + 1, w = lo;
    while (i <= m1 && j <= m2 && k <= hi) {
        comparisons += 2;
        if (arr[i] <= arr[j] && arr[i] <= arr[k])      tmp[w++] = arr[i++];
        else if (arr[j] <= arr[i] && arr[j] <= arr[k])  tmp[w++] = arr[j++];
        else                                             tmp[w++] = arr[k++];
    }
    /* at most one of the three runs remains -> finish with ordinary 2-way merges */
    while (i <= m1 && j <= m2) { comparisons++; tmp[w++] = (arr[i] <= arr[j]) ? arr[i++] : arr[j++]; }
    while (j <= m2 && k <= hi) { comparisons++; tmp[w++] = (arr[j] <= arr[k]) ? arr[j++] : arr[k++]; }
    while (i <= m1 && k <= hi) { comparisons++; tmp[w++] = (arr[i] <= arr[k]) ? arr[i++] : arr[k++]; }
    while (i <= m1) tmp[w++] = arr[i++];
    while (j <= m2) tmp[w++] = arr[j++];
    while (k <= hi) tmp[w++] = arr[k++];
    for (int x = lo; x <= hi; x++) arr[x] = tmp[x];
}
void merge_sort3(int *arr, int lo, int hi, int *tmp) {
    if (lo >= hi) return;
    int len = hi - lo + 1;
    if (len < 3) {                       /* fall back to a simple 2-way split for tiny ranges */
        int mid = lo + (hi - lo) / 2;
        merge_sort3(arr, lo, mid, tmp);
        merge_sort3(arr, mid + 1, hi, tmp);
        merge2(arr, lo, mid, hi, tmp);
        return;
    }
    int third = len / 3;
    int m1 = lo + third - 1;
    int m2 = lo + 2 * third - 1;
    merge_sort3(arr, lo, m1, tmp);
    merge_sort3(arr, m1 + 1, m2, tmp);
    merge_sort3(arr, m2 + 1, hi, tmp);
    merge3(arr, lo, m1, m2, hi, tmp);
}

static double now_us(void) { return (double)clock() * 1000000.0 / CLOCKS_PER_SEC; }

int main(void) {
    srand(7);
    int sizes[] = {1000, 2000, 5000, 10000, 20000, 50000, 100000, 200000, 400000};
    int ns = sizeof(sizes) / sizeof(sizes[0]);

    FILE *fp = fopen("mergesort_compare.csv", "w");
    fprintf(fp, "n,comparisons_2way,comparisons_3way,time_2way_us,time_3way_us\n");

    for (int s = 0; s < ns; s++) {
        int n = sizes[s];
        int *base = malloc(n * sizeof(int));
        int *a1 = malloc(n * sizeof(int));
        int *a2 = malloc(n * sizeof(int));
        int *tmp = malloc(n * sizeof(int));
        for (int i = 0; i < n; i++) base[i] = rand();
        for (int i = 0; i < n; i++) { a1[i] = base[i]; a2[i] = base[i]; }

        comparisons = 0;
        double t = now_us();
        merge_sort(a1, 0, n - 1, tmp);
        double time2 = now_us() - t;
        long long cmp2 = comparisons;

        comparisons = 0;
        t = now_us();
        merge_sort3(a2, 0, n - 1, tmp);
        double time3 = now_us() - t;
        long long cmp3 = comparisons;

        /* sanity check both produce a sorted array */
        int ok = 1;
        for (int i = 1; i < n; i++) if (a1[i-1] > a1[i] || a2[i-1] > a2[i]) ok = 0;

        printf("n=%7d  2-way cmp=%10lld time=%9.1fus | 3-way cmp=%10lld time=%9.1fus | sorted_ok=%d\n",
               n, cmp2, time2, cmp3, time3, ok);
        fprintf(fp, "%d,%lld,%lld,%.2f,%.2f\n", n, cmp2, cmp3, time2, time3);

        free(base); free(a1); free(a2); free(tmp);
    }
    fclose(fp);
    printf("mergesort_compare.csv generated successfully.\n");
    return 0;
}
