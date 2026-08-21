/* Q1: Sort n (number, colour) pairs by colour - reds, then blues, then
   yellows - keeping the numbers within each colour still sorted.

   Input is ALREADY sorted by number. So we do NOT need any comparison
   sort at all. Since there are only 3 possible colours, this is really
   a STABLE BUCKETING problem (like counting sort with 3 buckets):

     1. Walk through the array once (already sorted by number) and copy
        every RED item, in the order seen, into the output.
     2. Do the same for BLUE items.
     3. Do the same for YELLOW items.

   Because we scan the original (number-sorted) array left to right and
   only ever APPEND to a bucket, the numbers inside each colour bucket
   stay in sorted order automatically (stability is free).

   Total work = 3 passes over n items = O(n). No sorting, no comparisons
   between numbers needed.
*/
#include <stdio.h>
#include <stdlib.h>

typedef enum { RED, BLUE, YELLOW } Colour;

typedef struct {
    int number;
    Colour colour;
} Item;

const char* colourName(Colour c) {
    if (c == RED) return "RED";
    if (c == BLUE) return "BLUE";
    return "YELLOW";
}

/* O(n) stable sort by colour, using 3 buckets (counting-sort style). */
void sortByColour(Item arr[], int n, Item out[]) {
    int idx = 0;
    for (Colour c = RED; c <= YELLOW; c++) {
        for (int i = 0; i < n; i++) {
            if (arr[i].colour == c) out[idx++] = arr[i];
        }
    }
}

int main(void) {
    int n;
    printf("Enter number of items: ");
    scanf("%d", &n);

    Item *arr = malloc(n * sizeof(Item));
    Item *out = malloc(n * sizeof(Item));

    printf("Enter %d items as: number colour(0=RED,1=BLUE,2=YELLOW)\n", n);
    printf("(numbers must already be given in sorted order)\n");
    for (int i = 0; i < n; i++) {
        int c;
        scanf("%d %d", &arr[i].number, &c);
        arr[i].colour = (Colour)c;
    }

    sortByColour(arr, n, out);

    printf("\nSorted by colour (RED, then BLUE, then YELLOW), numbers stay sorted within each colour:\n");
    for (int i = 0; i < n; i++)
        printf("(%d, %s)\n", out[i].number, colourName(out[i].colour));

    free(arr); free(out);
    return 0;
}
