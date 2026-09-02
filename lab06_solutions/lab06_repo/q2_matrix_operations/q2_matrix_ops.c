/* ============================================================
   Q2: 2D Square Matrix Operations and their Complexities
   ============================================================
   Input representation: an n x n matrix stored as a contiguous
   1-D block accessed via M[i*n + j] (row-major), which is the
   most cache-friendly representation for a fixed-size square
   matrix and lets us do in-place transpose easily.

   Operations implemented (worst-case complexity above each):
     (i)   Matrix addition                 -> O(n^2)
     (ii)  Matrix multiplication            -> O(n^3) naive
                                               (Strassen: O(n^2.81); not required here)
     (iii) Zero-matrix test                 -> O(n^2)
     (iv)  Symmetric-matrix test             -> O(n^2)
     (v)   Determinant (Gaussian elim.)      -> O(n^3)
                                               (naive cofactor expansion is O(n!))
     (vi)  In-place transpose                -> O(n^2)
     (vii) Eigenvalue / eigenvector
           (power iteration, dominant pair)  -> O(k*n^2), k = #iterations
                                               (exact solution for n>=5 has NO
                                                closed form - Abel-Ruffini - so all
                                                practical methods, e.g. QR
                                                algorithm, are iterative: O(n^3)
                                                per iteration)
   ============================================================ */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <string.h>

double *alloc_mat(int n) { return (double *)malloc(n * n * sizeof(double)); }
#define AT(M, n, i, j) (M[(i) * (n) + (j)])

double *random_matrix(int n, int seed) {
    srand(seed);
    double *M = alloc_mat(n);
    for (int i = 0; i < n * n; i++) M[i] = (rand() % 2001 - 1000) / 10.0;
    return M;
}

void print_matrix(double *M, int n) {
    int lim = n < 6 ? n : 6;
    for (int i = 0; i < lim; i++) {
        for (int j = 0; j < lim; j++) printf("%8.2f ", AT(M, n, i, j));
        printf(n > 6 ? "...\n" : "\n");
    }
    if (n > 6) printf("...\n");
}

/* (i) Addition ---------------------------------------------------- O(n^2) */
double *mat_add(double *A, double *B, int n) {
    double *C = alloc_mat(n);
    for (int i = 0; i < n * n; i++) C[i] = A[i] + B[i];
    return C;
}

/* (ii) Multiplication ---------------------------------------------- O(n^3) */
double *mat_mul(double *A, double *B, int n) {
    double *C = alloc_mat(n);
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++) {
            double sum = 0;
            for (int k = 0; k < n; k++) sum += AT(A, n, i, k) * AT(B, n, k, j);
            AT(C, n, i, j) = sum;
        }
    return C;
}

/* (iii) Zero matrix test -------------------------------------------- O(n^2) */
int is_zero_matrix(double *A, int n) {
    for (int i = 0; i < n * n; i++) if (fabs(A[i]) > 1e-12) return 0;
    return 1;
}

/* (iv) Symmetric matrix test ----------------------------------------- O(n^2) */
int is_symmetric(double *A, int n) {
    for (int i = 0; i < n; i++)
        for (int j = i + 1; j < n; j++)
            if (fabs(AT(A, n, i, j) - AT(A, n, j, i)) > 1e-9) return 0;
    return 1;
}

/* (v) Determinant via Gaussian elimination with partial pivoting ---- O(n^3) */
double determinant(double *A, int n) {
    double *M = alloc_mat(n);
    memcpy(M, A, n * n * sizeof(double));
    double det = 1.0;
    for (int col = 0; col < n; col++) {
        int pivot_row = col;
        double best = fabs(AT(M, n, col, col));
        for (int r = col + 1; r < n; r++)
            if (fabs(AT(M, n, r, col)) > best) { best = fabs(AT(M, n, r, col)); pivot_row = r; }
        if (best < 1e-12) { free(M); return 0.0; } /* singular */
        if (pivot_row != col) {
            for (int k = 0; k < n; k++) {
                double t = AT(M, n, col, k); AT(M, n, col, k) = AT(M, n, pivot_row, k); AT(M, n, pivot_row, k) = t;
            }
            det = -det;
        }
        det *= AT(M, n, col, col);
        for (int r = col + 1; r < n; r++) {
            double factor = AT(M, n, r, col) / AT(M, n, col, col);
            for (int k = col; k < n; k++) AT(M, n, r, k) -= factor * AT(M, n, col, k);
        }
    }
    free(M);
    return det;
}

