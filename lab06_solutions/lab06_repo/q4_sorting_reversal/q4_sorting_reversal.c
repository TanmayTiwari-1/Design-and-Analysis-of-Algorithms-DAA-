/* ============================================================
   Q4: Sorting a permutation using reverse(p,i,j)
   ============================================================

   PART A - O(n) reversals suffice (Selection-by-reversal, "pancake sort"):
     For i = n down to 2:
        let j = position of value i in current p[1..i]
        if j != i:
           reverse(p, 1, j)   // bring value i to the front
           reverse(p, 1, i)   // then move it to its final position i
     Each iteration uses at most 2 reversals and fixes one more
     element into its final place -> at most 2(n-1) = O(n) reversals
     total. This proves any permutation can be sorted with O(n)
     reversals (irrespective of individual reversal cost).

   PART B - O(n log^2 n) COST algorithm (cost of reverse(i,j) = j-i+1):
     Use MERGE SORT, but implement the in-place MERGE step using
     only array reversals (a "rotation" of a block can be done
     with exactly 3 reversals: reverse(i,mid-1); reverse(mid,j);
     reverse(i,j) — this rotates the block left/right, total cost
     O(2*(j-i+1)) = O(block length)).

     merge_by_reversal(p, lo, mid, hi):  merges two adjacent SORTED
     blocks p[lo..mid-1] and p[mid..hi] in place, using ROTATION
     recursively:
        if either block empty: return
        find (binary search) split point in the right block:
            let k = size of the left prefix of left-block <= some
            comparison element - concretely:
              m1 = size of left block = mid-lo
              m2 = size of right block = hi-mid+1
              if m1 <= m2:
                  take the middle element L[m1/2] of the left block
                  binary-search its position q in the right block
                  rotate p[lo+m1/2 .. mid+q-1] so that the right-block
                  prefix (size q) moves before the left-block's
                  middle element -> this places L[m1/2] in its final
                  sorted position, cost = O(m1/2 + q)  (the rotated
                  block length)
                  recurse on the two resulting sub-merges
              else: symmetric case using the right block's middle
     This recursive-rotation merge is the classic in-place-merge-
     via-rotation technique; merging two blocks whose combined size
     is s costs O(s log s) (its own D&C recursion, same recurrence
     shape as merge sort's own T(s)=2T(s/2)+O(s)).

     Overall recurrence for the sort:
        T(n) = 2 T(n/2) + O(n log n)      [cost of merging at this level]
        By the Master theorem (case with an extra log factor):
        T(n) = O(n log^2 n).

   This file implements BOTH parts and empirically measures the
   TOTAL REVERSAL COST (sum of |j-i|+1 over every reverse() call)
   to confirm it grows like n*log^2(n), not n^2.
   ============================================================ */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <string.h>

/* ---------------- reverse with cost accounting ---------------- */
long long g_total_cost = 0;
long long g_call_count = 0;

void reverse(int *p, int i, int j) { /* inclusive indices, 0-based */
    g_total_cost += (j - i + 1);
    g_call_count++;
    while (i < j) { int t = p[i]; p[i] = p[j]; p[j] = t; i++; j--; }
}

int is_sorted(int *p, int n) {
    for (int i = 1; i < n; i++) if (p[i - 1] > p[i]) return 0;
    return 1;
}

void print_arr(int *p, int n) {
    int lim = n < 20 ? n : 20;
    for (int i = 0; i < lim; i++) printf("%d ", p[i]);
    if (n > 20) printf("...");
    printf("\n");
}

/* ============================================================
   PART A: selection-by-reversal, O(n) reversals (pancake-sort style)
   ============================================================ */
void sort_by_reversal_On_reversals(int *p, int n) {
    /* We repeatedly place the maximum remaining element at the end. */
    for (int size = n; size >= 2; size--) {
        /* find index of max element in p[0..size-1] */
        int max_idx = 0;
        for (int k = 1; k < size; k++) if (p[k] > p[max_idx]) max_idx = k;
        if (max_idx == size - 1) continue; /* already in place */
        if (max_idx != 0) reverse(p, 0, max_idx);   /* bring max to front */
        reverse(p, 0, size - 1);                    /* flip it to the end */
    }
}

