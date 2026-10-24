# Interpolator: linear interpolation, Chebyshev nodes and Chebyshev approximation

| | |
|---|---|
| Owner | Batte Derrick Calvin (Member 2) |
| Header | `include/numerical-computing/interpolator.hpp` |
| Source | `src/interpolator.cpp` |
| Tests | `tests/test_interpolator.cpp` (37 checks) |
| Examples | `examples/example_interpolator.cpp`, `examples/example_interpolator_applied.cpp` |

## 1. What it does

*Interpolation* estimates a value between known data points (x_i, y_i). *Approximation* replaces a whole function by a simpler one that is close to it everywhere on an interval.

| Method | What it returns |
|---|---|
| `linear_interpolate(x, x_vals, y_vals)` | the value at x of the straight line joining the two data points on either side of x |
| `chebyshev_nodes(a, b, n)` | n well-placed sample points on [a, b] |
| `chebyshev_coefficients(f, a, b, n)` | the n coefficients c_0 … c_(n−1) of the Chebyshev approximation of f on [a, b] |
| `chebyshev_approximate(coeffs, a, b, x)` | the value at x of that approximation |

```cpp
numcomp::Interpolator interp;
std::vector<double> x = {0, 1, 2}, y = {0, 1, 4};
double v = interp.linear_interpolate(1.5, x, y);                 // 2.5
std::vector<double> nodes = interp.chebyshev_nodes(-1, 1, 3);    // 0.866, 0, -0.866

std::vector<double> c = interp.chebyshev_coefficients([](double t) { return std::exp(t); }, 0, 1, 12);
double e = interp.chebyshev_approximate(c, 0, 1, 0.5);             // e^0.5, error about 1e-15
```

## 2. How linear interpolation works

1. Validate the data (shared helper `checks::check_data_points`):
   - same number of x and y values;
   - at least 2 points;
   - x strictly increasing.
2. Find the interval [x_i, x_(i+1)] that contains x (shared helper `checks::find_interval`). If there is none, x is outside the data: throw `out_of_range`.
3. Return the point on the line through the two neighbours:

  y = y_i + (y_(i+1) − y_i)·(x − x_i)/(x_(i+1) − x_i)

**Accuracy:** for a smooth f, the error between two points is at most h²·max|f''|/8, where h is the gap. Halving the gaps divides the error by 4. Straight-line data is reproduced exactly.

## 3. How Chebyshev nodes work

The Chebyshev polynomial T_n has its n roots at cos((2k − 1)π/(2n)), k = 1..n, on [−1, 1]. Mapping them onto [a, b] gives:

  x_k = (a + b)/2 + (b − a)/2 · cos((2k − 1)π/(2n))

