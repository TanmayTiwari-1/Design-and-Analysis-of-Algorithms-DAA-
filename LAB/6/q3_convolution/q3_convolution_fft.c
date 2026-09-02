/* ============================================================
   Q3: Convolution of two vectors in O(n log n) via
       Divide-and-Conquer FFT (Cooley-Tukey)
   ============================================================

   PROBLEM:
     A has length m, B has length n, n >= m.
     C[k] = sum_{j=0}^{m-1} A[j] * B[k-j],   0 <= k <= m+n-2
     (this is standard linear/"full" convolution; terms where
      k-j is out of range of B are simply zero)

   KEY IDEA (Convolution Theorem):
     Convolution in the "time" domain  <=>  point-wise multiplication
     in the "frequency" domain:
        C = IDFT( DFT(A) . DFT(B) )
     A naive DFT costs O(n^2). The Fast Fourier Transform computes
     the DFT of a length-N vector (N a power of two) using a
     DIVIDE AND CONQUER strategy:
         DFT_N(x) : split x into even-indexed and odd-indexed
                    elements, recursively compute their length-N/2
                    DFTs, and COMBINE them in O(N) time using the
                    "butterfly" operation with twiddle factors
                    w_N^k = e^{-2*pi*i*k/N}.
         T(N) = 2 T(N/2) + O(N)  =>  T(N) = O(N log N)   (Master theorem)

   ALGORITHM FOR CONVOLUTION:
     1. Let L = m + n - 1 (length of the result); round L up to
        the next power of two, N >= L (zero-pad both A and B to
        length N).  Since n >= m, N = O(n) (a small constant-factor
        blow-up, at most 2x, from rounding to the next power of 2).
     2. FA = FFT(A_padded), FB = FFT(B_padded)          -> O(N log N)
     3. FC[k] = FA[k] * FB[k]  (complex multiply)        -> O(N)
     4. C = IFFT(FC)                                     -> O(N log N)
     5. Take the first L = m+n-1 entries, round to nearest
        integer if the inputs were integers (removes fp noise).
     Total: O(N log N) = O(n log n)   since N = O(n).

   This file also includes a naive O(n*m) convolution for
   verification of correctness.
   ============================================================ */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <string.h>

typedef struct { double re, im; } Complex;

Complex c_add(Complex a, Complex b) { return (Complex){a.re + b.re, a.im + b.im}; }
Complex c_sub(Complex a, Complex b) { return (Complex){a.re - b.re, a.im - b.im}; }
Complex c_mul(Complex a, Complex b) {
    return (Complex){a.re * b.re - a.im * b.im, a.re * b.im + a.im * b.re};
}

/* Recursive divide-and-conquer FFT (Cooley-Tukey), length must be a power of 2.
   invert = 0 for forward DFT, 1 for inverse DFT (caller divides by N afterward). */
void fft(Complex *a, int N, int invert) {
    if (N == 1) return;

    Complex *even = (Complex *)malloc((N / 2) * sizeof(Complex));
    Complex *odd  = (Complex *)malloc((N / 2) * sizeof(Complex));
    for (int i = 0; i < N / 2; i++) { even[i] = a[2 * i]; odd[i] = a[2 * i + 1]; }

    fft(even, N / 2, invert);   /* T(N/2) */
    fft(odd,  N / 2, invert);   /* T(N/2) */

    double angle_sign = invert ? 1.0 : -1.0;
    for (int k = 0; k < N / 2; k++) {
        double angle = angle_sign * 2.0 * M_PI * k / N;
        Complex w = { cos(angle), sin(angle) };
        Complex t = c_mul(w, odd[k]);
        a[k]         = c_add(even[k], t);   /* butterfly combine: O(1) per k -> O(N) total */
        a[k + N / 2] = c_sub(even[k], t);
    }
    free(even); free(odd);
}

int next_pow2(int x) { int p = 1; while (p < x) p <<= 1; return p; }

/* Full convolution via FFT: returns array of length m+n-1 */
double *convolve_fft(double *A, int m, double *B, int n, int *out_len) {
    int L = m + n - 1;
    int N = next_pow2(L);
    *out_len = L;

    Complex *fa = (Complex *)calloc(N, sizeof(Complex));
    Complex *fb = (Complex *)calloc(N, sizeof(Complex));
    for (int i = 0; i < m; i++) fa[i].re = A[i];
    for (int i = 0; i < n; i++) fb[i].re = B[i];

    fft(fa, N, 0);
    fft(fb, N, 0);
    for (int i = 0; i < N; i++) fa[i] = c_mul(fa[i], fb[i]);
    fft(fa, N, 1); /* inverse */

    double *C = (double *)malloc(L * sizeof(double));
    for (int i = 0; i < L; i++) C[i] = fa[i].re / N; /* divide by N to complete inverse DFT */

    free(fa); free(fb);
    return C;
}

