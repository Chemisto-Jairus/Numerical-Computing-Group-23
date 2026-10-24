# CubicSpline: natural cubic spline interpolation

| | |
|---|---|
| Owner | Batte Derrick Calvin (Member 2) |
| Header | `include/numerical-computing/cubic_spline.hpp` |
| Source | `src/cubic_spline.cpp` |
| Tests | `tests/test_cubic_spline.cpp` (18 checks) |
| Examples | `examples/example_cubic_spline.cpp`, `examples/example_cubic_spline_applied.cpp` |

## 1. What it does

A cubic spline joins the data points with a separate **cubic polynomial on each interval**. At every inner point the neighbouring pieces have the same value, slope and curvature, so the curve is smooth (twice continuously differentiable). *Natural* means the curvature (second derivative) is zero at both end points.

| Method | Returns |
|---|---|
| `second_derivatives(x_vals, y_vals)` | the spline's second derivative M_i at every data point |
| `interpolate(x, x_vals, y_vals)` | the spline's value at x |

```cpp
numcomp::CubicSpline spline;
std::vector<double> x = {0, 1, 2}, y = {0, 1, 0};
double v = spline.interpolate(0.5, x, y);   // 0.6875
```

## 2. How it works

**Step 1: the unknowns.** On [x_i, x_(i+1)] with h_i = x_(i+1) − x_i, a cubic is fixed by the two end values y_i, y_(i+1) and the two end curvatures M_i, M_(i+1). So the only unknowns are the M_i.

**Step 2: the equations.** Requiring the slope to match at each inner point i gives one linear equation:

  h_(i−1)·M_(i−1) + 2(h_(i−1) + h_i)·M_i + h_i·M_(i+1) = 6·[ (y_(i+1) − y_i)/h_i − (y_i − y_(i−1))/h_(i−1) ]

With the natural conditions M_0 = M_(n−1) = 0, that is n − 2 equations in n − 2 unknowns.

**Step 3: solve with the Thomas algorithm.** Each equation involves only three neighbouring unknowns, so the matrix is *tridiagonal*. The Thomas algorithm is Gaussian elimination specialised to that shape. It is written once, in the shared helper `linalg::solve_tridiagonal` (`src/internal/tridiagonal`), because `FiniteDifferenceSolver` solves the same kind of system:
- *Forward sweep:* for k = 1..m−1, factor = lower_k / diag_(k−1); diag_k −= factor·upper_(k−1); rhs_k −= factor·rhs_(k−1).
- *Back substitution:* M_last = rhs_last / diag_last, then upwards M_k = (rhs_k − upper_k·M_(k+1)) / diag_k.

It costs O(n) operations instead of O(n³). The diagonal 2(h_(i−1) + h_i) is larger than the two off-diagonal entries together (strict diagonal dominance), so no pivoting is needed and no division by zero can happen.

**Step 4: evaluate.** Find the interval containing x (`checks::find_interval`). With t0 = x − x_i and t1 = x_(i+1) − x:

  S(x) = M_i·t1³/(6h) + M_(i+1)·t0³/(6h) + (y_i/h − M_i·h/6)·t1 + (y_(i+1)/h − M_(i+1)·h/6)·t0

Hand check from the tests: for (0,0), (1,1), (2,0) the single equation is 4M_1 = −12, so M_1 = −3, and S(0.5) = 0.6875.

## 3. Accuracy

For smooth data the spline error is about h⁴. Example 2 shows this for sin x on [0, π]: each time h halves, the error drops by 16.9, 16.2, 16.1, 16.0. Near the ends, natural conditions cost some accuracy when the true f'' is not zero there (e^x, for example). Straight-line data is reproduced exactly.

## 4. Why it is written this way

| Decision | Reason |
|---|---|
| Natural end conditions | Need no extra information (clamped splines need the end slopes, which users rarely know). |
| Thomas algorithm, not the general `MatrixSolver` | The system is tridiagonal: O(n) and no pivoting, instead of O(n³) on a mostly-zero matrix. |
| Thomas algorithm in a shared helper (Week 5) | `FiniteDifferenceSolver` needs the same solver; one copy serves both classes. The four vectors are handed over with `std::move`, so they are not copied. |
| Stateless: recompute on every call | Same group pattern as every class. The cost is O(n) per call, which is negligible for the intended data sizes. A class that stores M after a `fit()` step would be the next step for large data. |
| Shared checks with `Interpolator` | Identical data rules and messages in both classes. |
| `second_derivatives()` is public | Lets users (and tests) check the hand-computed M values. |
| 2 points: return zeros | No inner point means no equation. The spline is the straight line. |

## 5. Errors

| Exception | When |
|---|---|
| `std::invalid_argument` | sizes differ, fewer than 2 points, x not strictly increasing (or NaN) |
| `std::out_of_range` | x outside [x_vals.front(), x_vals.back()] |

## 6. Limitations

- Natural splines only (no clamped or not-a-knot ends).
- The second derivatives are recomputed for every evaluation.
- No extrapolation.

## 7. Tests and examples

- **Tests (18 checks):** passes through every point; the hand-worked value and symmetry; M values; natural ends; straight lines; sin within 1e-3; 2-point data; end points; uneven spacing; every invalid input.
- **Example 1:** the hand-checkable case, spline vs. linear for sin x, straight-line exactness, and rejected inputs.
- **Example 2:** a smooth speed profile from speedometer readings, and the convergence table above.
