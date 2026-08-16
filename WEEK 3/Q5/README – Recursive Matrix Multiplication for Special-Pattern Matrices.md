# README – Recursive Matrix Multiplication for Special-Pattern Matrices

## 1. Aim

To implement **recursive matrix multiplication** for matrices having the special block pattern:

\[
M =
\begin{bmatrix}
M_1 & M_2\\
M_2 & M_1
\end{bmatrix}
\]

The program takes two such matrices, recursively multiplies them, and produces the resulting matrix.

---

## 2. Problem Description

The matrices used in this program have a special structure:

```text
        ┌─────────┬─────────┐
        │   M1    │   M2    │
        ├─────────┼─────────┤
        │   M2    │   M1    │
        └─────────┴─────────┘
```

Instead of performing four independent matrix multiplications, the program uses the identities:

\[
(M_1+M_2)(N_1+N_2)
\]

and

\[
(M_1-M_2)(N_1-N_2)
\]

to obtain the two blocks of the result.

For matrices

\[
A =
\begin{bmatrix}
A_1&A_2\\
A_2&A_1
\end{bmatrix},
\quad
B =
\begin{bmatrix}
B_1&B_2\\
B_2&B_1
\end{bmatrix}
\]

their product is:

\[
C = AB =
\begin{bmatrix}
C_1&C_2\\
C_2&C_1
\end{bmatrix}
\]

where:

\[
C_1=A_1B_1+A_2B_2
\]

and

\[
C_2=A_1B_2+A_2B_1
\]

---

## 3. Main Idea of the Algorithm

The program uses the following transformations:

### Sum

\[
S=(A_1+A_2)(B_1+B_2)
\]

### Difference

\[
D=(A_1-A_2)(B_1-B_2)
\]

From these two products:

\[
C_1=\frac{S+D}{2}
\]

and

\[
C_2=\frac{S-D}{2}
\]

This works because:

\[
S+D
=2(A_1B_1+A_2B_2)
\]

and

\[
S-D
=2(A_1B_2+A_2B_1)
\]

The final result is then constructed as:

```text
        ┌─────────┬─────────┐
        │   C1    │   C2    │
        ├─────────┼─────────┤
        │   C2    │   C1    │
        └─────────┴─────────┘
```

---

## 4. Algorithm

1. Read `n`, where `n` must be a power of 2.
2. Allocate matrices `A` and `B`.
3. Read the elements of matrices `A` and `B`.
4. Divide each matrix into four blocks.
5. Because of the special pattern, only the top-left and top-right blocks are required.
6. Calculate:
   - `A1 + A2`
   - `A1 - A2`
   - `B1 + B2`
   - `B1 - B2`
7. Recursively multiply the sum matrices.
8. Recursively multiply the difference matrices.
9. Calculate `C1` using:

   ```text
   C1 = (S + D) / 2
   ```

10. Calculate `C2` using:

   ```text
   C2 = (S - D) / 2
   ```

11. Construct the final matrix:

```text
C = [[C1, C2],
     [C2, C1]]
```

12. Print the result.
13. Free all dynamically allocated memory.

---

## 5. Important Functions

### `allocMat()`

```c
Matrix allocMat(int n)
```

Dynamically allocates an `n × n` matrix using `malloc()` and `calloc()`.

---

### `freeMat()`

```c
void freeMat(Matrix m, int n)
```

Releases the dynamically allocated memory of a matrix.

This is important because the program creates many temporary matrices during recursion.

---

### `splitBlocks()`

```c
void splitBlocks(Matrix M, int n, Matrix M1, Matrix M2)
```

Extracts the two unique blocks from the special matrix:

```text
M = [[M1, M2],
     [M2, M1]]
```

Only `M1` and `M2` from the top half need to be extracted because the bottom half is redundant.

---

### `combine()`

```c
Matrix combine(Matrix A, Matrix B, int h, int sign)
```

Creates either:

\[
A+B
\]

or

\[
A-B
\]

depending on the value of `sign`.

For example:

```c
combine(A, B, h, 1)
```

produces `A + B`.

```c
combine(A, B, h, -1)
```

produces `A - B`.

---

### `specialMultiply()`

