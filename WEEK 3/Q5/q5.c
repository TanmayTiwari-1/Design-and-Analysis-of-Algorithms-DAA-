#include <stdio.h>
#include <stdlib.h>

typedef double** Matrix;

Matrix allocMat(int n) {
    Matrix m = malloc(n * sizeof(double*));
    for (int i = 0; i < n; i++) m[i] = calloc(n, sizeof(double));
    return m;
}
void freeMat(Matrix m, int n) { for (int i = 0; i < n; i++) free(m[i]); free(m); }

/* Extract M1 (top-left h x h) and M2 (top-right h x h) blocks of an n x n
   special-pattern matrix M (bottom half is redundant by the pattern). */
void splitBlocks(Matrix M, int n, Matrix M1, Matrix M2) {
    int h = n / 2;
    for (int i = 0; i < h; i++)
        for (int j = 0; j < h; j++) {
            M1[i][j] = M[i][j];
            M2[i][j] = M[i][j + h];
        }
}
Matrix combine(Matrix A, Matrix B, int h, int sign) {
    Matrix R = allocMat(h);
    for (int i = 0; i < h; i++)
        for (int j = 0; j < h; j++)
            R[i][j] = A[i][j] + sign * B[i][j];
    return R;
}

/* Multiplies two h x h "ordinary" matrices only used as the base case when
   h==1 within the recursive special multiply; kept for clarity/testing. */

/* Recursively multiply two n x n special-pattern matrices; returns full
   n x n result (which is itself guaranteed to be special-pattern). */
Matrix specialMultiply(Matrix M, Matrix N, int n) {
    Matrix C = allocMat(n);
    if (n == 1) {
        C[0][0] = M[0][0] * N[0][0];
        return C;
    }
    int h = n / 2;
    Matrix M1 = allocMat(h), M2 = allocMat(h), N1 = allocMat(h), N2 = allocMat(h);
    splitBlocks(M, n, M1, M2);
    splitBlocks(N, n, N1, N2);

    Matrix Ms = combine(M1, M2, h, 1), Md = combine(M1, M2, h, -1);
    Matrix Ns = combine(N1, N2, h, 1), Nd = combine(N1, N2, h, -1);

    Matrix S = specialMultiply(Ms, Ns, h);   // (M1+M2)*(N1+N2)
    Matrix D = specialMultiply(Md, Nd, h);   // (M1-M2)*(N1-N2)

    Matrix P1 = allocMat(h), P2 = allocMat(h);
    for (int i = 0; i < h; i++)
        for (int j = 0; j < h; j++) {
            P1[i][j] = (S[i][j] + D[i][j]) / 2.0;   // M1N1+M2N2
            P2[i][j] = (S[i][j] - D[i][j]) / 2.0;   // M1N2+M2N1
        }

    for (int i = 0; i < h; i++)
        for (int j = 0; j < h; j++) {
            C[i][j]         = P1[i][j];
            C[i][j + h]     = P2[i][j];
            C[i + h][j]     = P2[i][j];
            C[i + h][j + h] = P1[i][j];
        }

    freeMat(M1,h); freeMat(M2,h); freeMat(N1,h); freeMat(N2,h);
    freeMat(Ms,h); freeMat(Md,h); freeMat(Ns,h); freeMat(Nd,h);
    freeMat(S,h); freeMat(D,h); freeMat(P1,h); freeMat(P2,h);
    return C;
}

int main(void) {
    int n;
    printf("Enter n (power of 2): ");
    scanf("%d", &n);

    Matrix A = allocMat(n), B = allocMat(n);
    printf("Enter matrix A (%d x %d), must satisfy the [[M1,M2],[M2,M1]] pattern:\n", n, n);
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++) scanf("%lf", &A[i][j]);
    printf("Enter matrix B (%d x %d), same pattern:\n", n, n);
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++) scanf("%lf", &B[i][j]);

    Matrix C = specialMultiply(A, B, n);

    printf("Result C = A x B:\n");
    for (int i = 0; i < n; i++) {

        for (int j = 0; j < n; j++) printf("%.1f ", C[i][j]);
        printf("\n");
    }

    freeMat(A, n); freeMat(B, n); freeMat(C, n);
    return 0;
}
