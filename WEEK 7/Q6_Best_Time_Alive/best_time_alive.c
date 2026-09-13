/*
 * Q6: The Best Time to Be Alive
 * --------------------------------
 * Given the birth and death years of a set of prominent scientists, find
 * the year (or years) in which the largest number of them were alive at
 * once.  Tie-break rule given in the problem: if scientist A died in the
 * same year scientist B was born, A's death is considered to happen
 * BEFORE B's birth that year (so they do NOT count as overlapping at
 * that instant).
 *
 * INPUT REPRESENTATION
 * ---------------------
 * n scientists, each with (birthYear, deathYear).  We convert every
 * lifetime into two EVENTS:
 *      (birthYear , +1)   "a life begins"
 *      (deathYear , -1)   "a life ends"
 * and sort all 2n events by year; ties are broken by processing -1
 * events (deaths) BEFORE +1 events (births) in the same year, exactly
 * matching the rule above.
 *
 * ALGORITHM  (classic sweep-line / max overlapping intervals)
 * ---------------------------------------------------------------
 * Sort the 2n events as described, then scan them left to right keeping
 * a running counter "alive" of how many scientists are alive; add the
 * event's delta, and after each "+1" event compare "alive" against the
 * best seen so far, recording the year whenever a new maximum is set.
 *
 * COMPLEXITY
 * ----------
 *   Sorting  : O(n log n)
 *   Sweep    : O(n)
 *   Total    : O(n log n) time,  O(n) space.
 */

#include <stdio.h>
#include <stdlib.h>

typedef struct { int year; int delta; } Event;

int cmp(const void *a, const void *b) {
    Event *ea = (Event *)a, *eb = (Event *)b;
    if (ea->year != eb->year) return ea->year - eb->year;
    return ea->delta - eb->delta;  /* -1 (death) sorts before +1 (birth) */
}

int main(void) {
    /* Sample data set: (name index), birth year, death year */
    struct { const char *name; int birth, death; } sci[] = {
        {"Copernicus",      1473, 1543},
        {"Galileo",         1564, 1642},
        {"Kepler",          1571, 1630},
        {"Newton",          1643, 1727},
        {"Leibniz",         1646, 1716},
        {"Euler",           1707, 1783},
        {"Gauss",           1777, 1855},
        {"Darwin",          1809, 1882},
        {"Mendel",          1822, 1884},
        {"Einstein",        1879, 1955},
        {"Curie",           1867, 1934},
        {"Bohr",            1885, 1962},
        {"Turing",          1912, 1954},
        {"Ramanujan",       1887, 1920},
        {"Hawking",         1942, 2018}
    };
    int n = sizeof(sci) / sizeof(sci[0]);

    Event *ev = malloc(sizeof(Event) * 2 * n);
    for (int i = 0; i < n; i++) {
        ev[2 * i].year = sci[i].birth; ev[2 * i].delta = +1;
        ev[2 * i + 1].year = sci[i].death; ev[2 * i + 1].delta = -1;
    }
    qsort(ev, 2 * n, sizeof(Event), cmp);

    printf("=========================================\n");
    printf(" Best Time To Be Alive  (n = %d scientists)\n", n);
    printf("=========================================\n");

    int alive = 0, best = 0, bestYear = 0;
    for (int i = 0; i < 2 * n; i++) {
        alive += ev[i].delta;
        if (ev[i].delta == +1 && alive > best) {
            best = alive;
            bestYear = ev[i].year;
        }
    }

    printf("Maximum number of scientists alive simultaneously : %d\n", best);
    printf("Achieved starting in the year                     : %d\n", bestYear);

    printf("\nScientists alive in %d:\n", bestYear);
    for (int i = 0; i < n; i++) {
        if (sci[i].birth <= bestYear && bestYear < sci[i].death)
            printf("  %-12s (%d - %d)\n", sci[i].name, sci[i].birth, sci[i].death);
    }

    free(ev);
    return 0;
}
