#include <stdio.h>
#include <stdlib.h>
#include <time.h>

long long comparisons;

/* merge two sorted arrays a[0..na-1], b[0..nb-1] -> freshly malloc'd result of size na+nb */
int* merge_two(int *a, int na, int *b, int nb) {
    int *res = malloc((na + nb) * sizeof(int));
    int i = 0, j = 0, k = 0;
    while (i < na && j < nb) {
        comparisons++;
        if (a[i] <= b[j]) res[k++] = a[i++];
        else               res[k++] = b[j++];
    }
    while (i < na) res[k++] = a[i++];
    while (j < nb) res[k++] = b[j++];
    return res;
}

/* ===================================================================
   METHOD 1: merge arrays sequentially: (((A1 U A2) U A3) U A4) ...
   Worst case: O(n) + O(2n) + O(3n) + ... + O((k-1)n) = O(n*k^2)
   =================================================================== */
int* method1_sequential(int **arrs, int *sizes, int k, long long *cmp_out) {
    comparisons = 0;
    int cur_size = sizes[0];
    int *cur = malloc(cur_size * sizeof(int));
    for (int i = 0; i < cur_size; i++) cur[i] = arrs[0][i];
    for (int idx = 1; idx < k; idx++) {
        int *merged = merge_two(cur, cur_size, arrs[idx], sizes[idx]);
        free(cur);
        cur = merged;
        cur_size += sizes[idx];
    }
    *cmp_out = comparisons;
    return cur;
}


int* method2_pairwise(int **arrs, int *sizes, int k, long long *cmp_out) {
    comparisons = 0;
    int **cur = malloc(k * sizeof(int*));
    int *cur_sizes = malloc(k * sizeof(int));
    int cur_k = k;
    for (int i = 0; i < k; i++) {
        cur_sizes[i] = sizes[i];
        cur[i] = malloc(sizes[i] * sizeof(int));
        for (int j = 0; j < sizes[i]; j++) cur[i][j] = arrs[i][j];
    }
    while (cur_k > 1) {
        int new_k = (cur_k + 1) / 2;
        int **next = malloc(new_k * sizeof(int*));
        int *next_sizes = malloc(new_k * sizeof(int));
        int w = 0;
        for (int i = 0; i + 1 < cur_k; i += 2) {
            next[w] = merge_two(cur[i], cur_sizes[i], cur[i+1], cur_sizes[i+1]);
            next_sizes[w] = cur_sizes[i] + cur_sizes[i+1];
            free(cur[i]); free(cur[i+1]);
            w++;
        }
        if (cur_k % 2 == 1) {                 /* odd one out carries forward untouched */
            next[w] = cur[cur_k - 1];
            next_sizes[w] = cur_sizes[cur_k - 1];
            w++;
        }
        free(cur); free(cur_sizes);
        cur = next; cur_sizes = next_sizes; cur_k = new_k;
    }
    int *result = cur[0];
    free(cur); free(cur_sizes);
    *cmp_out = comparisons;
    return result;
}

static double now_us(void) { return (double)clock() * 1000000.0 / CLOCKS_PER_SEC; }

int** make_sorted_arrays(int k, int n, int seed_offset) {
    int **arrs = malloc(k * sizeof(int*));
    for (int i = 0; i < k; i++) {
        arrs[i] = malloc(n * sizeof(int));
        int v = 0;
        for (int j = 0; j < n; j++) { v += 1 + rand() % 5; arrs[i][j] = v; }
    }
    return arrs;
}
void free_arrays(int **arrs, int k) { for (int i = 0; i < k; i++) free(arrs[i]); free(arrs); }
int is_sorted(int *a, int n) { for (int i = 1; i < n; i++) if (a[i-1] > a[i]) return 0; return 1; }

int main(void) {
    srand(99);

    {
        int k = 16;
        int ns[] = {200, 500, 1000, 2000, 4000, 8000, 16000, 32000};
        int cnt = sizeof(ns) / sizeof(ns[0]);
        FILE *fp = fopen("kmerge_vs_n.csv", "w");
        fprintf(fp, "n,k,cmp_method1,cmp_method2,time_method1_us,time_method2_us\n");
        for (int t = 0; t < cnt; t++) {
            int n = ns[t];
            int *sizes = malloc(k * sizeof(int));
            for (int i = 0; i < k; i++) sizes[i] = n;
            int **arrs = make_sorted_arrays(k, n, t);

            long long cmp1, cmp2;
            double t0 = now_us();
            int *r1 = method1_sequential(arrs, sizes, k, &cmp1);
            double time1 = now_us() - t0;

            t0 = now_us();
            int *r2 = method2_pairwise(arrs, sizes, k, &cmp2);
            double time2 = now_us() - t0;

            int total = n * k;
            printf("[A] n=%6d k=%2d  M1 cmp=%9lld t=%8.1fus  M2 cmp=%9lld t=%8.1fus  sorted(%d,%d)\n",
                   n, k, cmp1, time1, cmp2, time2, is_sorted(r1, total), is_sorted(r2, total));
            fprintf(fp, "%d,%d,%lld,%lld,%.2f,%.2f\n", n, k, cmp1, cmp2, time1, time2);

            free(r1); free(r2); free(sizes); free_arrays(arrs, k);
        }
        fclose(fp);
    }

    /* ---------- Experiment B: fixed n, vary k ---------- */
    {
        int n = 2000;
        int ks[] = {2, 4, 8, 16, 32, 64, 128, 256};
        int cnt = sizeof(ks) / sizeof(ks[0]);
        FILE *fp = fopen("kmerge_vs_k.csv", "w");
        fprintf(fp, "n,k,cmp_method1,cmp_method2,time_method1_us,time_method2_us\n");
        for (int t = 0; t < cnt; t++) {
            int k = ks[t];
            int *sizes = malloc(k * sizeof(int));
            for (int i = 0; i < k; i++) sizes[i] = n;
            int **arrs = make_sorted_arrays(k, n, t + 100);

            long long cmp1, cmp2;
            double t0 = now_us();
            int *r1 = method1_sequential(arrs, sizes, k, &cmp1);
            double time1 = now_us() - t0;

            t0 = now_us();
            int *r2 = method2_pairwise(arrs, sizes, k, &cmp2);
            double time2 = now_us() - t0;

            int total = n * k;
            printf("[B] n=%6d k=%4d  M1 cmp=%9lld t=%8.1fus  M2 cmp=%9lld t=%8.1fus  sorted(%d,%d)\n",
                   n, k, cmp1, time1, cmp2, time2, is_sorted(r1, total), is_sorted(r2, total));
            fprintf(fp, "%d,%d,%lld,%lld,%.2f,%.2f\n", n, k, cmp1, cmp2, time1, time2);

            free(r1); free(r2); free(sizes); free_arrays(arrs, k);
        }
        fclose(fp);
    }

    printf("kmerge_vs_n.csv and kmerge_vs_k.csv generated successfully.\n");
    return 0;
}
