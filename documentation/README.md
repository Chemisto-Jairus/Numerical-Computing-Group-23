# Group 23 Numerical Computing 2: documentation

This folder holds all project documentation apart from the top-level `README.md`. Generated API documentation goes to `docs/api/` (run `doxygen docs/Doxyfile` from the repository root; the folder is not stored in Git).

## The library at a glance

| Class | Methods | Module guide |
|---|---|---|
| `BasicRootFinder` | Bisection, Newton-Raphson | [modules/basic_root_finder.md](modules/basic_root_finder.md) |
| `AdvancedRootFinder` | Secant, Fixed-point iteration | [modules/advanced_root_finder.md](modules/advanced_root_finder.md) |
| `Interpolator` | Linear interpolation, Chebyshev nodes, Chebyshev approximation | [modules/interpolator.md](modules/interpolator.md) |
| `CubicSpline` | Natural cubic spline | [modules/cubic_spline.md](modules/cubic_spline.md) |
| `NumericalDifferentiator` | Forward, backward, central differences, second derivative, Richardson | [modules/numerical_differentiator.md](modules/numerical_differentiator.md) |
| `BasicIntegrator` | Trapezoidal, Midpoint | [modules/basic_integrator.md](modules/basic_integrator.md) |
| `AdvancedIntegrator` | Simpson's 1/3, Romberg | [modules/advanced_integrator.md](modules/advanced_integrator.md) |
| `BasicODESolver` | Euler, Heun | [modules/basic_ode_solver.md](modules/basic_ode_solver.md) |
| `AdvancedODESolver` | RK3, RK4 | [modules/advanced_ode_solver.md](modules/advanced_ode_solver.md) |
| `AdamsBashforthSolver` | Adams-Bashforth 2 and 4 | [modules/adams_bashforth_solver.md](modules/adams_bashforth_solver.md) |
| `AdamsMoultonSolver` | Adams-Moulton 2 and 4 | [modules/adams_moulton_solver.md](modules/adams_moulton_solver.md) |
| `RungeKuttaFehlbergSolver` | Adaptive RKF45 | [modules/runge_kutta_fehlberg_solver.md](modules/runge_kutta_fehlberg_solver.md) |
| `FiniteDifferenceSolver` | Finite difference method for boundary value problems | [modules/finite_difference_solver.md](modules/finite_difference_solver.md) |
| `MatrixSolver` | Gaussian elimination, LU decomposition | [modules/matrix_solver.md](modules/matrix_solver.md) |
| `ConjugateGradientSolver` | Conjugate gradient | [modules/conjugate_gradient_solver.md](modules/conjugate_gradient_solver.md) |

Together these cover all 18 methods on the course list: Bisection, Linear interpolation, Trapezoidal, Midpoint, Newton-Raphson, Secant, Fixed-point, Numerical differentiation, Simpson, Euler, Cubic spline, Chebyshev approximation, Conjugate gradient, Adams-Bashforth, Adams-Moulton, Runge-Kutta-Fehlberg, Romberg and the Finite difference method. Heun, RK3, RK4, LU and Chebyshev nodes are extras.

Supporting parts:

- [modules/shared_helpers.md](modules/shared_helpers.md): the internal input checks, the two shared ODE loops and the tridiagonal solver.
- [modules/command_line_app.md](modules/command_line_app.md): the interactive app.
- [modules/examples.md](modules/examples.md): the 30 example programs.
- [modules/test_framework.md](modules/test_framework.md): how the tests work.

## Reports

| File | Contents |
|---|---|
| [algorithm-report.pdf](algorithm-report.pdf) / [.md](algorithm-report.md) | The algorithms implemented |
| [testing.md](testing.md) | Testing approach and results |

## Configuration

- [Doxyfile](Doxyfile): Doxygen settings. Every undocumented item produces a warning, and the current code produces none.