These points crowd towards the ends of the interval. A **single high-degree polynomial** through Chebyshev points avoids the wild end oscillations (Runge's phenomenon) that equally spaced points cause.

Example 2 checks something worth knowing: for **piecewise linear** interpolation, Chebyshev points are *worse* than equal spacing. The error there depends on the widest gap, and Chebyshev gaps are widest in the middle. Use them for one global polynomial, which is exactly what Chebyshev approximation does.

## 4. How Chebyshev approximation works

The Chebyshev polynomials are T_0 = 1, T_1 = u, T_(j+1) = 2u·T_j − T_(j−1), or T_j(cos θ) = cos(jθ). The approximation of f on [a, b] is

  p(x) = c_0/2 + c_1·T_1(u) + … + c_(n−1)·T_(n−1)(u),  with u = (2x − a − b)/(b − a) ∈ [−1, 1].

**Coefficients** (`chebyshev_coefficients`):
1. Get the n nodes by calling `chebyshev_nodes(a, b, n)`. This also checks a, b and n. The node with index k corresponds to the angle θ_k = (2k + 1)π/(2n).
2. Evaluate f once at every node (and check each value is finite).
3. c_j = (2/n)·Σ_k f(x_k)·cos(j·θ_k), j = 0 … n−1. This is a discrete cosine transform. Because the cosines are orthogonal at these angles, p equals f at all n nodes.

**Evaluation** (`chebyshev_approximate`) uses **Clenshaw's recurrence**. Starting with b_n = b_(n+1) = 0, compute b_j = 2u·b_(j+1) − b_(j+2) + c_j for j = n−1 down to 1, then p(x) = u·b_1 − b_2 + c_0/2. This takes O(n) operations, never forms a T_j explicitly, and is numerically stable.

**Accuracy.** For a smooth f the coefficients fall off very fast, and the error is about the size of the first coefficient left out. Example 1: for eˣ on [0, 1], 6 terms give 1.2e-06, 10 terms 9.1e-13 and 12 terms 2.7e-15. For the Runge function on [−1, 1], where equally spaced polynomial interpolation diverges, the error *falls*: 0.40 (degree 4), 0.033 (degree 16), 2.5e-06 (degree 64). The result is close to the best possible polynomial of that degree (the minimax polynomial).

## 5. Why it is written this way

| Decision | Reason |
|---|---|
| Data passed as `const std::vector<double>&` | No copy is made and the caller's data cannot be changed. |
| x must be **strictly** increasing | Repeated x would divide by zero; unsorted data would make "the interval containing x" ambiguous. |
| `!(x[i] < x[i+1])` in the check | Rejects equal values, decreasing values **and NaN** in one test. |
| No extrapolation (`out_of_range`) | Outside the data the straight line has no basis, so the method refuses instead of guessing. |
| Checks and interval search in `src/internal/input_checks` | `CubicSpline` needs exactly the same rules; writing them once keeps both classes consistent. |
| Node limit of 1,000,000 | A huge n would try to allocate gigabytes (Week 3 bug B3). The same limit is used for ODE steps (`checks::MAX_POINTS`). |
| `!(a < b)` for the interval | Rejects a = b, a > b and NaN. |
| Approximation added to `Interpolator`, not a new class | It is built on `chebyshev_nodes`, which already lives here; the nodes are reused, not recomputed by a second formula. |
| Coefficients returned as a plain vector | The caller computes them once and evaluates many times; the class stays stateless. |
| Limit of 1,000 coefficients | Computing them costs n² cosines; 1,000 terms is far more than any smooth function needs. |
| Clenshaw's recurrence | O(n), stable, and simpler than building the polynomial in powers of x (which would lose accuracy). |
| `out_of_range` outside [a, b] | Same rule as linear interpolation: no extrapolation. |
| Interval search by binary search (shared `find_interval`) | O(log n) per look-up instead of O(n). |

## 6. Errors

| Exception | When |
|---|---|
| `std::invalid_argument` | sizes differ, fewer than 2 points, x not strictly increasing (or NaN); Chebyshev nodes: n ≤ 0, n > 1,000,000, !(a < b); Chebyshev coefficients: n ≤ 0, n > 1,000, !(a < b); empty coefficient list |
| `std::out_of_range` | x outside [x_vals.front(), x_vals.back()]; approximation evaluated outside [a, b] (or at NaN) |
| `std::runtime_error` | f returns NaN or infinity at a Chebyshev node |

## 7. Limitations

- Linear interpolation has corners at the data points (its derivative jumps). Use `CubicSpline` for a smooth curve.
- Chebyshev approximation needs f as a formula (it chooses where to sample). For measured data at fixed x values, use linear interpolation or the spline.
- Functions with a kink or a jump converge slowly (only algebraically), because the coefficients then decay slowly.

## 8. Tests and examples

- **Tests (37 checks):** normal, uneven and negative data; end points; 2-point data; size mismatch; one point; unsorted, repeated and NaN x; x outside the data; Chebyshev values, placement, n = 1, bad n, bad and NaN intervals, the node limit.
- **Example 1:** interpolating y = x², uneven data, Chebyshev nodes for n = 3, 5, 8, and rejected inputs.
- **Example 2:** a temperature table look-up, and equal spacing vs. Chebyshev samples (with the explanation above).
