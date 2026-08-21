/* Q4: n people, person i present during [a_i, b_i]. Find the time when
   the most people were simultaneously present, in O(n log n).

   Idea (event / sweep-line technique):
     1. Create 2n "events": an ENTRY event (a_i, +1) for every person's
        arrival, and an EXIT event (b_i, -1) for every departure.
     2. Sort all 2n events by time.                       -> O(n log n)
     3. Sweep left to right, maintaining a running "current count".
        Add +1 on an entry event, -1 on an exit event. Track the maximum
        value reached, and the time at which it was reached.           -> O(n)
   Total: O(n log n).

   Since all times are distinct (given), we don't need special tie-
   breaking rules between entry/exit events at the same instant.
*/
#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int time;
    int delta;   // +1 for entry, -1 for exit
} Event;

int cmp(const void *a, const void *b) {
    return ((Event*)a)->time - ((Event*)b)->time;
}

int main(void) {
    int n;
    printf("Enter number of people: ");
    scanf("%d", &n);

    Event *events = malloc(2 * n * sizeof(Event));
    printf("Enter %d (entry exit) pairs:\n", n);
    for (int i = 0; i < n; i++) {
        int a, b;
        scanf("%d %d", &a, &b);
        events[2 * i]     = (Event){a, +1};
        events[2 * i + 1] = (Event){b, -1};
    }

    qsort(events, 2 * n, sizeof(Event), cmp);

    int current = 0, maxCount = 0, bestTime = -1;
    for (int i = 0; i < 2 * n; i++) {
        current += events[i].delta;
        if (current > maxCount) {
            maxCount = current;
            bestTime = events[i].time;
        }
    }

    printf("Maximum simultaneous people present = %d, first reached at time = %d\n",
           maxCount, bestTime);

    free(events);
    return 0;
}
