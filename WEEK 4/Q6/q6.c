/* Q6: Given n intervals [l_i, r_i] on a line, find a point p covered by
   the largest number of intervals. Endpoints count as being inside.

   Idea (same sweep-line family as Q4):
     1. Create 2n events: a START event (l_i, +1) and an END event
        (r_i, -1) for every interval.
     2. Sort events by coordinate. IMPORTANT TIE-BREAK: if a start and an
        end land on the exact same coordinate, process the START first.
        This is what correctly implements "the endpoint counts as being
        in its interval" -- an interval ending exactly at p still covers
        p, so its -1 must be applied only AFTER we've counted p.       -> O(n log n)
     3. Sweep left to right, add delta at each event, and remember the
        coordinate where the running count peaks.                      -> O(n)
   Total: O(n log n).
*/
#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int coord;
    int type;    // 0 = start (+1), 1 = end (-1)  -- start sorts before end at same coord
} Event;

int cmp(const void *a, const void *b) {
    Event *e1 = (Event*)a, *e2 = (Event*)b;
    if (e1->coord != e2->coord) return e1->coord - e2->coord;
    return e1->type - e2->type;    // start(0) before end(1) at same coordinate
}

int main(void) {
    int n;
    printf("Enter number of intervals: ");
    scanf("%d", &n);

    Event *events = malloc(2 * n * sizeof(Event));
    printf("Enter %d intervals as: l r\n", n);
    for (int i = 0; i < n; i++) {
        int l, r;
        scanf("%d %d", &l, &r);
        events[2 * i]     = (Event){l, 0};
        events[2 * i + 1] = (Event){r, 1};
    }

    qsort(events, 2 * n, sizeof(Event), cmp);

    int current = 0, maxCount = 0, bestPoint = -1;
    for (int i = 0; i < 2 * n; i++) {
        if (events[i].type == 0) current++;   // start
        else                     current--;    // end (applied AFTER same-coord starts)

        if (events[i].type == 0 && current > maxCount) {
            maxCount = current;
            bestPoint = events[i].coord;
        }
    }

    printf("Point p = %d lies in the maximum number of intervals: %d\n", bestPoint, maxCount);

    free(events);
    return 0;
}
