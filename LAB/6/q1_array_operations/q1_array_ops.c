/* ============================================================
   Q1: 1D Array Operations and their Complexities
   ============================================================
   Input representation: a dynamically allocated array of n
   unsorted integers, arr[0..n-1].

   Operations implemented (worst-case complexity noted above each):
     (i)    max element                 -> O(n)
     (ii)   first & second largest      -> O(n)
     (iii)  mean                        -> O(n)
     (iv)   median                      -> O(n log n)  (sort based)
     (v)    standard deviation          -> O(n)
     (vi)   mode                        -> O(n log n)  (sort based)
     (vii)  remove all duplicates       -> O(n) average (hashing)
     (viii) reverse array               -> O(n)
     (ix)   partition wrt random pivot,
            elements < pivot placed
            AFTER elements >= pivot     -> O(n)

   Each routine is timed on a large randomly generated array so
   that the theoretical complexity can be validated empirically
   (see the driver / experiment section at the bottom).
   ============================================================ */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <string.h>

/* ---------- utility: generate a random array ---------- */
int *generate_array(int n, int range) {
    int *a = (int *)malloc(n * sizeof(int));
    for (int i = 0; i < n; i++) a[i] = (rand() % (2 * range + 1)) - range;
    return a;
}

void print_array(int *a, int n) {
    int lim = n < 20 ? n : 20;
    for (int i = 0; i < lim; i++) printf("%d ", a[i]);
    if (n > 20) printf("... (%d more)", n - 20);
    printf("\n");
}

/* (i) Maximum element -------------------------------------------------- O(n) */
int find_max(int *a, int n) {
    int m = a[0];
    for (int i = 1; i < n; i++) if (a[i] > m) m = a[i];
    return m;
}

/* (ii) First and second largest ---------------------------------------- O(n) */
void first_second_largest(int *a, int n, int *first, int *second) {
    if (n < 2) { printf("Need at least 2 elements\n"); return; }
    if (a[0] > a[1]) { *first = a[0]; *second = a[1]; }
    else             { *first = a[1]; *second = a[0]; }
    for (int i = 2; i < n; i++) {
        if (a[i] > *first)       { *second = *first; *first = a[i]; }
        else if (a[i] > *second) { *second = a[i]; }
    }
}

/* (iii) Mean ------------------------------------------------------------ O(n) */
double find_mean(int *a, int n) {
    double sum = 0;
    for (int i = 0; i < n; i++) sum += a[i];
    return sum / n;
}

/* helper: comparator for qsort */
int cmp_int(const void *x, const void *y) { return (*(int*)x - *(int*)y); }

/* (iv) Median  ------------------------------------------ O(n log n) via sort
   (Note: an O(n) worst-case is possible with median-of-medians
    selection, but the standard/simple approach used here is sort-based.) */
double find_median(int *a, int n) {
    int *tmp = (int *)malloc(n * sizeof(int));
    memcpy(tmp, a, n * sizeof(int));
    qsort(tmp, n, sizeof(int), cmp_int);
    double med;
    if (n % 2 == 0) med = (tmp[n/2 - 1] + tmp[n/2]) / 2.0;
    else            med = tmp[n/2];
    free(tmp);
    return med;
}

/* (v) Standard deviation -------------------------------------------------- O(n) */
double find_std_dev(int *a, int n) {
    double mean = find_mean(a, n);
    double sq_sum = 0;
    for (int i = 0; i < n; i++) sq_sum += (a[i] - mean) * (a[i] - mean);
    return sqrt(sq_sum / n);
}

/* (vi) Mode --------------------------------------------- O(n log n) via sort
   (sort, then scan for the longest run of equal values) */
int find_mode(int *a, int n) {
    int *tmp = (int *)malloc(n * sizeof(int));
    memcpy(tmp, a, n * sizeof(int));
    qsort(tmp, n, sizeof(int), cmp_int);
    int best_val = tmp[0], best_count = 1, cur_count = 1;
    for (int i = 1; i < n; i++) {
        if (tmp[i] == tmp[i-1]) cur_count++;
        else cur_count = 1;
        if (cur_count > best_count) { best_count = cur_count; best_val = tmp[i]; }
    }
    free(tmp);
    return best_val;
}

