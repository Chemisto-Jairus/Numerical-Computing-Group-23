# ConjugateGradientSolver: iterative solution of SPD linear systems

| | |
|---|---|
| Owner | Bakalu Valeria (Member 4) |
| Header | `include/numerical-computing/conjugate_gradient_solver.hpp` |
| Source | `src/conjugate_gradient_solver.cpp` |
| Tests | `tests/test_conjugate_gradient_solver.cpp` (20 checks) |
| Examples | `examples/example_conjugate_gradient_solver.cpp`, `examples/example_conjugate_gradient_solver_applied.cpp` |

## 1. What it does

| Method | Purpose |
|---|---|
| `solve(A, b, tol, max_iter)` | Solve $Ax = b$ starting from $x = 0$ |
| `solve_from(A, b, x0, tol, max_iter)` | The same, starting from the guess $x_0$ |

$A$ must be **symmetric positive definite** (SPD): $A = A^T$ and $x^T Ax > 0$ for every $x \neq 0$. Stiffness matrices, finite difference matrices and the normal equations of least squares are all SPD.

```cpp
numcomp::ConjugateGradientSolver cg;
std::vector<double> x = cg.solve({{4, 1}, {1, 3}}, {1, 2}, 1e-12, 100);   // (1/11, 7/11)
```

## 2. How it works

For an SPD matrix, solving $Ax = b$ is the same as finding the lowest point of the bowl-shaped function $\phi(x) = \frac{1}{2} x^T Ax - b^T x$. The residual $r = b - Ax$ points downhill. Conjugate gradient walks downhill, but it chooses each new direction so that it does not undo the progress of the earlier ones (the directions are *A-conjugate*: $p_i^T A p_j = 0$).

1. Check the shape (`checks::check_square_system`), tol and max_iter (`checks::check_tolerance`), the size of $x_0$, and symmetry (static `check_symmetric`).
2. $r = b - Ax_0$, $p = r$.
3. Repeat up to max_iter times:
   - $\alpha = \frac{r \cdot r}{p \cdot A p}$ is the best step length along $p$.
   - $x = x + \alpha p$ and $r = r - \alpha Ap$. The residual is updated without another product $Ax$.
   - Stop when $\|r\| \le \text{tol} \cdot \|b\|$.
   - $\beta = \frac{r_{\text{new}} \cdot r_{\text{new}}}{r \cdot r}$ and $p = r + \beta p$. This is the next direction, conjugate to the previous ones.
4. If $p \cdot Ap \le 0$, $A$ is not positive definite: throw `runtime_error`. If the loop ends without reaching tol, throw `runtime_error`.

In exact arithmetic CG reaches the exact answer in at most $n$ iterations for an $n \times n$ system. In example 1 the tridiagonal ($-1, 2, -1$) systems with $b = (1, \dots, 1)$ need only $n/2$ iterations, because $b$ excites only half of the eigenvectors. Each iteration costs one matrix-vector product, so $A$ is never modified or factorised.

## 3. Why it is written this way

| Decision | Reason |
|---|---|
| A new class, not a third method of `MatrixSolver` | CG is iterative (tol, max_iter, convergence failure) and only works for SPD matrices; the direct solvers work for any non-singular matrix and need no tolerance. Keeping them apart keeps each interface honest. |
| Relative stopping test $\|r\| \le \text{tol} \cdot \|b\|$ | Independent of the units of $b$. For $b = 0$ the test becomes $\|r\| \le \text{tol}$. |
| Symmetry checked up front (`invalid_argument`) | A non-symmetric $A$ is a wrong input; the method would silently give a wrong answer. |
| Positive definiteness detected during the iteration (`runtime_error`) | Checking it beforehand costs as much as solving the system. A non-positive $p \cdot Ap$ proves it is not SPD. |
| `dot`, `multiply` and `check_symmetric` as `static` helpers | They are needed only in this file; `static` keeps them private. |
| Shared checks | Same shape and tolerance rules and messages as `MatrixSolver` and the root finders. |

## 4. Errors

| Exception | When |
|---|---|
| `std::invalid_argument` | $A$ empty or not square; $b$ or $x_0$ the wrong size; $A$ not symmetric; $\text{tol} \le 0$ or NaN; $\text{max\_iter} \le 0$ |
| `std::runtime_error` | $A$ not positive definite; tolerance not reached within max_iter iterations |

## 5. Limitations

- Dense storage (vector of rows). A real large sparse problem would store only the non-zero entries; the algorithm itself would not change.
- No preconditioning, so badly conditioned systems need many iterations.
- Rounding means the "at most $n$ iterations" property holds only approximately for large or ill-conditioned systems.

## 6. Tests and examples

- **Tests (20 checks):** $2 \times 2$, $3 \times 3$ and diagonal systems; a $50 \times 50$ tridiagonal system solved within $n$ iterations; `solve_from` with an exact and a rough guess; $1 \times 1$ and $b = 0$; the caller's $A$ and $b$ unchanged; wrong shapes, non-symmetric, indefinite and negative definite matrices, bad tol/max_iter, and too few iterations.
- **Example 1:** a $3 \times 3$ system, iterations needed for $n = 2 \dots 40$, agreement with Gaussian elimination, rejected inputs.
- **Example 2:** a chain of springs, a heated rod with 99 unknowns (matches $\frac{x(1 - x)}{2}$), and a straight-line fit through the normal equations.