# FiniteDifferenceSolver: linear two-point boundary value problems

| | |
|---|---|
| Owner | Aogon Sharon (Member 3) |
| Header | `include/numerical-computing/finite_difference_solver.hpp` |
| Source | `src/finite_difference_solver.cpp` |
| Shared helper | `src/internal/tridiagonal.hpp/.cpp` (`linalg::solve_tridiagonal`, also used by CubicSpline) |
| Tests | `tests/test_finite_difference_solver.cpp` (20 checks) |
| Examples | `examples/example_finite_difference_solver.cpp`, `examples/example_finite_difference_solver_applied.cpp` |

## 1. What it does

| Method | Problem |
|---|---|
| `linear_bvp(p, q, r, a, b, alpha, beta, n)` | y'' = p(x)·y' + q(x)·y + r(x) on [a, b], y(a) = alpha, y(b) = beta |
| `poisson(r, a, b, alpha, beta, n)` | y'' = r(x) (the case p = q = 0) |

Both return the n + 1 grid points (x_i, y_i), from (a, alpha) to (b, beta).

```cpp
numcomp::FiniteDifferenceSolver fd;
const double pi = std::acos(-1.0);
auto grid = fd.poisson([=](double x) { return -pi * pi * std::sin(pi * x); }, 0.0, 1.0, 0.0, 0.0, 100);   // y = sin(pi x)
```

## 2. How it works

In a boundary value problem, the conditions are given at **both** ends. The solution cannot be marched from one side like an initial value problem. Instead:

1. **Grid.** h = (b − a)/n and x_i = a + i·h. The unknowns are y_1 … y_(n−1); y_0 = alpha and y_n = beta are known.
2. **Replace the derivatives** at each inner point by central differences (both second order, see NumericalDifferentiator):
   y'' ≈ (y_(i−1) − 2y_i + y_(i+1))/h² and y' ≈ (y_(i+1) − y_(i−1))/(2h).
3. **One equation per inner point.** Multiplying by −h² gives
   −(1 + h·p_i/2)·y_(i−1) + (2 + h²·q_i)·y_i − (1 − h·p_i/2)·y_(i+1) = −h²·r_i.
4. **Boundary values.** The first and last equations contain y_0 and y_n. These are known, so they move to the right-hand side.
5. **Solve.** Each equation links only three neighbours, so the matrix is tridiagonal. `linalg::solve_tridiagonal` (the Thomas algorithm) solves it in O(n) operations: one sweep down and one back up.

**Error ~ h².** Doubling n divides the error by 4 (example 1: 0.0083, 0.0021, 0.00051, 0.00013 for n = 10 … 80). A quadratic solution is reproduced exactly.

**When it works.** If q(x) ≥ 0 and h·|p(x)|/2 < 1, the matrix is diagonally dominant. The system then has a unique solution, and the Thomas algorithm needs no pivoting.

## 3. Why it is written this way

| Decision | Reason |
|---|---|
| Thomas algorithm moved to `src/internal/tridiagonal` | CubicSpline already contained it. Writing it a second time would break the "no duplicated code" rule, so both classes now call one helper. |
| Tridiagonal, not a full matrix for `MatrixSolver` | n − 1 unknowns would need (n − 1)² numbers and O(n³) work; the tridiagonal form needs 4(n − 1) numbers and O(n) work. 1,000,000 intervals take a fraction of a second. |
| `poisson` calls `linear_bvp` with p = q = 0 | One implementation; the simpler method is a convenience. |
| p, q, r as functions | Covers variable-coefficient problems such as the textbook test problem on [1, 2]. |
| Last grid point set to exactly b | a + n·h can differ from b by rounding. |
| Checks: shared `check_integration_inputs` (n > 0, finite a, b) plus n ≥ 2, n ≤ 1,000,000, a < b, finite alpha and beta | Clear `invalid_argument` messages before any work. |

## 4. Errors

| Exception | When |
|---|---|
| `std::invalid_argument` | n < 2 or > 1,000,000; !(a < b); a, b, alpha or beta NaN or infinite |
| `std::runtime_error` | p, q or r is not finite at a grid point; the tridiagonal system is singular (zero pivot) |

## 5. Limitations

- Linear problems only. A nonlinear y'' = f(x, y, y') would need Newton's method around this solver, or a shooting method.
- Dirichlet conditions only (values of y at the ends, not of y').
- Equally spaced grid.

## 6. Tests and examples

- **Tests (20 checks):** sin(πx), y = x² exactly, sinh x, the Burden & Faires variable-coefficient problem; second order (ratio ≈ 4); grid size, end points, straight line for y'' = 0, the smallest grid n = 2; invalid n, a, b, alpha, beta; a singular r(x); a singular system.
- **Example 1:** sin(πx) table, convergence table, the textbook problem, rejected inputs.
- **Example 2:** a cooling fin (exact cosh/sinh solution) and the sag of a hanging cable.