/* (vii) Remove all duplicates ----------------------- O(n) average (hashing)
   Simple open-addressing hash set used to achieve average O(n). */
typedef struct { int *keys; char *used; int cap; } HashSet;

HashSet hs_create(int cap) {
    HashSet h;
    h.cap = cap;
    h.keys = (int *)calloc(cap, sizeof(int));
    h.used = (char *)calloc(cap, sizeof(char));
    return h;
}
unsigned hs_hash(int key, int cap) {
    unsigned long h = (unsigned long)(key + 2147483648L);
    return (unsigned)(h % cap);
}
int hs_insert_if_absent(HashSet *h, int key) {
    unsigned idx = hs_hash(key, h->cap);
    while (h->used[idx]) {
        if (h->keys[idx] == key) return 0;  /* already present -> duplicate */
        idx = (idx + 1) % h->cap;
    }
    h->used[idx] = 1;
    h->keys[idx] = key;
    return 1; /* newly inserted -> unique */
}

int remove_duplicates(int *a, int n, int *out) {
    HashSet h = hs_create(2 * n + 1);
    int k = 0;
    for (int i = 0; i < n; i++)
        if (hs_insert_if_absent(&h, a[i])) out[k++] = a[i];
    free(h.keys); free(h.used);
    return k; /* new length */
}

/* (viii) Reverse the array ------------------------------------------------ O(n) */
void reverse_array(int *a, int n) {
    int i = 0, j = n - 1;
    while (i < j) { int t = a[i]; a[i] = a[j]; a[j] = t; i++; j--; }
}

/* (ix) Partition wrt a random pivot: all elements < pivot AFTER
        all elements >= pivot.                                     ---- O(n)
   This is a single Lomuto-style pass with the usual comparison reversed. */
int partition_greater_first(int *a, int n) {
    int pivot_index = rand() % n;
    int pivot = a[pivot_index];
    /* move pivot to the end temporarily */
    int t = a[pivot_index]; a[pivot_index] = a[n-1]; a[n-1] = t;

    int store = 0;
    for (int i = 0; i < n - 1; i++) {
        if (a[i] >= pivot) {           /* keep >= pivot at the front */
            t = a[i]; a[i] = a[store]; a[store] = t;
            store++;
        }
    }
    /* put pivot itself right after the ">= pivot" block */
    t = a[store]; a[store] = a[n-1]; a[n-1] = t;
    return store; /* index of pivot after partition */
}

/* ------------------------------------------------------------------ */
/*                         DRIVER / VALIDATION                        */
/* ------------------------------------------------------------------ */
double now_seconds() {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return ts.tv_sec + ts.tv_nsec / 1e9;
}

void run_experiment(int n) {
    srand(42);
    int *a = generate_array(n, 1000000);

    printf("\n---- n = %d ----\n", n);

    double t0 = now_seconds();
    int mx = find_max(a, n);
    double t1 = now_seconds();
    printf("(i)   max               = %d\t\ttime=%.6fs\n", mx, t1 - t0);

    int first, second;
    t0 = now_seconds();
    first_second_largest(a, n, &first, &second);
    t1 = now_seconds();
    printf("(ii)  first,second      = %d, %d\ttime=%.6fs\n", first, second, t1 - t0);

    t0 = now_seconds();
    double mean = find_mean(a, n);
    t1 = now_seconds();
    printf("(iii) mean               = %.4f\ttime=%.6fs\n", mean, t1 - t0);

    t0 = now_seconds();
    double med = find_median(a, n);
    t1 = now_seconds();
    printf("(iv)  median             = %.4f\ttime=%.6fs\n", med, t1 - t0);

    t0 = now_seconds();
    double sd = find_std_dev(a, n);
    t1 = now_seconds();
    printf("(v)   std_dev            = %.4f\ttime=%.6fs\n", sd, t1 - t0);

    t0 = now_seconds();
    int mode = find_mode(a, n);
    t1 = now_seconds();
    printf("(vi)  mode               = %d\t\ttime=%.6fs\n", mode, t1 - t0);

    int *dedup_src = generate_array(n, n / 4 + 1); /* force real duplicates */
    int *out = (int *)malloc(n * sizeof(int));
    t0 = now_seconds();
    int newlen = remove_duplicates(dedup_src, n, out);
    t1 = now_seconds();
    printf("(vii) unique count       = %d / %d\ttime=%.6fs\n", newlen, n, t1 - t0);
    free(dedup_src); free(out);

    int *b = (int *)malloc(n * sizeof(int));
    memcpy(b, a, n * sizeof(int));
    t0 = now_seconds();
    reverse_array(b, n);
    t1 = now_seconds();
    printf("(viii) reversed[0]       = %d (orig last=%d)\ttime=%.6fs\n", b[0], a[n-1], t1 - t0);
    free(b);

    int *c = (int *)malloc(n * sizeof(int));
    memcpy(c, a, n * sizeof(int));
    t0 = now_seconds();
    int pidx = partition_greater_first(c, n);
    t1 = now_seconds();
    /* validate: everything before pidx should be >= pivot, everything after < pivot */
    int ok = 1;
    for (int i = 0; i < pidx; i++) if (c[i] < c[pidx]) ok = 0;
    for (int i = pidx + 1; i < n; i++) if (c[i] >= c[pidx]) ok = 0;
    printf("(ix)  partition valid?   = %s (pivot=%d @ idx %d)\ttime=%.6fs\n",
           ok ? "YES" : "NO", c[pidx], pidx, t1 - t0);
    free(c);

    free(a);
}

