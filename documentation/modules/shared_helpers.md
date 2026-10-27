# Shared internal helpers: input checks, ODE loops and the tridiagonal solver

| | |
|---|---|
| Owners | `input_checks`, `multistep`, `tridiagonal`: Chemisto Jairus (Member 1); `ode_stepper`: Mwanje Ian (Member 6) and Muwanguzi Samuel (Member 7) |
| Files | `src/internal/input_checks.hpp/.cpp`, `src/internal/ode_stepper.hpp/.cpp`, `src/internal/multistep.hpp/.cpp`, `src/internal/tridiagonal.hpp/.cpp` |
| Public? | **No.** These live in `src/internal/`, not in `include/`, so library users never see them. |

## 1. Why they exist

The project rules say the library should "avoid unnecessary duplication of code" (Section 3). Before these helpers:
- the same `tol`/`max_iter` check was typed in four root-finding methods;
- the same "x strictly increasing" loop appeared in two classes;
- the ~30-line ODE time loop was copied into four methods.

A fix to one copy did not reach the others, and the error messages could drift apart. Now each rule is written **once** and every method calls it.

## 2. `numcomp::checks` (input_checks)

| Function | Rule enforced | Used by |
|---|---|---|
| `check_tolerance(tol, max_iter)` | tol > 0 (and not NaN), max_iter > 0 | the 4 root finders, conjugate gradient, RKF45 |
| `check_finite_limits(a, b)` | a and b are finite numbers | all 4 integration methods |
| `check_integration_inputs(a, b, n)` | n > 0 and finite limits | trapezoidal, midpoint, Simpson, finite difference BVP |
| `check_data_points(x_vals, y_vals)` | same sizes, ≥ 2 points, x strictly increasing (no NaN) | linear interpolation, cubic spline |
| `find_interval(x, x_vals)` | x inside the data, and *which* interval (binary search, O(log n)) | linear interpolation, cubic spline |
| `check_ode_inputs(t0, t_end, h)` | h > 0, t_end > t0, ≤ 1,000,000 steps | both ODE loops (Euler, Heun, RK3, RK4, AB2, AB4, AM2, AM4) and RKF45 |
| `check_square_system(A, b)` | A non-empty and square, b the right size | Gaussian elimination, LU, conjugate gradient |
| `check_difference_inputs(x, h)` | x finite, h > 0 and finite | all 5 differentiation formulas |
| `check_finite_result(value)` | a computed value is finite (`runtime_error` otherwise) | differentiation, Chebyshev coefficients, multistep loop, Adams-Moulton corrector, RKF45, finite differences |
| `MAX_POINTS` (constant) | 1,000,000 | ODE step cap, Chebyshev node cap, finite difference grid cap |

Every function either returns normally or throws the documented exception (`invalid_argument`; `out_of_range` for `find_interval`; `runtime_error` for `check_finite_result`). The comparisons are written as `!(tol > 0)` and `!(x[i] < x[i+1])`. Every comparison involving NaN is false, so this form rejects NaN, while `tol <= 0` would accept it.

## 3. `numcomp::ode` (ode_stepper)

```cpp
using ODEFunction = std::function<double(double, double)>;
using StepRule    = double (*)(const ODEFunction& f, double t, double y, double h);

std::vector<std::pair<double, double>>
run_fixed_steps(const ODEFunction& f, double t0, double y0, double t_end, double h, StepRule step);
```

`run_fixed_steps` does everything the four ODE methods have in common: checks, storing the curve, shortening the last step, and landing exactly on t_end. The only thing it does not know is the formula for one step, which each method passes as a **function pointer**:

```cpp
static double euler_step(const ode::ODEFunction& f, double t, double y, double h) { return y + h * f(t, y); }

std::vector<std::pair<double, double>> BasicODESolver::euler(/* f, t0, y0, t_end, h */) 
{
    return ode::run_fixed_steps(f, t0, y0, t_end, h, euler_step);
}
```

This is the *strategy* idea without class inheritance. The loop is fixed, and the varying part is plugged in. A named function pointer was chosen over a lambda or a virtual class because it is the easiest form to read and explain.

## 4. `numcomp::ode::run_multistep` (multistep, Week 5)

```cpp
using MultistepRule = double (*)(const ODEFunction& f, const std::vector<double>& slopes, double t, double y, double h);

std::vector<std::pair<double, double>>
run_multistep(const ODEFunction& f, double t0, double y0, double t_end, double h, int steps, MultistepRule rule);
```

The same idea as `run_fixed_steps`, for methods that use several earlier slopes (Adams-Bashforth and Adams-Moulton). The loop:
- checks the inputs (`check_ode_inputs`);
- evaluates f **once** per point and stores the slope, so an explicit step costs one new evaluation;
- creates the first `steps − 1` points with RK4, by calling the public `AdvancedODESolver::rk4`, because the multistep formula needs earlier slopes that do not exist yet at t0;
- also uses RK4 for a final step shorter than h (the Adams weights assume equal spacing);
- checks every new value is finite and lands exactly on t_end.

Each method supplies only its formula as a `MultistepRule`, for example `ab4_step` or `am4_step`.

## 5. `numcomp::linalg::solve_tridiagonal` (tridiagonal, Week 5)

```cpp
std::vector<double> solve_tridiagonal(std::vector<double> lower, std::vector<double> diag,
                                      std::vector<double> upper, std::vector<double> rhs);
```

The Thomas algorithm: a forward sweep removes the sub-diagonal, then back substitution finds x from the last row up. It takes O(m) operations for m unknowns. It used to live inside `CubicSpline::second_derivatives`. The finite difference method needs exactly the same solver, so it moved here, and both classes call it. A zero pivot throws `runtime_error`; vectors of different sizes throw `invalid_argument`.

## 6. Other shared code (not in src/internal)

| Helper | Where | Shared by |
|---|---|---|
| `find_pivot_row`, `back_substitution` (`static`) | `src/matrix_solver.cpp` | Gaussian elimination and LU |
| `BasicIntegrator::trapezoidal_rule` (reused) | `src/advanced_integrator.cpp` | Romberg's first column (composition between classes) |
| `AdvancedODESolver::rk4` (reused) | `src/internal/multistep.cpp` | starting values of Adams-Bashforth and Adams-Moulton |
| `Interpolator::chebyshev_nodes` (reused) | `src/interpolator.cpp` | Chebyshev approximation samples f at these nodes |
| `NumericalDifferentiator::central_difference` (reused) | `src/numerical_differentiator.cpp` | Richardson extrapolation |
| `dot`, `multiply`, `check_symmetric` (`static`) | `src/conjugate_gradient_solver.cpp` | the conjugate gradient iteration |
| `solve_corrector` (`static`) | `src/adams_moulton_solver.cpp` | AM2 and AM4 |
| `test_helpers.hpp` | `tests/` | all 15 test programs (see `test_framework.md`) |
| `example_helpers.hpp` | `examples/` | all 30 example programs (see `examples.md`) |
| `app::ui`, `app::catalog` | `src/app/` | every screen of the command-line app (see `command_line_app.md`) |

## 7. Effect

| | Copies without helpers | Copies with helpers |
|---|---|---|
| tol/max_iter check | 6 | 1 |
| finite-limits check | 4 | 1 |
| strictly-increasing loop | 2 | 1 |
| fixed-step ODE loop | 4 | 1 |
| multistep loop with RK4 start | 4 | 1 |
| Thomas algorithm | 2 | 1 |
| RK4 step | 2 | 1 |
| pivot search / back substitution | 2 / 2 | 1 / 1 |

The helpers are internal, so the public headers do not depend on them.