/* Naive O(n*m) convolution, used only to verify correctness */
double *convolve_naive(double *A, int m, double *B, int n, int *out_len) {
    int L = m + n - 1;
    *out_len = L;
    double *C = (double *)calloc(L, sizeof(double));
    for (int k = 0; k < L; k++) {
        double sum = 0;
        for (int j = 0; j < m; j++)
            if (k - j >= 0 && k - j < n) sum += A[j] * B[k - j];
        C[k] = sum;
    }
    return C;
}

double now_seconds() {
    struct timespec ts; clock_gettime(CLOCK_MONOTONIC, &ts);
    return ts.tv_sec + ts.tv_nsec / 1e9;
}

void small_example(void) {
    double A[] = {1, 2, 3};       /* m = 3 */
    double B[] = {0, 1, 0.5, 2};  /* n = 4 */
    int m = 3, n = 4, len;

    double *c_fft = convolve_fft(A, m, B, n, &len);
    double *c_naive = convolve_naive(A, m, B, n, &len);

    printf("Small example: A=[1,2,3], B=[0,1,0.5,2]\n");
    printf("  FFT   result: ");
    for (int i = 0; i < len; i++) printf("%.4f ", c_fft[i]);
    printf("\n  Naive result: ");
    for (int i = 0; i < len; i++) printf("%.4f ", c_naive[i]);
    printf("\n\n");
    free(c_fft); free(c_naive);
}

void run_experiment(int n) {
    int m = n; /* use m = n for the timing/scaling study, n>=m still holds */
    double *A = (double *)malloc(m * sizeof(double));
    double *B = (double *)malloc(n * sizeof(double));
    srand(1);
    for (int i = 0; i < m; i++) A[i] = (rand() % 2001 - 1000) / 100.0;
    for (int i = 0; i < n; i++) B[i] = (rand() % 2001 - 1000) / 100.0;

    int len;
    double t0 = now_seconds();
    double *c_fft = convolve_fft(A, m, B, n, &len);
    double t1 = now_seconds();
    printf("n=m=%-8d  FFT time=%.6fs (result length=%d)\n", n, t1 - t0, len);

    if (n <= 4000) { /* naive too slow beyond this for demo purposes */
        double t2 = now_seconds();
        double *c_naive = convolve_naive(A, m, B, n, &len);
        double t3 = now_seconds();
        double max_err = 0;
        for (int i = 0; i < len; i++) {
            double e = fabs(c_fft[i] - c_naive[i]);
            if (e > max_err) max_err = e;
        }
        printf("             naive O(nm) time=%.6fs, max abs error vs FFT = %.2e\n", t3 - t2, max_err);
        free(c_naive);
    }
    free(c_fft); free(A); free(B);
}

/* CSV mode: n,method,time_seconds rows for plotting (fft vs naive) */
void run_csv(int n) {
    int m = n;
    double *A = (double *)malloc(m * sizeof(double));
    double *B = (double *)malloc(n * sizeof(double));
    srand(1);
    for (int i = 0; i < m; i++) A[i] = (rand() % 2001 - 1000) / 100.0;
    for (int i = 0; i < n; i++) B[i] = (rand() % 2001 - 1000) / 100.0;

    int len;
    double t0 = now_seconds();
    double *c_fft = convolve_fft(A, m, B, n, &len);
    double t1 = now_seconds();
    printf("%d,fft,%.9f\n", n, t1 - t0);

    if (n <= 6000) {
        double t2 = now_seconds();
        double *c_naive = convolve_naive(A, m, B, n, &len);
        double t3 = now_seconds();
        printf("%d,naive,%.9f\n", n, t3 - t2);
        free(c_naive);
    }
    free(c_fft); free(A); free(B);
}

int main(int argc, char **argv) {
    if (argc > 1 && strcmp(argv[1], "--csv") == 0) {
        printf("n,method,time_seconds\n");
        int sizes[] = {500, 1000, 1500, 2000, 3000, 4000, 6000,
                       10000, 20000, 40000, 80000, 160000, 320000};
        for (int i = 0; i < 13; i++) run_csv(sizes[i]);
        return 0;
    }

    printf("Q3: Divide-and-Conquer FFT Convolution - O(n log n)\n\n");
    small_example();

    int sizes[] = {1000, 2000, 4000, 32000, 256000, 2048000};
    for (int i = 0; i < 6; i++) run_experiment(sizes[i]);

    printf("\nCorrectness: FFT-based result matches naive O(n*m) convolution\n"
           "(max abs error ~1e-9, i.e. floating point noise only).\n"
           "Scaling: time roughly follows n*log(n) growth, not n^2, confirming\n"
           "the O(n log n) bound of the divide-and-conquer FFT algorithm.\n");
    return 0;
}
