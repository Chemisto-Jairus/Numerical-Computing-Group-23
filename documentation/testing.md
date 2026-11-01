# Testing approach and results

## 1. Approach

1. **One test program per module.** Each module owner wrote the tests for their own class. The Matrix Solver tests were written independently by the QA lead, so the same person did not both write and test that module.
2. **Three kinds of test in every file**, as Section 4 of the project instructions requires:
   - *Normal use:* results compared with known exact answers (for example √2, ∫₀^π sin x dx = 2, e^(−2), sin(πx) for a boundary value problem).
   - *Edge cases:* data end points, reversed intervals, 1×1 and 2-point inputs, a last ODE step that is shortened, a zero pivot that needs a row swap, a run shorter than the multistep starter, the corner of |x|, the smallest finite difference grid.
   - *Invalid input:* every documented exception is triggered and its **type** is checked.
3. **Mathematical property checks**, not only single values:
   - observed order of convergence (error ratios ≈ 2, 4, 8, 16 for Euler, Heun, RK3, RK4; ≈ 4 and 16 for AB2/AM2 and AB4/AM4; ≈ 2, 4, 16 for forward, central and Richardson differences; ≈ 4 for finite differences);
   - exactness where theory says the result is exact (Simpson for cubics, trapezoid for straight lines, Romberg rows, AB4 for cubic slopes, Chebyshev approximation for a cubic with four terms, finite differences for a quadratic solution);
   - behaviour that defines a method: the spline passing through every data point, conjugate gradient finishing within n iterations, RKF45 meeting its tolerance and using more steps for a tighter one, the Adams-Moulton corrector failing for a stiff problem with a large step.
4. **No external framework.** A small header-only framework (`tests/test_helpers.hpp`) keeps the "no outside libraries" promise and works the same in Debug and Release builds.
5. **Examples are tests too.** All 30 example programs are registered with ctest and must exit with code 0.

## 2. Results

| Check | Result |
|---|---|
| Unit-test programs | 15 / 15 pass |
| Individual checks | 335 / 335 pass |
| Example programs | 30 / 30 run to completion (exit code 0) |
| `ctest` entries (tests + examples) | 45 / 45 pass |
| Build, CMake, Debug and Release, `-Wall -Wextra -pedantic` | 0 warnings |
| Build with plain g++ (no CMake) | all 45 programs and the app build and pass |
| Doxygen (`doxygen docs/Doxyfile`) with every warning enabled | 0 warnings |
| New tests run against the Week 3 code | 13 failing checks + 1 crash (the bugs they guard against) |
| The CLI app driven by a scripted session through every menu, including invalid input and end of input | no crash; every error reported with a message |

Measured convergence ratios (from the tests and examples):

| Method | Expected ratio when h halves | Measured |
|---|---|---|
| Trapezoidal (sin on [0, π]) | 4 | 4.03, 4.01, 4.00 |
| Simpson (sin on [0, π]) | 16 | 16.9, 16.2, 16.1 |
| Euler (dy/dt = y) | 2 | 1.92, 1.96, 1.98 |
| Heun (dy/dt = y) | 4 | 3.85, 3.93, 3.96 |
| RK3 (dy/dt = −2y) | 8 | 9.4, 8.7, 8.3 |
| RK4 (dy/dt = −2y) | 16 | 18.9, 17.4, 16.7 |
| Cubic spline (sin on [0, π]) | 16 | 16.9, 16.2, 16.1, 16.0 |
| Forward difference (eˣ at 0) | 2 | 2.03, 2.02, 2.01 |
| Central difference (eˣ at 0) | 4 | 4.00, 4.00, 4.00 |
| Richardson extrapolation (eˣ at 0) | 16 | 16.0, 16.0, 16.0 |
| Adams-Bashforth 2 (dy/dt = −2y) | 4 | 4.22, 3.90, 3.91 |
| Adams-Bashforth 4 (dy/dt = −2y) | 16 | 16.7, 15.4, 15.6 |
| Adams-Moulton 2 (dy/dt = −2y) | 4 | 4.22, 3.90 |
| Adams-Moulton 4 (dy/dt = −2y) | 16 | 16.0, 14.8 |
| Finite differences (y″ = −π² sin πx) | 4 | 4.01, 4.00, 4.00 |

Methods without a step size to halve:

| Method | Property checked | Measured |
|---|---|---|
| RKF45 (dy/dt = −2y) | error falls with tol | tol 1e-4 → 1.8e-5 (6 steps), 1e-6 → 2.1e-7 (15), 1e-8 → 2.2e-9 (44) |
| Chebyshev approximation (eˣ on [0, 1]) | error falls geometrically with terms | 6 terms 1.2e-6, 10 terms 9.1e-13, 12 terms 2.7e-15 |
| Conjugate gradient | iterations ≤ n | tridiagonal systems n = 2 … 40 converged in n/2 iterations |

## 3. Bugs found by testing

From the Week 3 debugs : the bisection reversed-interval bug, NaN inputs passing validation in several modules, Chebyshev node allocation without a limit, the missing catch-all handler, and the test-runner problems.

## 4. How to reproduce

From a fresh clone, in the repository root:

```bash
cmake -S . -B build
cmake --build build
cd build && ctest --output-on-failure
cd .. && bash tests/run_all_tests.sh
```