/* ============================================================
   PART B: merge sort where the merge step is implemented purely
   via reversal-based rotation, giving O(n log^2 n) total cost.
   ============================================================ */

/* rotate p[i..j] so that the block [mid..j] comes before [i..mid-1].
   i.e. left-rotate the combined block by (mid-i) positions.
   Implemented with the classic "3 reversals" trick:
       reverse(i, mid-1); reverse(mid, j); reverse(i, j)
   Cost = O(j-i+1) (three reversals whose total length is bounded
   by 2*(j-i+1)). */
void rotate_by_reversal(int *p, int i, int mid, int j) {
    if (i >= mid || mid > j) return;
    reverse(p, i, mid - 1);
    reverse(p, mid, j);
    reverse(p, i, j);
}

/* binary search: number of elements in p[lo..hi] (sorted) that are <= key */
int count_leq(int *p, int lo, int hi, int key) {
    int l = lo, h = hi + 1;
    while (l < h) {
        int m = (l + h) / 2;
        if (p[m] <= key) l = m + 1; else h = m;
    }
    return l - lo;
}
/* number of elements in p[lo..hi] (sorted) that are < key */
int count_lt(int *p, int lo, int hi, int key) {
    int l = lo, h = hi + 1;
    while (l < h) {
        int m = (l + h) / 2;
        if (p[m] < key) l = m + 1; else h = m;
    }
    return l - lo;
}

/* Recursively merge two adjacent sorted blocks p[lo..mid-1], p[mid..hi]
   in place using only reversal-based rotation.  Divide & conquer:
   pick the middle element of the LARGER half, binary-search its
   destination in the other half, rotate it into place in O(block)
   cost, then recurse on the two independent remaining sub-merges. */
void merge_by_reversal(int *p, int lo, int mid, int hi) {
    int m1 = mid - lo;       /* size of left block  */
    int m2 = hi - mid + 1;   /* size of right block */
    if (m1 == 0 || m2 == 0) return;
    if (m1 == 1 && m2 == 1) {
        if (p[lo] > p[mid]) reverse(p, lo, mid);
        return;
    }

    if (m1 <= m2) {
        int left_mid = lo + m1 / 2;         /* index of pivot element in left block */
        int key = p[left_mid];
        int q = count_lt(p, mid, hi, key);  /* # elements in right block strictly < key */
        int new_pos = left_mid + q;         /* final position of key after rotation */
        rotate_by_reversal(p, left_mid, mid, mid + q - 1);
        /* after rotation: p[left_mid .. left_mid+q-1] = old right-block prefix,
           p[new_pos] = key, correctly placed.
           Recurse left of it and right of it. */
        merge_by_reversal(p, lo, left_mid, new_pos - 1);
        merge_by_reversal(p, new_pos + 1, mid + q, hi);
    } else {
        int right_mid = mid + m2 / 2;
        int key = p[right_mid];
        int q = count_leq(p, lo, mid - 1, key); /* # elements in left block <= key */
        int new_pos = lo + q + (right_mid - mid);
        /* rotate so that left-block suffix (elements > key, i.e. p[lo+q..mid-1])
           moves after the right block's prefix [mid..right_mid] */
        rotate_by_reversal(p, lo + q, mid, right_mid);
        merge_by_reversal(p, lo, lo + q, new_pos - 1);
        merge_by_reversal(p, new_pos + 1, right_mid + 1, hi);
    }
}

void merge_sort_by_reversal(int *p, int lo, int hi) {
    if (lo >= hi) return;
    int mid = lo + (hi - lo) / 2 + 1;
    merge_sort_by_reversal(p, lo, mid - 1);
    merge_sort_by_reversal(p, mid, hi);
    merge_by_reversal(p, lo, mid, hi);
}

/* ------------------------------------------------------------------ */
int *random_permutation(int n, int seed) {
    int *p = (int *)malloc(n * sizeof(int));
    for (int i = 0; i < n; i++) p[i] = i + 1;
    srand(seed);
    for (int i = n - 1; i > 0; i--) { int j = rand() % (i + 1); int t = p[i]; p[i] = p[j]; p[j] = t; }
    return p;
}

double now_seconds() {
    struct timespec ts; clock_gettime(CLOCK_MONOTONIC, &ts);
    return ts.tv_sec + ts.tv_nsec / 1e9;
}

