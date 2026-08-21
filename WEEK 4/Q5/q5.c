/* Q5: Given n intervals (x_i, y_i), merge all overlapping ones.
   Example: {(1,3),(2,6),(8,10),(7,18)} -> {(1,6),(7,18)}

   Idea:
     1. Sort intervals by their START value x_i.          -> O(n log n)
     2. Scan left to right, keeping a "current merged interval".
        If the next interval's start <= current interval's end, they
        overlap -> extend current end to max(current end, next end).
        Otherwise, the current merged interval is finished -> output it,
        and start a new current interval from the next one.  -> O(n)
   Total: O(n log n).
*/
#include <stdio.h>
#include <stdlib.h>

typedef struct { int x, y; } Interval;

int cmp(const void *a, const void *b) {
    return ((Interval*)a)->x - ((Interval*)b)->x;
}

/* Merges intervals in-place logic; writes result into 'out', returns count. */
int mergeIntervals(Interval arr[], int n, Interval out[]) {
    if (n == 0) return 0;
    qsort(arr, n, sizeof(Interval), cmp);

    int count = 0;
    Interval cur = arr[0];
    for (int i = 1; i < n; i++) {
        if (arr[i].x <= cur.y) {              // overlap
            if (arr[i].y > cur.y) cur.y = arr[i].y;
        } else {                              // no overlap, close current
            out[count++] = cur;
            cur = arr[i];
        }
    }
    out[count++] = cur;
    return count;
}

int main(void) {
    int n;
    printf("Enter number of intervals: ");
    scanf("%d", &n);

    Interval *arr = malloc(n * sizeof(Interval));
    Interval *out = malloc(n * sizeof(Interval));

    printf("Enter %d intervals as: x y\n", n);
    for (int i = 0; i < n; i++) scanf("%d %d", &arr[i].x, &arr[i].y);

    int count = mergeIntervals(arr, n, out);

    printf("Merged intervals: ");
    for (int i = 0; i < count; i++) printf("(%d, %d) ", out[i].x, out[i].y);
    printf("\n");

    free(arr); free(out);
    return 0;
}