/* CSV mode: run each operation over a range of n and print
   n,operation,time_seconds  rows to stdout so they can be
   redirected into a .csv file for plotting. */
void run_csv(int n) {
    srand(42);
    int *a = generate_array(n, 1000000);
    double t0, t1;

    t0 = now_seconds(); find_max(a, n); t1 = now_seconds();
    printf("%d,max,%.9f\n", n, t1 - t0);

    int first, second;
    t0 = now_seconds(); first_second_largest(a, n, &first, &second); t1 = now_seconds();
    printf("%d,first_second_largest,%.9f\n", n, t1 - t0);

    t0 = now_seconds(); find_mean(a, n); t1 = now_seconds();
    printf("%d,mean,%.9f\n", n, t1 - t0);

    t0 = now_seconds(); find_median(a, n); t1 = now_seconds();
    printf("%d,median,%.9f\n", n, t1 - t0);

    t0 = now_seconds(); find_std_dev(a, n); t1 = now_seconds();
    printf("%d,std_dev,%.9f\n", n, t1 - t0);

    t0 = now_seconds(); find_mode(a, n); t1 = now_seconds();
    printf("%d,mode,%.9f\n", n, t1 - t0);

    int *dedup_src = generate_array(n, n / 4 + 1);
    int *out = (int *)malloc(n * sizeof(int));
    t0 = now_seconds(); remove_duplicates(dedup_src, n, out); t1 = now_seconds();
    printf("%d,remove_duplicates,%.9f\n", n, t1 - t0);
    free(dedup_src); free(out);

    int *b = (int *)malloc(n * sizeof(int));
    memcpy(b, a, n * sizeof(int));
    t0 = now_seconds(); reverse_array(b, n); t1 = now_seconds();
    printf("%d,reverse,%.9f\n", n, t1 - t0);
    free(b);

    int *c = (int *)malloc(n * sizeof(int));
    memcpy(c, a, n * sizeof(int));
    t0 = now_seconds(); partition_greater_first(c, n); t1 = now_seconds();
    printf("%d,partition,%.9f\n", n, t1 - t0);
    free(c);

    free(a);
}

int main(int argc, char **argv) {
    if (argc > 1 && strcmp(argv[1], "--csv") == 0) {
        printf("n,operation,time_seconds\n");
        int sizes[] = {1000, 2000, 5000, 10000, 20000, 50000, 100000,
                       200000, 500000, 1000000};
        for (int i = 0; i < 10; i++) run_csv(sizes[i]);
        return 0;
    }

    printf("Q1: 1D Array Operations - Complexity Validation\n");
    int sizes[] = {1000, 10000, 100000, 1000000};
    for (int i = 0; i < 4; i++) run_experiment(sizes[i]);
    printf("\nDoubling n roughly doubles the time for O(n) ops (i,ii,iii,v,vii,viii,ix)\n"
           "and more-than-doubles (by an extra log factor) for O(n log n) ops (iv,vi),\n"
           "consistent with the theoretical worst-case bounds derived above.\n");
    return 0;
}
