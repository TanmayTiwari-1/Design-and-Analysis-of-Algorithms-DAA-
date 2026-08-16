# Special-Pattern Recursive Matrix Multiplication (C)

A C program that multiplies two square matrices of a special **block-symmetric pattern**:

```
M = [ M1  M2 ]
    [ M2  M1 ]
```

By exploiting this structure, the program computes the product using **only 2 recursive multiplications per level** (instead of the usual 4 for naive block multiplication, or 7 for general-purpose Strassen), since the result of multiplying two such matrices is itself guaranteed to follow the same pattern.

## Overview

For matrices of the form above, the product can be derived using the identity:

```
(M1+M2)(N1+N2) = M1N1 + M1N2 + M2N1 + M2N2
(M1-M2)(N1-N2) = M1N1 - M1N2 - M2N1 + M2N2
```

Adding and subtracting these two results (each divided by 2) directly yields:

```
P1 = M1N1 + M2N2   =  (S + D) / 2
P2 = M1N2 + M2N1   =  (S - D) / 2
```

where `S = (M1+M2)(N1+N2)` and `D = (M1-M2)(N1-N2)`.

The full result matrix is then reassembled as:

```
C = [ P1  P2 ]
    [ P2  P1 ]
```

This means only **2 recursive multiplications** (`S` and `D`) are needed per level, rather than the 4 required for standard 2x2 block matrix multiplication — a direct consequence of the input's special symmetric structure.

## Files

| File | Description |
|------|--------------|
| `special_multiply.c` | Main source file containing the full implementation |

## How It Works

1. **`allocMat(n)`** — Allocates an `n x n` matrix of `double`, zero-initialized via `calloc`.
2. **`freeMat(m, n)`** — Frees a previously allocated matrix.
3. **`splitBlocks(M, n, M1, M2)`** — Extracts the top-left (`M1`) and top-right (`M2`) `h x h` quadrants of an `n x n` matrix. The bottom half is not read, since it is redundant under the special pattern (`M21 = M12`, `M22 = M11`).
4. **`combine(A, B, h, sign)`** — Computes `A + B` or `A - B` elementwise, used to build the sum (`Ms`, `Ns`) and difference (`Md`, `Nd`) blocks.
5. **`specialMultiply(M, N, n)`** — The core recursive routine:
   - Base case: `n == 1` → direct scalar multiplication.
   - Otherwise:
     - Splits `M` and `N` into `M1, M2` and `N1, N2`.
     - Recursively computes `S = specialMultiply(M1+M2, N1+N2)` and `D = specialMultiply(M1-M2, N1-N2)`.
     - Derives `P1 = (S+D)/2` and `P2 = (S-D)/2`.
     - Reassembles the full result as `[[P1, P2], [P2, P1]]`.
6. **`main()`** — Reads `n`, reads matrices `A` and `B` (expected to already follow the special pattern), runs `specialMultiply()`, and prints the resulting `n x n` matrix.

## Building

```bash
gcc -O2 -o special_multiply special_multiply.c -lm
```

## Running

```bash
./special_multiply
```

Example session (`n = 2`, so `M1` and `M2` are 1x1 scalars):

```
Enter n (power of 2): 2
Enter matrix A (2 x 2), must satisfy the [[M1,M2],[M2,M1]] pattern:
1 2
2 1
Enter matrix B (2 x 2), same pattern:
3 4
4 3
Result C = A x B:
19.0 16.0
16.0 19.0
```

## Notes & Limitations

- **Input must follow the pattern**: The program assumes (but does not verify) that both input matrices satisfy `M21 = M12` and `M22 = M11` at every recursive level. Since `splitBlocks` never reads the bottom half of the matrix, passing in an arbitrary (non-pattern) matrix will silently produce an incorrect result rather than an error.
- **`n` must be a power of 2**: Unlike some Strassen-style implementations, this program does **not** pad or validate the input size. Non-power-of-2 sizes will cause incorrect integer division (`h = n / 2`) at the base case and undefined behavior.
- **Uses `double`**: Values are read and printed as floating point (`%lf` / `%.1f`), and division by `2.0` is used to recover `P1`/`P2`, which assumes exact or near-exact divisibility. Accumulated floating-point rounding error is possible for large/deep recursions.
- **No input validation**: `n = 0`, negative `n`, or non-power-of-2 `n` are not checked.
- **Square matrices only**: Both input matrices must be the same size.

## Complexity

Because each level performs only 2 recursive multiplications on half-sized blocks (instead of 4 or 7), the recurrence is:

```
T(n) = 2 * T(n/2) + O(n^2)
```

which solves to **O(n^2 log n)** — asymptotically faster than standard O(n^3) matrix multiplication, and even faster than general Strassen's O(n^2.807), thanks to the extra structure in the input.

| Operation | Complexity |
|-----------|------------|
| Time      | O(n^2 log n) |
| Space     | O(n^2) (plus recursive temporaries) |

## License

Free to use and modify for educational or personal purposes.