```c
Matrix specialMultiply(Matrix M, Matrix N, int n)
```

This is the main recursive function.

It:
- Divides the matrices into blocks.
- Creates sum and difference matrices.
- Recursively multiplies them.
- Calculates `P1` and `P2`.
- Constructs the final result.

### Base Case

When:

```c
n == 1
```

normal scalar multiplication is performed:

```c
C[0][0] = M[0][0] * N[0][0];
```

---

## 6. Example

For:

```text
A =  1  2
     2  1

B =  3  4
     4  3
```

The blocks are:

```text
A1 = [1]    A2 = [2]

B1 = [3]    B2 = [4]
```

Calculate:

\[
S=(1+2)(3+4)=3\times7=21
\]

\[
D=(1-2)(3-4)=(-1)(-1)=1
\]

Therefore:

\[
C_1=\frac{21+1}{2}=11
\]

\[
C_2=\frac{21-1}{2}=10
\]

So:

```text
C =  11  10
     10  11
```

---

## 7. Input Format

The program first asks for the matrix size:

```text
Enter n (power of 2):
```

Then enter all elements of matrix `A`, followed by all elements of matrix `B`.

Example:

```text
Enter n (power of 2): 2

Enter matrix A (2 x 2), must satisfy the [[M1,M2],[M2,M1]] pattern:
1 2
2 1

Enter matrix B (2 x 2), same pattern:
3 4
4 3
```

---

## 8. Output

The program prints:

```text
Result C = A x B:
11.0 10.0
10.0 11.0
```

---

## 9. Complexity

At each recursive level, the algorithm performs **two recursive matrix multiplications** of size `n/2`.

Therefore, the main recurrence is approximately:

\[
T(n)=2T(n/2)+O(n^2)
\]

Using the Master Theorem:

\[
T(n)=O(n^2)
\]

Thus, the recursive multiplication takes:

**Time Complexity: `O(n²)`**

The algorithm exploits the special matrix structure to achieve this improvement over ordinary matrix multiplication.

---

## 10. Space Complexity

The program dynamically allocates several temporary matrices during each recursive call.

The auxiliary space is approximately:

**O(n²)**

because matrices of different sizes are allocated during recursion.

---

## 11. Important Requirements

### Matrix Size

`n` should be a **power of 2**:

```text
1, 2, 4, 8, 16, 32, ...
```

The recursive division uses:

```c
int h = n / 2;
```

so the intended input must allow repeated division down to `1`.

### Special Matrix Pattern

Both input matrices must satisfy:

\[
\begin{bmatrix}
M_1&M_2\\
M_2&M_1
\end{bmatrix}
\]

For example, a valid `4 × 4` matrix has the form:

```text
a b c d
c d a b
c d a b
a b c d
```

when interpreted according to the required block structure.

The program assumes the user provides matrices satisfying the required pattern; it does **not** validate the pattern.

---

## 12. Concepts Involved

- Recursive algorithms
- Divide and conquer
- Matrix multiplication
- Block matrix representation
- Dynamic memory allocation
- Pointers and pointer-to-pointer matrices
- `malloc()` and `calloc()`
- Memory deallocation using `free()`
- Base case and recursive case
- Time-complexity analysis
- Master Theorem
- Exploiting special matrix structure

---

## 13. Key Takeaway

The main advantage of this algorithm is that it **uses the special structure of the matrices instead of treating them as arbitrary matrices**.

Rather than calculating all four block products independently, it uses only two recursive multiplications:

\[
(A_1+A_2)(B_1+B_2)
\]

and

\[
(A_1-A_2)(B_1-B_2)
\]

and then reconstructs the result using:

\[
C_1=\frac{S+D}{2},
\qquad
C_2=\frac{S-D}{2}
\]

This demonstrates how recognizing a mathematical structure can significantly reduce the computational work required by an algorithm.

## 14. Conclusion

The program successfully implements recursive multiplication of special-pattern matrices. It demonstrates the use of **divide-and-conquer recursion, block matrices, dynamic memory allocation, and complexity analysis**. By exploiting the matrix structure, the algorithm achieves an `O(n²)` time complexity for this restricted class of matrices.