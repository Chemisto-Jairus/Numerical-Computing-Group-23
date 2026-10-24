# Helpers Report

**Group 23 · Numerical Computing 2**

## The helper version: what was shared and why (WEEK 4)

Same library, same tests, same output, with each repeated piece of code written once.

### At a glance (count done by AI)

| Measure | Result |
|---|---|
| Lines of code (comments and blank lines not counted) | 2,889 → 1,442 |
| Helpers in use | 33 (16 library, 9 tests/build, 8 app) |
| Changes to the public headers in `include/` | 0 |
| Same checks pass, both ways round | 186 / 186 |

---

## 1. What was repeated, and how often

| Repeated block | Copies before | Copies after |
|---|---:|---:|
| tol / max_iter check | 4 | 1 |
| finite-limits check | 4 | 1 |
| strictly-increasing loop | 2 | 1 |
| ODE stepping loop | 4 | 1 |
| pivot search | 2 | 1 |
| back substitution | 2 | 1 |
| `report()` in tests | 9 | 1 |
| `close()` in tests | 8 | 1 |
| try/catch blocks in tests | 61 | 3 |
| `add_executable` blocks (`tests/CMakeLists.txt`) | 9 | 1 |

The 3 remaining try blocks are inside the helpers themselves (`expect_throw` and `expect_no_throw`).

| Lines of code | Repetition version | Helper version |
|---|---:|---:|
| `src/` (library + app) | 850 | 776 |
| &nbsp;&nbsp;of which `src/internal/` (new) | 0 | 132 |
| `tests/*.cpp` + `test_helpers.hpp` | 1,926 | 575 |
| `tests/CMakeLists.txt` + `run_all_tests.sh` | 113 | 91 |
| **Total** | **2,889** | **1,442** |

---

## 2. Library helpers

These are in `src/internal/` or marked `static`. **None of them is part of the public API.** Users still include only the headers in `include/numerical-computing/`.

| Helper | File | Kind | Replaces | Used by |
|---|---|---|---|---|
| `checks::check_tolerance(tol, max_iter)` | `src/internal/input_checks.*` | function | 4 copies of the `tol` / `max_iter` if-statement | bisection, newton_raphson, secant_method, fixed_point_iteration |
| `checks::check_finite_limits(a, b)` | `src/internal/input_checks.*` | function | 4 copies of the `std::isfinite` check | all 4 integration methods |
| `checks::check_integration_inputs(a, b, n)` | `src/internal/input_checks.*` | function | 3 copies of "n > 0 + finite limits" | trapezoidal_rule, midpoint_rule, simpsons_rule |
| `checks::check_data_points(x_vals, y_vals)` | `src/internal/input_checks.*` | function | 2 copies of the size check + strictly-increasing loop | Interpolator::linear_interpolate, CubicSpline::second_derivatives |
| `checks::find_interval(x, x_vals)` | `src/internal/input_checks.*` | function | 2 copies of the "which interval holds x" loop | Interpolator::linear_interpolate, CubicSpline::interpolate |
| `checks::check_ode_inputs(t0, t_end, h)` | `src/internal/input_checks.*` | function | 4 copies of the h / t_end / step-cap checks | run_fixed_steps (so euler, heun, rk3, rk4) |
| `checks::check_square_system(A, b)` | `src/internal/input_checks.*` | function | 2 copies of the empty / square / size checks | gaussian_elimination, lu_solve |
| `checks::MAX_POINTS` | `src/internal/input_checks.hpp` | constant | the number 1,000,000 typed in 5 places | ODE step cap, Chebyshev node cap |
| `ode::run_fixed_steps(f, t0, y0, t_end, h, step)` | `src/internal/ode_stepper.*` | function + function pointer | 4 copies of the ~30-line ODE loop (store first point, step, shorten last step, land on t_end) | euler, heun, rk3, rk4 |
| `euler_step`, `heun_step`, `rk3_step`, `rk4_step` | `src/basic_ode_solver.cpp`, `src/advanced_ode_solver.cpp` | static functions | — (these are the only lines that really differ between the methods) | passed to `run_fixed_steps` |
| `find_pivot_row(M, k)` | `src/matrix_solver.cpp` | static function | 2 copies of the partial-pivoting search | gaussian_elimination, lu_solve |
| `back_substitution(U, y)` | `src/matrix_solver.cpp` | static function | 2 copies of the back-substitution loop | gaussian_elimination, lu_solve |
| `BasicIntegrator::trapezoidal_rule` (reused) | `src/advanced_integrator.cpp` | class reuse (composition) | Romberg's own copy of the trapezoid loop | romberg_integration, column 0 |

---

## 3. Test helpers