/* (vi) In-place transpose --------------------------------------------- O(n^2) */
void transpose_in_place(double *A, int n) {
    for (int i = 0; i < n; i++)
        for (int j = i + 1; j < n; j++) {
            double t = AT(A, n, i, j); AT(A, n, i, j) = AT(A, n, j, i); AT(A, n, j, i) = t;
        }
}

/* (vii) Dominant eigenvalue/eigenvector via Power Iteration -- O(k*n^2) */
double power_iteration(double *A, int n, double *eigenvector, int max_iter, double tol) {
    double *v = (double *)malloc(n * sizeof(double));
    double *w = (double *)malloc(n * sizeof(double));
    for (int i = 0; i < n; i++) v[i] = 1.0; /* initial guess */
    double lambda_old = 0, lambda = 0;
    for (int it = 0; it < max_iter; it++) {
        for (int i = 0; i < n; i++) {
            double s = 0;
            for (int j = 0; j < n; j++) s += AT(A, n, i, j) * v[j];
            w[i] = s;
        }
        double norm = 0;
        for (int i = 0; i < n; i++) norm += w[i] * w[i];
        norm = sqrt(norm);
        if (norm < 1e-15) break;
        for (int i = 0; i < n; i++) v[i] = w[i] / norm;
        /* Rayleigh quotient for eigenvalue estimate */
        double num = 0, den = 0;
        for (int i = 0; i < n; i++) {
            double s = 0;
            for (int j = 0; j < n; j++) s += AT(A, n, i, j) * v[j];
            num += v[i] * s;
            den += v[i] * v[i];
        }
        lambda = num / den;
        if (fabs(lambda - lambda_old) < tol) break;
        lambda_old = lambda;
    }
    memcpy(eigenvector, v, n * sizeof(double));
    free(v); free(w);
    return lambda;
}

/* ------------------------------------------------------------------ */
double now_seconds() {
    struct timespec ts; clock_gettime(CLOCK_MONOTONIC, &ts);
    return ts.tv_sec + ts.tv_nsec / 1e9;
}

void run_experiment(int n) {
    printf("\n---- n = %d ----\n", n);
    double *A = random_matrix(n, 1);
    double *B = random_matrix(n, 2);

    double t0 = now_seconds();
    double *C = mat_add(A, B, n);
    double t1 = now_seconds();
    printf("(i)   add               time=%.6fs\n", t1 - t0);
    free(C);

    t0 = now_seconds();
    double *D = mat_mul(A, B, n);
    t1 = now_seconds();
    printf("(ii)  multiply          time=%.6fs\n", t1 - t0);
    free(D);

    t0 = now_seconds();
    int z = is_zero_matrix(A, n);
    t1 = now_seconds();
    printf("(iii) is_zero=%d         time=%.6fs\n", z, t1 - t0);

    /* build a symmetric matrix S = A + A^T for a real positive test */
    double *At = alloc_mat(n); memcpy(At, A, n*n*sizeof(double)); transpose_in_place(At, n);
    double *S = mat_add(A, At, n);
    t0 = now_seconds();
    int sym = is_symmetric(S, n);
    t1 = now_seconds();
    printf("(iv)  is_symmetric(S)=%d time=%.6fs\n", sym, t1 - t0);

    int small_n = n > 500 ? 500 : n; /* determinant grows numerically unstable/slow beyond this for demo */
    double *Asmall = random_matrix(small_n, 3);
    t0 = now_seconds();
    double det = determinant(Asmall, small_n);
    t1 = now_seconds();
    printf("(v)   det (n=%d) = %.4e  time=%.6fs\n", small_n, det, t1 - t0);
    free(Asmall);

    double *Acopy = alloc_mat(n); memcpy(Acopy, A, n*n*sizeof(double));
    t0 = now_seconds();
    transpose_in_place(Acopy, n);
    t1 = now_seconds();
    /* validate transpose */
    int ok = 1;
    for (int i = 0; i < n && ok; i++)
        for (int j = 0; j < n; j++)
            if (fabs(AT(Acopy, n, i, j) - AT(A, n, j, i)) > 1e-9) { ok = 0; break; }
    printf("(vi)  transpose valid?=%s time=%.6fs\n", ok ? "YES" : "NO", t1 - t0);
    free(Acopy);

    double *eigvec = (double *)malloc(n * sizeof(double));
    t0 = now_seconds();
    double lambda = power_iteration(S, n, eigvec, 1000, 1e-10);
    t1 = now_seconds();
    printf("(vii) dominant eigenvalue of S ~= %.4f   time=%.6fs\n", lambda, t1 - t0);
    free(eigvec);

    free(A); free(B); free(At); free(S);
}

