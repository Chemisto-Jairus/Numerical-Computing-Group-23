# MatrixSolver: Gaussian elimination and LU decomposition

| | |
|---|---|
| Owner | Batte Derrick Calvin (Member 2); independent tests by Kyeyune Praise Pauline (Member 8) |
| Header | `include/numerical-computing/matrix_solver.hpp` |
| Source | `src/matrix_solver.cpp` |
| Tests | `tests/test_matrix_solver.cpp` (21 checks) |
| Examples | `examples/example_matrix_solver.cpp`, `examples/example_matrix_solver_applied.cpp` |

## 1. What it does

Solves a square linear system **A x = b** (n equations, n unknowns).

| Method | Approach |
|---|---|
| `gaussian_elimination(A, b)` | reduce [A | b] to upper triangular form, then back-substitute |
| `lu_solve(A, b)` | factorise P A = L U, then solve L y = P b and U x = y |

A matrix is a `std::vector<std::vector<double>>` of rows: `A[i][j]` is row i, column j.

```cpp
numcomp::MatrixSolver solver;
std::vector<std::vector<double>> A = {{2, 1}, {1, 3}};
std::vector<double> b = {5, 10};
std::vector<double> x = solver.gaussian_elimination(A, b);   // {1, 3}
```

## 2. How Gaussian elimination works

1. Check the shape (`checks::check_square_system`): A not empty, every row of length n, b of length n.
2. Copy A and b (the caller's data must not change).
3. For each column k = 0..n−1:
   - **Partial pivoting:** find the row p ≥ k with the largest |M[p][k]| (`find_pivot_row`). If even that is below 1e-12, the matrix is singular: throw `runtime_error`. Swap rows p and k (in M and b).
   - **Eliminate:** for every row i below k, factor = M[i][k]/M[k][k]; subtract factor × row k from row i (and from b_i). Column k below the pivot becomes zero.
4. M is now upper triangular. **Back substitution** (`back_substitution`), from the last row up: x_i = (b_i − Σ_(j>i) M[i][j]·x_j) / M[i][i].

## 3. How LU decomposition works

Elimination can be recorded as a factorisation P·A = L·U:
- U is the upper triangular matrix that elimination produces;
- L is lower triangular with 1s on the diagonal, and L[i][k] holds the multiplier used to clear entry (i, k);
- P is the row permutation from pivoting. Here it is applied to b as the rows are swapped, so it is never stored as a matrix.

Then A x = b becomes L (U x) = P b:
1. **Forward substitution:** solve L y = P b from the top row down. L has unit diagonal, so there is no division: y_i = (Pb)_i − Σ_(j<i) L[i][j]·y_j.
2. **Back substitution:** solve U x = y (the same shared helper as Gaussian elimination).

One detail when two rows are swapped at column k: only the multipliers already stored in L (columns 0..k−1) move with the rows. The unit diagonal stays where it is.

**Why have both?** The answers are the same, but L and U can be reused: solving the same A with many different b costs O(n³) once and then only O(n²) per b. The current API refactorises on every call (stateless design). The method is kept because it is the standard and more flexible formulation (the timetable's stretch goal).

## 4. Why it is written this way

| Decision | Reason |
|---|---|
| Partial pivoting in both methods | Without it, a zero pivot (example: A[0][0] = 0) divides by zero even when the system is solvable, and small pivots magnify rounding errors. |
| Singular threshold 1e-12 | A pivot that small means no unique solution (or a numerically meaningless one). |
| Inputs taken by `const&`, then copied | Elimination overwrites the matrix; the caller's A and b stay unchanged (this is tested). |
| `find_pivot_row` and `back_substitution` as `static` functions | Shared by both methods, but private to the source file, so the public class API stays at two functions. |
| Shape check in `checks::check_square_system` | One rule, one set of messages, for both methods. |

## 5. Accuracy and cost

- Both are O(n³) operations (about n³/3 multiplications for the factorisation) and O(n²) memory.
- With partial pivoting, the computed solution is the exact solution of a slightly perturbed system (backward stable in practice). The relative error then grows with the condition number of A.
- In the tests and examples, residuals |A x − b| are around 1e-15.

## 6. Errors

| Exception | When |
|---|---|
| `std::invalid_argument` | A empty, not square (including rows of different lengths), or b the wrong size |
| `std::runtime_error` | a pivot below 1e-12 (singular or nearly singular matrix) |

## 7. Limitations

- Dense storage. Meant for small systems (tens to a few hundreds of unknowns).
- The threshold is absolute, so very badly scaled matrices may be wrongly called singular, or not detected.
- No iterative refinement, condition-number estimate, or sparse/banded variants (the spline uses its own tridiagonal solver).

## 8. Tests and examples

- **Tests (21 checks, written independently by Pauline):** 2×2, 3×3 and 4×4 systems; both methods agree; 1×1; a zero first pivot; the identity matrix; inputs unchanged; empty, non-square, ragged and wrong-size input; singular and all-zero matrices.
- **Example 1:** the textbook 3×3 system with both methods, an automatic row swap, a 4×4 system, and rejected inputs.
- **Example 2:** loop currents in a resistor circuit, a parabola through three points, and a three-alloy mixing problem.
