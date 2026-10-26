# Example programs

| | |
|---|---|
| Owners | Each module owner wrote the two examples for their module; Nabakooza Bridget Maria (Member 9) wrote `examples/CMakeLists.txt` and `examples/example_helpers.hpp` |
| Folder | `examples/` |
| Count | 30 programs: 2 per module class (15 classes) |

## 1. The list

| Module | Example 1 (the methods) | Example 2 (applied problems) |
|---|---|---|
| BasicRootFinder | `example_basic_root_finder` | `example_basic_root_finder_applied`: projectile, Kepler's equation, break-even |
| AdvancedRootFinder | `example_advanced_root_finder` | `example_advanced_root_finder_applied`: loan rate, Babylonian roots, tank depth |
| Interpolator | `example_interpolator` (also Chebyshev approximation) | `example_interpolator_applied`: temperature table, choosing sample points, one global Chebyshev polynomial |
| CubicSpline | `example_cubic_spline` | `example_cubic_spline_applied`: speed profile, convergence study |
| BasicIntegrator | `example_basic_integrator` | `example_basic_integrator_applied`: distance, probability, work |
| AdvancedIntegrator | `example_advanced_integrator` | `example_advanced_integrator_applied`: erf(1), arc length, cost |
| BasicODESolver | `example_basic_ode_solver` | `example_basic_ode_solver_applied`: cooling, logistic growth |
| AdvancedODESolver | `example_advanced_ode_solver` | `example_advanced_ode_solver_applied`: RC circuit, drug infusion |
| MatrixSolver | `example_matrix_solver` | `example_matrix_solver_applied`: circuit currents, parabola fit, alloy mixing |
| NumericalDifferentiator | `example_numerical_differentiator` | `example_numerical_differentiator_applied`: car velocity and acceleration, marginal cost, Newton with a numerical derivative |
| ConjugateGradientSolver | `example_conjugate_gradient_solver` | `example_conjugate_gradient_solver_applied`: spring chain, heated rod (99 unknowns), line of best fit |
| AdamsBashforthSolver | `example_adams_bashforth_solver` | `example_adams_bashforth_solver_applied`: skydiver terminal velocity, bacterial growth |
| AdamsMoultonSolver | `example_adams_moulton_solver` | `example_adams_moulton_solver_applied`: second-order reaction, draining tank |
| RungeKuttaFehlbergSolver | `example_runge_kutta_fehlberg_solver` | `example_runge_kutta_fehlberg_solver_applied`: parachute opening, drug elimination |
| FiniteDifferenceSolver | `example_finite_difference_solver` | `example_finite_difference_solver_applied`: cooling fin, hanging cable |

## 2. How each example is organised

Every example includes its module header and `example_helpers.hpp`, and uses the helpers in namespace `demo`:

| Helper | Job |
|---|---|
| `demo::title(name, subtitle)` | framed title |
| `demo::section(name, code)` | runs one part inside try/catch, so one failing part cannot stop the demo |
| `demo::compare(label, approx, exact)` | result, exact value and error on one line |
| `demo::value(label, v)` | a labelled number |
| `demo::table_header`, `demo::table_row` | aligned tables (convergence studies) |
| `demo::show_rejected(label, code)` | shows that an invalid call is rejected, and prints the library's message |
| `demo::finish()` | exit code 0 if every section completed, 1 otherwise |

Following the group rules, each example has its own `main()`, uses valid inputs (except in the clearly labelled "rejected" demonstrations), keeps library calls inside try/catch, and prints labelled results.

## 3. Building and running

`examples/CMakeLists.txt` builds one executable per example (each wrapped in `if(EXISTS …)`) and registers each with ctest:

```bash
cmake -S . -B build && cmake --build build
./build/examples/example_cubic_spline
cd build && ctest -R example      # run only the examples
bash tests/run_all_tests.sh       # tests, then examples, from the repository root
```