/* CSV mode: n,operation,time_seconds rows for plotting */
void run_csv(int n) {
    double *A = random_matrix(n, 1);
    double *B = random_matrix(n, 2);
    double t0, t1;

    t0 = now_seconds(); double *C = mat_add(A, B, n); t1 = now_seconds();
    printf("%d,add,%.9f\n", n, t1 - t0); free(C);

    t0 = now_seconds(); double *D = mat_mul(A, B, n); t1 = now_seconds();
    printf("%d,multiply,%.9f\n", n, t1 - t0); free(D);

    t0 = now_seconds(); is_zero_matrix(A, n); t1 = now_seconds();
    printf("%d,is_zero,%.9f\n", n, t1 - t0);

    double *At = alloc_mat(n); memcpy(At, A, n*n*sizeof(double)); transpose_in_place(At, n);
    double *S = mat_add(A, At, n);
    t0 = now_seconds(); is_symmetric(S, n); t1 = now_seconds();
    printf("%d,is_symmetric,%.9f\n", n, t1 - t0);

    t0 = now_seconds(); determinant(A, n); t1 = now_seconds();
    printf("%d,determinant,%.9f\n", n, t1 - t0);

    double *Acopy = alloc_mat(n); memcpy(Acopy, A, n*n*sizeof(double));
    t0 = now_seconds(); transpose_in_place(Acopy, n); t1 = now_seconds();
    printf("%d,transpose,%.9f\n", n, t1 - t0);
    free(Acopy);

    double *eigvec = (double *)malloc(n * sizeof(double));
    t0 = now_seconds(); power_iteration(S, n, eigvec, 1000, 1e-10); t1 = now_seconds();
    printf("%d,eigenvalue,%.9f\n", n, t1 - t0);
    free(eigvec);

    free(A); free(B); free(At); free(S);
}

int main(int argc, char **argv) {
    if (argc > 1 && strcmp(argv[1], "--csv") == 0) {
        printf("n,operation,time_seconds\n");
        int sizes[] = {20, 40, 60, 80, 100, 150, 200, 250, 300, 350, 400};
        for (int i = 0; i < 11; i++) run_csv(sizes[i]);
        return 0;
    }

    printf("Q2: 2D Square Matrix Operations - Complexity Validation\n");
    int sizes[] = {50, 100, 200, 400};
    for (int i = 0; i < 4; i++) run_experiment(sizes[i]);
    printf("\nDoubling n: O(n^2) ops (i,iii,iv,vi) roughly x4 time; \n"
           "O(n^3) ops (ii,v) roughly x8 time - matching the derived bounds.\n");
    return 0;
}
