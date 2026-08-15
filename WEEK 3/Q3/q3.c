#include <stdio.h>
#include <stdlib.h>

long comparisons = 0;

typedef struct { int mx, mn; } Pair;

Pair maxMin(int arr[], int lo, int hi) {
    int n = hi - lo + 1;
    Pair result;

    if (n == 1) {
        result.mx = result.mn = arr[lo];
        return result;
    }
    if (n == 2) {
        comparisons++;
        if (arr[lo] > arr[hi]) { result.mx = arr[lo]; result.mn = arr[hi]; }
        else                   { result.mx = arr[hi]; result.mn = arr[lo]; }
        return result;
    }

    int mid = lo + (hi - lo) / 2;
    Pair left  = maxMin(arr, lo, mid);
    Pair right = maxMin(arr, mid + 1, hi);

    comparisons++;
    result.mx = (left.mx > right.mx) ? left.mx : right.mx;
    comparisons++;
    result.mn = (left.mn < right.mn) ? left.mn : right.mn;
    return result;
}

int main(void) {
    int n;
    printf("Enter number of elements: ");
    scanf("%d", &n);
    int *arr = malloc(n * sizeof(int));
    printf("Enter %d elements: ", n);
    for (int i = 0; i < n; i++) scanf("%d", &arr[i]);

    comparisons = 0;
    Pair res = maxMin(arr, 0, n - 1);

    printf("Maximum = %d, Minimum = %d\n", res.mx, res.mn);
    printf("Comparisons used = %ld (bound 3n/2 = %.1f)\n", comparisons, 1.5 * n);

    free(arr);
    return 0;
}
