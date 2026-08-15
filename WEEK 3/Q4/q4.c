#include <stdio.h>
#include <stdlib.h>

typedef int** Matrix;

Matrix allocMat(int n) {
    Matrix m = malloc(n * sizeof(int*));
    for (int i = 0; i < n; i++) m[i] = calloc(n, sizeof(int));
    return m;
}
void freeMat(Matrix m, int n) {
    for (int i = 0; i < n; i++) free(m[i]);
    free(m);
}
Matrix addMat(Matrix A, Matrix B, int n, int sign) {
    Matrix C = allocMat(n);
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            C[i][j] = A[i][j] + sign * B[i][j];
    return C;
}

void splitMat(Matrix M, int n, Matrix M11, Matrix M12, Matrix M21, Matrix M22) {
    int h = n / 2;
    for (int i = 0; i < h; i++)
        for (int j = 0; j < h; j++) {
            M11[i][j] = M[i][j];
            M12[i][j] = M[i][j + h];
            M21[i][j] = M[i + h][j];
            M22[i][j] = M[i + h][j + h];
        }
}
void joinMat(Matrix C, int n, Matrix C11, Matrix C12, Matrix C21, Matrix C22) {
    int h = n / 2;
    for (int i = 0; i < h; i++)
        for (int j = 0; j < h; j++) {
            C[i][j]         = C11[i][j];
            C[i][j + h]     = C12[i][j];
            C[i + h][j]     = C21[i][j];
            C[i + h][j + h] = C22[i][j];
        }
}

Matrix strassen(Matrix A, Matrix B, int n) {
    Matrix C = allocMat(n);
    if (n == 1) {
        C[0][0] = A[0][0] * B[0][0];
        return C;
    }

    int h = n / 2;
    Matrix A11 = allocMat(h), A12 = allocMat(h), A21 = allocMat(h), A22 = allocMat(h);
    Matrix B11 = allocMat(h), B12 = allocMat(h), B21 = allocMat(h), B22 = allocMat(h);
    splitMat(A, n, A11, A12, A21, A22);
    splitMat(B, n, B11, B12, B21, B22);

    Matrix t1, t2, P1, P2, P3, P4, P5, P6, P7;

    t1 = addMat(B12, B22, h, -1); P1 = strassen(A11, t1, h); freeMat(t1, h);
    t1 = addMat(A11, A12, h, 1);  P2 = strassen(t1, B22, h); freeMat(t1, h);
    t1 = addMat(A21, A22, h, 1);  P3 = strassen(t1, B11, h); freeMat(t1, h);
    t1 = addMat(B21, B11, h, -1); P4 = strassen(A22, t1, h); freeMat(t1, h);
    t1 = addMat(A11, A22, h, 1); t2 = addMat(B11, B22, h, 1);
    P5 = strassen(t1, t2, h); freeMat(t1, h); freeMat(t2, h);
    t1 = addMat(A12, A22, h, -1); t2 = addMat(B21, B22, h, 1);
    P6 = strassen(t1, t2, h); freeMat(t1, h); freeMat(t2, h);
    t1 = addMat(A11, A21, h, -1); t2 = addMat(B11, B12, h, 1);
    P7 = strassen(t1, t2, h); freeMat(t1, h); freeMat(t2, h);

    Matrix C11 = allocMat(h), C12 = allocMat(h), C21 = allocMat(h), C22 = allocMat(h);
    for (int i = 0; i < h; i++)
        for (int j = 0; j < h; j++) {
            C11[i][j] = P5[i][j] + P4[i][j] - P2[i][j] + P6[i][j];
            C12[i][j] = P1[i][j] + P2[i][j];
            C21[i][j] = P3[i][j] + P4[i][j];
            C22[i][j] = P5[i][j] + P1[i][j] - P3[i][j] - P7[i][j];
        }
    joinMat(C, n, C11, C12, C21, C22);

    freeMat(A11,h); freeMat(A12,h); freeMat(A21,h); freeMat(A22,h);
    freeMat(B11,h); freeMat(B12,h); freeMat(B21,h); freeMat(B22,h);
    freeMat(P1,h); freeMat(P2,h); freeMat(P3,h); freeMat(P4,h);
    freeMat(P5,h); freeMat(P6,h); freeMat(P7,h);
    freeMat(C11,h); freeMat(C12,h); freeMat(C21,h); freeMat(C22,h);
    return C;
}

int nextPow2(int n) { int p = 1; while (p < n) p <<= 1; return p; }

int main(void) {
    int n;
    printf("Enter matrix size n: ");
    scanf("%d", &n);
    int N = nextPow2(n);   // Strassen needs power-of-2 dims; pad with zeros

    Matrix A = allocMat(N), B = allocMat(N);
    printf("Enter matrix A (%d x %d):\n", n, n);
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++) scanf("%d", &A[i][j]);
    printf("Enter matrix B (%d x %d):\n", n, n);
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++) scanf("%d", &B[i][j]);

    Matrix C = strassen(A, B, N);

    printf("Result C = A x B:\n");
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) printf("%d ", C[i][j]);
        printf("\n");
    }

    freeMat(A, N); freeMat(B, N); freeMat(C, N);
    return 0;
}