void small_demo(void) {
    int p[] = {1, 4, 3, 2, 5};
    int n = 5;
    printf("Demo (matches the problem statement's own example):\n  before: ");
    print_arr(p, n);
    reverse(p, 1, 3); /* reverse elements p2..p4 (0-indexed: 1..3) => [4,3,2]->[2,3,4] */
    printf("  after reverse(p,1,3) (0-indexed): ");
    print_arr(p, n);
    printf("  sorted? %s\n\n", is_sorted(p, n) ? "YES" : "NO");
}

void experiment_partA(int n) {
    int *p = random_permutation(n, 7);
    g_total_cost = 0; g_call_count = 0;
    double t0 = now_seconds();
    sort_by_reversal_On_reversals(p, n);
    double t1 = now_seconds();
    printf("[Part A] n=%-8d sorted=%s  #reversals=%lld (<=2(n-1)=%d)  time=%.5fs\n",
           n, is_sorted(p, n) ? "YES" : "NO", g_call_count, 2 * (n - 1), t1 - t0);
    free(p);
}

void experiment_partB(int n) {
    int *p = random_permutation(n, 11);
    g_total_cost = 0; g_call_count = 0;
    double t0 = now_seconds();
    merge_sort_by_reversal(p, 0, n - 1);
    double t1 = now_seconds();
    double predicted = (double)n * log2((double)n) * log2((double)n);
    printf("[Part B] n=%-8d sorted=%s  #reversal-calls=%lld  total-cost=%lld  "
           "(n*log2(n)^2 ~= %.0f)  time=%.5fs\n",
           n, is_sorted(p, n) ? "YES" : "NO", g_call_count, g_total_cost, predicted, t1 - t0);
    free(p);
}

/* CSV mode: n,metric,value rows for plotting.
   Part A's *runtime* is O(n^2) (n rounds, each doing an O(n) scan
   for the max) even though the *reversal count* is only O(n), so
   we cap Part A's sizes lower than Part B's to keep this fast. */
void run_csv_partA(int n) {
    int *p1 = random_permutation(n, 7);
    g_total_cost = 0; g_call_count = 0;
    sort_by_reversal_On_reversals(p1, n);
    printf("%d,partA_reversal_count,%lld\n", n, g_call_count);
    free(p1);
}

void run_csv_partB(int n) {
    int *p2 = random_permutation(n, 11);
    g_total_cost = 0; g_call_count = 0;
    merge_sort_by_reversal(p2, 0, n - 1);
    printf("%d,partB_total_cost,%lld\n", n, g_total_cost);
    free(p2);
}

int main(int argc, char **argv) {
    if (argc > 1 && strcmp(argv[1], "--csv") == 0) {
        printf("n,metric,value\n");
        int sizesA[] = {100, 500, 1000, 2000, 5000, 10000, 20000, 40000};
        for (int i = 0; i < 8; i++) run_csv_partA(sizesA[i]);

        int sizesB[] = {100, 500, 1000, 2000, 5000, 10000, 20000,
                         50000, 100000, 200000, 500000, 1000000};
        for (int i = 0; i < 12; i++) run_csv_partB(sizesB[i]);
        return 0;
    }

    printf("Q4: Sorting via reversal\n\n");
    small_demo();

    printf("---- PART A: O(n) reversal COUNT (selection-by-reversal) ----\n");
    int sizesA[] = {100, 1000, 10000, 100000};
    for (int i = 0; i < 4; i++) experiment_partA(sizesA[i]);

    printf("\n---- PART B: O(n log^2 n) reversal COST (merge-sort-by-reversal) ----\n");
    int sizesB[] = {100, 1000, 10000, 100000, 1000000};
    for (int i = 0; i < 5; i++) experiment_partB(sizesB[i]);

    printf("\nObservation: In Part A the NUMBER of reversals stays within 2(n-1)\n"
           "(confirming O(n) reversals suffice), while the total ELEMENT-MOVEMENT\n"
           "cost of Part A would actually be O(n^2) in the worst case (each\n"
           "reversal can itself cost O(n)).  Part B instead bounds the *cost*\n"
           "(sum of reversal lengths) itself to O(n log^2 n), as required,\n"
           "by doing the merge step through recursive rotation rather than a\n"
           "single full-length reversal.\n");
    return 0;
}
