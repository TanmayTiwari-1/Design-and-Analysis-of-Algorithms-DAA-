# Strassen Matrix Multiplication (C)

A C implementation of **Strassen's algorithm** for multiplying two square matrices, achieving a time complexity of **O(n^log2(7)) ≈ O(n^2.807)**, which is asymptotically faster than the standard O(n^3) approach.

## Overview

Strassen's algorithm reduces the number of recursive multiplications needed per divide-and-conquer step from 8 (naive) to 7, by cleverly combining sums and differences of submatrix blocks. This program:

- Accepts an arbitrary matrix size `n` from the user.
- Pads the matrices up to the next power of 2 (Strassen's algorithm requires square, power-of-2 dimensions) so any `n` can be handled.
- Recursively divides each matrix into quadrants, computes 7 intermediate products, and reconstructs the result.
- Prints the final product matrix `C = A x B`, trimmed back to the original `n x n` size.

## Files

| File | Description |
|------|--------------|
| `strassen.c` | Main source file containing the full implementation |

## How It Works

1. **`allocMat(n)`** — Allocates an `n x n` matrix of `int`, zero-initialized via `calloc`.
2. **`freeMat(m, n)`** — Frees a previously allocated matrix.
3. **`addMat(A, B, n, sign)`** — Computes `A + B` or `A - B` depending on `sign` (`+1` / `-1`).
4. **`splitMat` / `joinMat`** — Split a matrix into four quadrants (`M11, M12, M21, M22`) or reassemble quadrants into a full matrix.
5. **`strassen(A, B, n)`** — The core recursive routine:
   - Base case: `n == 1` → direct scalar multiplication.
   - Otherwise, computes the 7 Strassen products `P1..P7` from combinations of the quadrants, then derives the four result quadrants:
     ```
     C11 = P5 + P4 - P2 + P6
     C12 = P1 + P2
     C21 = P3 + P4
     C22 = P5 + P1 - P3 - P7
     ```
6. **`nextPow2(n)`** — Rounds `n` up to the nearest power of 2, since the recursive splitting requires even dimensions at every level.
7. **`main()`** — Reads `n`, reads matrices `A` and `B` (padded to `N x N`), runs `strassen()`, and prints the top-left `n x n` portion of the result.

## Building

```bash
gcc -O2 -o strassen strassen.c
```

## Running

```bash
./strassen
```

Example session:

```
Enter matrix size n: 2
Enter matrix A (2 x 2):
1 2
3 4
Enter matrix B (2 x 2):
5 6
7 8
Result C = A x B:
19 22
43 50
```

## Notes & Limitations

- **Power-of-2 padding**: Any matrix size is accepted, but internally it's padded with zeros to the next power of 2 (e.g., `n = 5` becomes `N = 8`). This wastes some computation for non-power-of-2 sizes but keeps the algorithm simple and correct.
- **Memory usage**: Being a straightforward recursive implementation, it allocates many intermediate matrices (`A11..A22`, `B11..B22`, `P1..P7`, `C11..C22`, plus temporaries) at every recursion level, which increases memory overhead compared to an in-place implementation.
- **No overflow checking**: Results are stored as `int`; multiplying large matrices with large values may overflow.
- **Practical crossover**: For small matrices, the constant-factor overhead of Strassen's algorithm often makes it slower than the naive O(n^3) approach. It becomes advantageous mainly for large `n`.
- **Square matrices only**: This implementation only supports multiplying two square matrices of the same size.

## Complexity

| Operation | Complexity |
|-----------|------------|
| Time      | O(n^log2(7)) ≈ O(n^2.807) |
| Space     | O(n^2) (with additional overhead from recursive temporaries) |

## License

Free to use and modify for educational or personal purposes.
