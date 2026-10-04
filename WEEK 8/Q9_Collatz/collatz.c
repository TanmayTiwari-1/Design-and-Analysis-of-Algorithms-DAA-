/* Q9: Collatz Conjecture analyser (modular C program)
   Input (stdin):  1 n      -> trajectory of a single start value n
                   2 a b    -> statistics over the interval [a, b]
   Files written: trajectory.csv (mode 1), collatz_interval.csv (mode 2)
   Overflow: 3n+1 is checked against ULLONG_MAX before it is computed.   */
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

typedef unsigned long long ull;

/* ---- module 1: one Collatz step. returns 0 on overflow, 1 otherwise ---- */
int collatz_next(ull n, ull *out) {
    if (n % 2 == 0) { *out = n / 2; return 1; }
    if (n > (ULLONG_MAX - 1) / 3) return 0;          /* 3n+1 would overflow */
    *out = 3 * n + 1;
    return 1;
}

/* ---- module 2: steps to reach 1 and peak value (no storage) ---- */
int analyze(ull n, ull *steps, ull *peak) {
    *steps = 0; *peak = n;
    while (n != 1) {
        if (!collatz_next(n, &n)) return 0;
        (*steps)++;
        if (n > *peak) *peak = n;
    }
    return 1;
}

/* ---- module 3: full trajectory in a dynamically grown array ---- */
ull *trajectory(ull n, size_t *len) {
    size_t cap = 64; *len = 0;
    ull *t = malloc(cap * sizeof(ull));
    if (!t) return NULL;
    t[(*len)++] = n;
    while (n != 1) {
        if (!collatz_next(n, &n)) { free(t); return NULL; }   /* overflow */
        if (*len == cap) {
            cap *= 2;
            ull *tmp = realloc(t, cap * sizeof(ull));
            if (!tmp) { free(t); return NULL; }
            t = tmp;
        }
        t[(*len)++] = n;
    }
    return t;
}

/* ---- module 4: single start value ---- */
void run_single(ull n) {
    size_t len;
    ull *t = trajectory(n, &len);
    if (!t) { printf("Overflow (or out of memory) while processing %llu\n", n); return; }
    ull peak = t[0];
    FILE *f = fopen("trajectory.csv", "w");
    if (f) fprintf(f, "index,value\n");
    printf("Trajectory of %llu:\n", n);
    for (size_t i = 0; i < len; i++) {
        printf("%llu%s", t[i], i + 1 < len ? " -> " : "\n");
        if (t[i] > peak) peak = t[i];
        if (f) fprintf(f, "%zu,%llu\n", i, t[i]);
    }
    printf("Steps to reach 1 = %zu, peak value = %llu\n", len - 1, peak);
    if (f) { fclose(f); printf("(saved trajectory.csv)\n"); }
    free(t);
}

/* ---- module 5: interval [a,b] ---- */
void run_interval(ull a, ull b) {
    ull count = b - a + 1;
    ull *steps = malloc(count * sizeof(ull));         /* dynamic result table */
    ull *peaks = malloc(count * sizeof(ull));
    if (!steps || !peaks) { printf("Out of memory\n"); free(steps); free(peaks); return; }
    ull best_n = a, best_steps = 0, peak_n = a, peak_v = 0, total = 0, overflow = 0;
    for (ull n = a; n <= b; n++) {
        ull s, p;
        if (!analyze(n, &s, &p)) { overflow++; steps[n - a] = peaks[n - a] = 0; continue; }
        steps[n - a] = s; peaks[n - a] = p; total += s;
        if (s > best_steps) { best_steps = s; best_n = n; }
        if (p > peak_v)     { peak_v = p;     peak_n = n; }
    }
    printf("Interval [%llu, %llu]\n", a, b);
    printf("Longest trajectory : n = %llu with %llu steps\n", best_n, best_steps);
    printf("Highest peak       : n = %llu reaches %llu\n", peak_n, peak_v);
    printf("Average steps      : %.2f\n", (double)total / (double)(count - overflow ? count - overflow : 1));
    if (overflow) printf("Overflow occurred for %llu start values (skipped)\n", overflow);
    FILE *f = fopen("collatz_interval.csv", "w");
    if (f) {
        fprintf(f, "n,steps,peak\n");
        for (ull i = 0; i < count; i++) fprintf(f, "%llu,%llu,%llu\n", a + i, steps[i], peaks[i]);
        fclose(f); printf("(saved collatz_interval.csv)\n");
    }
    free(steps); free(peaks);
}

int main(void) {
    int mode; ull a, b;
    if (scanf("%d", &mode) != 1) { printf("Bad input\n"); return 1; }
    if (mode == 1) {
        if (scanf("%llu", &a) != 1 || a < 1) { printf("n must be >= 1\n"); return 1; }
        run_single(a);
    } else if (mode == 2) {
        if (scanf("%llu %llu", &a, &b) != 2 || a < 1 || b < a) { printf("Need 1 <= a <= b\n"); return 1; }
        run_interval(a, b);
    } else { printf("Mode must be 1 or 2\n"); return 1; }
    return 0;
}