`tests/test_helpers.hpp` is one small header-only test framework. It is still plain C++17: no download and no outside library, so the README's "no outside libraries" stays true.

| Helper | File | Kind | Replaces | Used by |
|---|---|---|---|---|
| `test::check(name, ok)` | `tests/test_helpers.hpp` | inline function | 9 copies of `report()` + the passed/failed counters | all 9 test files |
| `test::close(a, b, tol)` | `tests/test_helpers.hpp` | inline function | 8 copies of `close()` | all 9 test files |
| `test::expect_throw<Exception>(name, code)` | `tests/test_helpers.hpp` | template | 61 hand-written try/catch blocks (about 15 lines each) | every invalid-input test |
| `test::expect_no_throw(name, code)` | `tests/test_helpers.hpp` | template | try/catch used to show something does not throw | Romberg 20-rows test |
| `test::finish(module)` | `tests/test_helpers.hpp` | inline function | 9 copies of the summary line + "return 1 if failed" | every `main()` |
| `check_both(name, A, b, expected, tol)` | `tests/test_matrix_solver.cpp` | function | Gaussian/LU test pairs written twice | matrix tests |
| `BadInput` table + for loop | `tests/test_basic_ode_solver.cpp`, `test_advanced_ode_solver.cpp` | struct + loop | 12 near-identical error checks per file | ODE error tests |
| `foreach(test_name ${GROUP23_TESTS})` | `tests/CMakeLists.txt` | CMake loop | 9 copy-pasted `if`/`add_executable`/`target_link_libraries`/`add_test` blocks | every test program |
| `TESTS=( … )` + for loop | `tests/run_all_tests.sh` | bash array | 9 `run_test` lines | the test runner |

---

## 4. App helpers (`src/main.cpp`)

| Helper | File | Kind | Replaces | Used by |
|---|---|---|---|---|
| `read_number<T>(prompt)` | `src/main.cpp` | template | `read_int()` and `read_double()`, which were the same apart from the type | every prompt |
| `read_root_settings`, `read_limits`, `read_ode_settings`, `read_system` | `src/main.cpp` | functions | the same 2–12 prompts typed again in 4 (or 2) menu options | options 4–17 |
| `print_integral`, `print_ode_result`, `print_solution` | `src/main.cpp` | functions | the same 2–3 print lines in 4 (or 2) menu options | options 8–17 |

---

## 5. Before and after

### An ODE method

**Before**

```cpp
// basic_ode_solver.cpp - BEFORE (repetition version). euler() alone:
std::vector<std::pair<double, double>> BasicODESolver::euler(std::function<double(double, double)> f, double t0, double y0, double t_end, double h)
{
    if (!(h > 0) || !(t_end > t0)) {
        throw std::invalid_argument("Step size h must be strictly positive and t_end must be greater than t0.");
    }
    if ((t_end - t0) / h > 1000000) {
        throw std::invalid_argument("Too many steps: please use a bigger step size h.");
    }
    std::vector<std::pair<double, double>> result;
    result.push_back({t0, y0});
    double t = t0;
    double y = y0;
    double eps = 1e-9 * h;
    while (t_end - t > eps) {
        if (t + h > t_end) {
            h = t_end - t;
        }
        y = y + h * f(t, y);                 // <- the only Euler-specific line
        t = t + h;
        if (t_end - t <= eps) t = t_end;
        result.push_back({t, y});
    }
    return result;
}
// ...and heun(), rk3(), rk4() repeat every other line.
```

**After**

```cpp
// basic_ode_solver.cpp - AFTER (helper version)
static double euler_step(const ode::ODEFunction& f, double t, double y, double h)
{
    return y + h * f(t, y);
}

std::vector<std::pair<double, double>> BasicODESolver::euler(std::function<double(double, double)> f, double t0, double y0, double t_end, double h)
{
    return ode::run_fixed_steps(f, t0, y0, t_end, h, euler_step);
}
// The loop and the checks live once, in src/internal/ode_stepper.cpp.
```

### A "should throw" test

**Before**

```cpp
// BEFORE: one "should throw" test
void test_zero_intervals_throw()
{
    numcomp::BasicIntegrator integrator;
    bool trap_ok = false;
    try
    {
        integrator.trapezoidal_rule(square, 0.0, 1.0, 0);
    }
    catch (const std::invalid_argument&)
    {
        trap_ok = true;
    }
    catch (...)
    {
    }
    report("Trapezoidal: n = 0 throws invalid_argument", trap_ok);
}
```

**After**

```cpp
// AFTER: the same test with test_helpers.hpp
test::expect_throw<std::invalid_argument>("Trapezoidal: n = 0 throws invalid_argument",
    [&] { integrator.trapezoidal_rule(square, 0.0, 1.0, 0); });
```

---

