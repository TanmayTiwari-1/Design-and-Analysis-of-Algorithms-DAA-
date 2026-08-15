#include <stdio.h>

void selectionSort(int A[], int n) {
    // 0-indexed in C; logic identical to the 1-indexed pseudocode above.
    for (int i = 0; i < n - 1; i++) {
        int min_idx = i;
        for (int j = i + 1; j < n; j++) {
            if (A[j] < A[min_idx]) min_idx = j;
        }
        int tmp = A[i]; A[i] = A[min_idx]; A[min_idx] = tmp;
    }
}

int main(void) {
    int n;
    printf("Enter number of elements: ");
    scanf("%d", &n);
    int A[1000];
    printf("Enter %d elements: ", n);
    for (int i = 0; i < n; i++) scanf("%d", &A[i]);

    selectionSort(A, n);

    printf("Sorted array: ");
    for (int i = 0; i < n; i++) printf("%d ", A[i]);
    printf("\n");
    return 0;
}
