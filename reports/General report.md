# General Report - Numerical Computing 2 (Group 23)

## Table of contents

[Group Members and Contributions](<General report.md#group-members-and-contributions>)

[Requirements and Dependencies](<General report.md#requirements-and-dependencies>)

[The Library at a Glance](<General report.md#the-library-at-a-glance>)

[Algorithm Implementation Summary](<General report.md#algorithm-implementation-summary-see-module-reports>)

[Command-line app: numcomp_app](<General report.md#command-line-app-numcomp_app>)

[Project Structure](<General report.md#project-structure>)

[Concepts  used in our library](<General report.md#concepts--used-in-our-library>)

[Additional Documentation & Links](<General report.md#additional-documentation--links>)

[AI clause for entire project](<General report.md#ai-clause-for-entire-project>)


## Group Members and Contributions

| Name | Student Number | Project Role / Assigned Module | 
| ----- | ----- | ----- | 
| CHEMISTO JAIRUS | 25/U/07928/PS | Member 1 (Lead) / BasicRootFinder / NumericalDifferentiator / shared helpers incl. the multistep loop and tridiagonal solver, CMake, merges, reports | 
| BATTE DERRICK CALVIN | 25/U/0717 | Member 2 / Interpolator incl. Chebyshev approximation, CubicSpline, MatrixSolver | 
| Aogon Sharon | 25/U/08690/PS | Member 3 / BasicIntegrator / FiniteDifferenceSolver / Class layouts(pdf) | 
| BAKALU VALERIA | 25/U/07924/PS | Member 4 / AdvancedRootFinder, ConjugateGradientSolver, Inheritance of the librabry(jpg) | 
| WASSWA GEORGE MPANGA | 25/U/08793/PS | Member 5 / AdvancedIntegrator / RungeKuttaFehlbergSolver | 
| MWANJE IAN | 25/U/08757/PS | Member 6 / BasicODESolver / AdamsBashforthSolver / ODE stepper | 
| MUWANGUZI SAMUEL | 25/U/0843 | Member 7 / AdvancedODESolver / AdamsMoultonSolver / ODE stepper | 
| KYEYUNE PRAISE PAULINE | 25/U/07943/PS | Member 8 / test framework/  test runner / Matrix Solver tests / QA; not linear algebra | 
| Nabakooza Bridget Maria | 25/U/07592/PS | Member 9 /  numcomp_app / examples build / example helpers | 

## Commit history proof of contribution

You can view the contributions made by [Member 1()](commits?author=username).
You can view the contributions made by [Member 2(Batte Derrick Calvin)](https://github.com/Chemisto-Jairus/Numerical-Computing-Group-23/commits/main/?author=itsBDC).
You can view the contributions made by [Member 3()](commits?author=username).
You can view the contributions made by [Member 4()](commits?author=username).
You can view the contributions made by [Member 5()](commits?author=username).
You can view the contributions made by [Member 6()](commits?author=username).
You can view the contributions made by [Member 7()](commits?author=username).
You can view the contributions made by [Member 8()](commits?author=username).
You can view the contributions made by [Member 9()](commits?author=username).

## Requirements and Dependencies

- **Compiler:** needs C++17 support. 
- **CMake:** version 3.10 or newer. 
- **Git:** only to clone the repository. 
- **Outside libraries:** None, everything is written by us from scratch.

## The Library at a Glance

| Class | Methods | Module guide | 
| ----- | ----- | ----- | 
| `BasicRootFinder` | Bisection, Newton-Raphson | [modules/basic_root_finder.md](../documentation/modules/basic_root_finder.md) | 
| `AdvancedRootFinder` | Secant, Fixed-point iteration | [modules/advanced_root_finder.md](../documentation/modules/advanced_root_finder.md) | 
| `Interpolator` | Linear interpolation, Chebyshev nodes, Chebyshev approximation | [modules/interpolator.md](../documentation/modules/interpolator.md) | 
| `CubicSpline` | Natural cubic spline | [modules/cubic_spline.md](../documentation/modules/cubic_spline.md) | 
| `NumericalDifferentiator` | Forward, backward, central differences, second derivative, Richardson | [modules/numerical_differentiator.md](../documentation/modules/numerical_differentiator.md) | 
| `BasicIntegrator` | Trapezoidal, Midpoint | [modules/basic_integrator.md](../documentation/modules/basic_integrator.md) | 
| `AdvancedIntegrator` | Simpson's 1/3, Romberg | [modules/advanced_integrator.md](../documentation/modules/advanced_integrator.md) | 
| `BasicODESolver` | Euler, Heun | [modules/basic_ode_solver.md](../documentation/modules/basic_ode_solver.md) | 
| `AdvancedODESolver` | RK3, RK4 | [modules/advanced_ode_solver.md](../documentation/modules/advanced_ode_solver.md) | 
| `AdamsBashforthSolver` | Adams-Bashforth 2 and 4 | [modules/adams_bashforth_solver.md](../documentation/modules/adams_bashforth_solver.md) | 
| `AdamsMoultonSolver` | Adams-Moulton 2 and 4 | [modules/adams_moulton_solver.md](../documentation/modules/adams_moulton_solver.md) | 
| `RungeKuttaFehlbergSolver` | Adaptive RKF45 | [modules/runge_kutta_fehlberg_solver.md](../documentation/modules/runge_kutta_fehlberg_solver.md) | 
| `FiniteDifferenceSolver` | Finite difference method for boundary value problems | [modules/finite_difference_solver.md](../documentation/modules/finite_difference_solver.md) | 
| `MatrixSolver` | Gaussian elimination, LU decomposition | [modules/matrix_solver.md](../documentation/modules/matrix_solver.md) | 
| `ConjugateGradientSolver` | Conjugate gradient | [modules/conjugate_gradient_solver.md](../documentation/modules/conjugate_gradient_solver.md) | 

## Algorithm Implementation Summary [See module reports](../documentation/modules)

The `numcomp2` library implements eighteen core algorithms and five related methods across seven mathematical areas. All logic is implemented in stateless classes, sharing central helpers (like tridiagonal solvers and generic ODE loops) to keep code DRY.

### 1. Root Finding

Implements Bisection ($O(N)$), Newton-Raphson (quadratic), Secant (superlinear $1.618$), and Fixed-Point iteration. Safeguards include bracket checking, zero-derivative exceptions, and strict maximum iteration cutoffs.

### 2. Interpolation & Approximation

Features Piecewise Linear Interpolation and Natural Cubic Spline (solved in $O(N)$ without pivoting via the Thomas algorithm). It mitigates Runge's phenomenon using Chebyshev nodes and evaluates minimax polynomials via Clenshaw's recurrence algorithm in $O(N)$.

### 3. Differentiation

Evaluates derivatives using Forward/Backward (1st order) and Central/Second differences (2nd order). Utilizes Richardson Extrapolation for 4th-order accuracy, with safeguards addressing rounding errors at extremely small step sizes.

### 4. Integration

Implements Trapezoidal and Midpoint (2nd order), and Simpson's 1/3 rule (4th order). Utilizes Romberg integration to systematically eliminate even-powered error terms.

### 5. ODE Solvers (Initial Value Problems)

* **One-step:** Euler, Heun, RK3, and RK4.
* **Multistep:** Explicit Adams-Bashforth (AB2, AB4) and implicit Adams-Moulton (AM2, AM4). Implicit methods are resolved using fixed-point iteration on an Euler predictor.
* **Adaptive:** Runge-Kutta-Fehlberg (RKF45), which scales step size dynamically based on embedded error estimates bounded by user tolerance.

## 6. Linear & Boundary Value Systems

* **Linear Systems:** Gaussian Elimination and LU Decomposition using partial pivoting. Conjugate Gradient iterative solving for symmetric positive definite (SPD) systems.
* **BVPs:** Solves linear continuous models using the finite difference method, mapping central difference approximations onto a tridiagonal matrix to yield $O(h^2)$ precision.

## 7. Verification and Limitations

Verified via 15 standard test programs executing 335 rigorous checks. Exactness and expected theoretical convergence orders (e.g., halving step sizes yielding error reductions of 2, 4, or 16) have been mathematically verified across all methods. Identified limitations (such as explicit ODE instability on stiff problems, or lack of preconditioning in Conjugate Gradient) are thoroughly documented in individual module guides.

## Command-line app: numcomp_app

| | |
|---|---|
| Owners | Nabakooza Bridget Maria (Member 9), with Chemisto Jairus (Member 1) |
| Files | `src/main.cpp`, `src/app/console_ui.*`, `src/app/catalog.*`, `src/app/module_menus.*` |
| Build target | `numcomp_app` (linked against the `numcomp2` library) |

![SEE THE UI](../documentation/Numcomp_app.png)

### 1. What it does

An interactive console program that demonstrates every method of the library. It opens with a large **GROUP 23** banner, then guides the user in three steps:

1. **Topic:** Root finding, Interpolation (and Chebyshev approximation), Differentiation, Integration, Initial value ODEs, Boundary value ODEs, Linear systems, or About. Every one of the 15 library classes is reachable.
2. **Method:** for example Bisection, Newton-Raphson, Secant, Fixed-point, or *Compare all*. The ODE topic first asks for a **family** (one-step, multistep, adaptive, or compare all nine methods), then for the method in that family.
3. **Problem:** a preset equation, function, integrand, model, boundary value problem, data set or matrix from the catalog, or the user's own (a custom cubic, custom data points, a custom linear ODE, a custom constant-coefficient BVP, a custom matrix up to 6×6).

Suggested parameters are shown in brackets, and **Enter accepts them**. Results appear next to the exact answer with the absolute error. Root finders also show the residual f(root) and the number of function evaluations.

### 2. Structure

| File | Role |
|---|---|
| `src/main.cpp` | Banner and main menu; dispatches to a topic menu; ends cleanly when the input ends. |
| `src/app/console_ui.*` | All input and output: line-based input with defaults and validation, numbered menus, framed headers, aligned results and tables, the banner. ASCII only, 78 columns wide. |
| `src/app/catalog.*` | Preset problems with their exact answers: 7 root equations, 5 fixed-point forms, 8 integrals (with antiderivatives, also used for Chebyshev approximation), 7 functions with exact first and second derivatives, 7 ODE models (with exact solutions), 5 boundary value problems (with exact solutions), 5 data sets, 8 linear systems (2 of them symmetric positive definite, for conjugate gradient). |
| `src/app/module_menus.*` | One menu per topic (seven topics and the About screen). Each calculation runs inside `run_safely`, which turns library exceptions into messages. |

### 3. How input is handled

- Input is read **one whole line at a time** (`std::getline`). A number must use the whole line: "2abc" is rejected, not read as 2.
- An empty line accepts the default shown in brackets.
- Menu choices must be whole numbers in range. Anything else gets a clear message and the question is asked again.
- A trailing `\r` (from Windows line endings) is removed.
- When input ends (Ctrl+D / Ctrl+Z, or the end of a piped file), a special `InputEnded` object is thrown. It is **not** derived from `std::exception`, so the calculation handlers do not catch it. `main()` catches it and exits with a goodbye message.

### 4. How errors are handled

Every library call runs inside `run_safely`, which catches from the most specific type to the most general:

| Caught | Message prefix |
|---|---|
| `std::invalid_argument` | `[!] Invalid input:` |
| `std::out_of_range` | `[!] Out of range:` |
| `std::runtime_error` | `[!] Calculation failed:` |
| `std::exception` (anything else, e.g. `bad_alloc`) | `[!] Unexpected error:` |

Results are computed **before** anything is printed, so an error never leaves a half-printed line. In the "compare" screens every method runs in its own handler. For example, conjugate gradient can reject a non-symmetric matrix while Gaussian elimination and LU still show their answers.

### 4a. What each topic offers

| Topic | Methods | Extra screens |
|---|---|---|
| Root finding | Bisection, Newton-Raphson, Secant, Fixed-point | compare three methods (with function-evaluation counts) |
| Interpolation | Linear, cubic spline, Chebyshev nodes, Chebyshev approximation | compare linear and spline; approximation error table |
| Differentiation | Forward, backward, central, second derivative, Richardson | error table for h = 0.1 … 1e-6 (shows rounding taking over) |
| Integration | Trapezoidal, Midpoint, Simpson, Romberg | compare all four |
| Initial value ODEs | One-step: Euler, Heun, RK3, RK4; multistep: AB2, AB4, AM2, AM4; adaptive: RKF45 | compare all nine at t_end; RKF45 table shows the step used |
| Boundary value ODEs | Finite difference method | convergence study n = 10 … 80 with error ratios |
| Linear systems | Gaussian elimination, LU, conjugate gradient | compare all three, with residuals |

### 5. Why it is written this way

| Decision | Reason |
|---|---|
| Separate `app/` files instead of one long main.cpp | Each file has one job (screen, data, menus), so each is short enough to read and test. |
| Catalog entries carry exact answers | The app can show the error for every method, which is the point of a numerical-methods demo. |
| "Compare all" options | Shows side by side why one method is preferred over another (accuracy, evaluations). |
| ODE methods grouped by family | Nine methods in one list is hard to read; the family question also teaches how the methods differ. |
| Tables with five or more columns use narrower columns; long error messages are wrapped | Every line stays within 78 characters. |
| ASCII-only output | Box-drawing characters show as garbage in the default Windows console. |
| Defaults for every parameter | A first-time user can press Enter through a whole demo. |

### Building and running in git bash

```bash
cmake -G "MinGW Makefiles" -S . -B build
cmake --build build
./build/numcomp_app.exe
```

With g++ only, see `docs/guides/build-guide.html`.


## Project Structure

We used the following structure for our project:

```

Numerical-Computing-Group-23/
├── documentation/
│   │
│   │   
│   │ 
│   ├── guides/
│   │       ├── doxygen_comments_guide.md
│   │       ├── install_guide.md
│   │       └── build_guide.md
│   │
│   │
│   ├── examples results 
│   │       ├── results_adams_bashforth_solver.md
│   │       ├── results_adams_moulton_solver.md
│   │       ├── results_advanced_integrator.md
│   │       ├── results_advanced_ode_solver.md
│   │       ├── results_advanced_root_finder.md
│   │       ├── results_basic_integrator.md
│   │       ├── results_basic_ode_solver.md
│   │       ├── results_basic_root_finder.md
│   │       ├── results_conjugate_gradient_solver.md
│   │       ├── results_cubic_spline.md
│   │       ├── results_finite_difference_solver.md
│   │       ├── results_interpolator.md
│   │       ├── results_matrix_solver.md
│   │       ├── results_numerical_differentiator.md
│   │       └── results_runge_kutta_fehlberg_solver.md
│   │        
│   ├── modules/ 
│   │   ├── adams_bashforth_solver.md
│   │   ├── adams_moulton_solver.md
│   │   ├── advanced_integrator.md
│   │   ├── advanced_ode_solver.md
│   │   ├── advanced_root_finder.md
│   │   ├── basic_integrator.md
│   │   ├── basic_ode_solver.md
│   │   ├── basic_root_finder.md
│   │   ├── command_line_app.md
│   │   ├── conjugate_gradient_solver.md
│   │   ├── cubic_spline.md
│   │   ├── examples.md
│   │   ├── finite_difference_solver.md
│   │   ├── interpolator.md
│   │   ├── matrix_solver.md
│   │   ├── numerical_differentiator.md
│   │   ├── runge_kutta_fehlberg_solver.md
│   │   ├── shared_helpers.md
│   │   └── test_framework.md
│   │
│   ├── test results 
│   │       ├── test_results_adams_bashforth_solver.md
│   │       ├── test_results_adams_moulton_solver.md
│   │       ├── test_results_advanced_integrator.md
│   │       ├── test_results_advanced_ode_solver.md
│   │       ├── test_results_advanced_root_finder.md
│   │       ├── test_results_basic_integrator.md
│   │       ├── test_results_basic_ode_solver.md
│   │       ├── test_results_basic_root_finder.md
│   │       ├── test_results_conjugate_gradient_solver.md
│   │       ├── test_results_cubic_spline.md
│   │       ├── test_results_finite_difference_solver.md
│   │       ├── test_results_interpolator.md
│   │       ├── test_results_matrix_solver.md
│   │       ├── test_results_numerical_differentiator.md
│   │       └── test_results_runge_kutta_fehlberg_solver.md
│   ├── README.md 
│   ├── algorithm-report.md 
│   ├── Inheritance of Numcomp2 by Valeria.jpg
│   ├── Shared helpers by Bridget.pdf
│   ├── Class layouts by Sharon.pdf
│   └── testing.md 
├── examples/
│   ├── CMakeLists.txt
│   ├── example_adams_bashforth_solver.cpp
│   ├── example_adams_bashforth_solver_applied.cpp
│   ├── example_adams_moulton_solver.cpp
│   ├── example_adams_moulton_solver_applied.cpp
│   ├── example_advanced_integrator.cpp
│   ├── example_advanced_integrator_applied.cpp
│   ├── example_advanced_ode_solver.cpp
│   ├── example_advanced_ode_solver_applied.cpp
│   ├── example_advanced_root_finder.cpp
│   ├── example_advanced_root_finder_applied.cpp
│   ├── example_basic_integrator.cpp
│   ├── example_basic_integrator_applied.cpp
│   ├── example_basic_ode_solver.cpp
│   ├── example_basic_ode_solver_applied.cpp
│   ├── example_basic_root_finder.cpp
│   ├── example_basic_root_finder_applied.cpp
│   ├── example_conjugate_gradient_solver.cpp
│   ├── example_conjugate_gradient_solver_applied.cpp
│   ├── example_cubic_spline.cpp
│   ├── example_cubic_spline_applied.cpp
│   ├── example_finite_difference_solver.cpp
│   ├── example_finite_difference_solver_applied.cpp
│   ├── example_interpolator.cpp
│   ├── example_interpolator_applied.cpp
│   ├── example_matrix_solver.cpp
│   ├── example_matrix_solver_applied.cpp
│   ├── example_numerical_differentiator.cpp
│   ├── example_numerical_differentiator_applied.cpp
│   ├── example_runge_kutta_fehlberg_solver.cpp
│   └── example_runge_kutta_fehlberg_solver_applied.cpp
├── include/
│   └── numerical-computing/
│       ├── adams_bashforth_solver.hpp 
│       ├── adams_moulton_solver.hpp 
│       ├── advanced_integrator.hpp
│       ├── advanced_ode_solver.hpp
│       ├── advanced_root_finder.hpp
│       ├── basic_integrator.hpp
│       ├── basic_ode_solver.hpp
│       ├── basic_root_finder.hpp
│       ├── conjugate_gradient_solver.hpp 
│       ├── cubic_spline.hpp
│       ├── finite_difference_solver.hpp 
│       ├── interpolator.hpp
│       ├── matrix_solver.hpp
│       ├── numcomp.hpp 
│       ├── numerical_differentiator.hpp 
│       └── runge_kutta_fehlberg_solver.hpp 
├── reports/
│   ├── week-01.md
│   ├── week-02.md
│   ├── week-03.md
│   ├── week-04.md
│   ├── Final week report 
│   └── General report 
├── src/
│   ├── app/ 
│   │   ├── catalog.cpp
│   │   ├── catalog.hpp
│   │   ├── console_ui.cpp
│   │   ├── console_ui.hpp
│   │   ├── module_menus.cpp
│   │   └── module_menus.hpp
│   ├── internal/
│   │   ├── input_checks.cpp
│   │   ├── input_checks.hpp
│   │   ├── multistep.cpp 
│   │   ├── multistep.hpp 
│   │   ├── ode_stepper.cpp
│   │   ├── ode_stepper.hpp
│   │   ├── tridiagonal.cpp 
│   │   └── tridiagonal.hpp 
│   ├── adams_bashforth_solver.cpp 
│   ├── adams_moulton_solver.cpp 
│   ├── advanced_integrator.cpp 
│   ├── advanced_ode_solver.cpp
│   ├── advanced_root_finder.cpp
│   ├── basic_integrator.cpp
│   ├── basic_ode_solver.cpp
│   ├── basic_root_finder.cpp
│   ├── conjugate_gradient_solver.cpp 
│   ├── cubic_spline.cpp
│   ├── finite_difference_solver.cpp 
│   ├── interpolator.cpp
│   ├── main.cpp
│   ├── matrix_solver.cpp
│   ├── numerical_differentiator.cpp
│   └── runge_kutta_fehlberg_solver.cpp 
├── tests/
│   ├── CMakeLists.txt
│   ├── run_all_tests.sh 
│   ├── test_adams_bashforth_solver.cpp 
│   ├── test_adams_moulton_solver.cpp 
│   ├── test_advanced_integrator.cpp
│   ├── test_advanced_ode_solver.cpp
│   ├── test_advanced_root_finder.cpp
│   ├── test_basic_integrator.cpp
│   ├── test_basic_ode_solver.cpp
│   ├── test_basic_root_finder.cpp
│   ├── test_conjugate_gradient_solver.cpp 
│   ├── test_cubic_spline.cpp
│   ├── test_finite_difference_solver.cpp 
│   ├── test_helpers.hpp
│   ├── test_interpolator.cpp
│   ├── test_matrix_solver.cpp
│   ├── test_numerical_differentiator.cpp 
│   └── test_runge_kutta_fehlberg_solver.cpp 
├── .gitignore
├── CMakeLists.txt
├── Doxyfile
├── LICENSE
└── README.md

```

## Concepts  used in our library

### Inheritance of our library

![See this image](<../documentation/Inheritance of Numcomp2 by Valeria.jpg>)

### Class layous of our library

![layout 1](<../documentation/class layouts 1 by Sharon .png>)
![layout 2](<../documentation/class layouts 2 by Sharon.png>)

### Relationship between Shared helpers and classes by Bridget

![see this image](<../documentation/Relationship between Shared helpers and classes by Bridget.png>)


## Shared internal helpers: input checks, ODE loops and the tridiagonal solver

| | |
|---|---|
| Owners | `input_checks`, `multistep`, `tridiagonal`: Chemisto Jairus (Member 1); `ode_stepper`: Mwanje Ian (Member 6) and Muwanguzi Samuel (Member 7) |
| Files | `src/internal/input_checks.hpp/.cpp`, `src/internal/ode_stepper.hpp/.cpp`, `src/internal/multistep.hpp/.cpp`, `src/internal/tridiagonal.hpp/.cpp` |
| Public? | **No.** These live in `src/internal/`, not in `include/`, so library users never see them. |

### 1. Why they exist

The project rules say the library should "avoid unnecessary duplication of code" (Section 3). Before these helpers:
- the same `tol`/`max_iter` check was typed in four root-finding methods;
- the same "x strictly increasing" loop appeared in two classes;
- the ~30-line ODE time loop was copied into four methods.

A fix to one copy did not reach the others, and the error messages could drift apart. Now each rule is written **once** and every method calls it.

### 2. `numcomp::checks` (input_checks)

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

### 3. `numcomp::ode` (ode_stepper)

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

### 4. `numcomp::ode::run_multistep` (multistep, Week 5)

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

### 5. `numcomp::linalg::solve_tridiagonal` (tridiagonal, Week 5)

```cpp
std::vector<double> solve_tridiagonal(std::vector<double> lower, std::vector<double> diag,
                                      std::vector<double> upper, std::vector<double> rhs);
```

The Thomas algorithm: a forward sweep removes the sub-diagonal, then back substitution finds x from the last row up. It takes O(m) operations for m unknowns. It used to live inside `CubicSpline::second_derivatives`. The finite difference method needs exactly the same solver, so it moved here, and both classes call it. A zero pivot throws `runtime_error`; vectors of different sizes throw `invalid_argument`.

### 6. Other shared code (not in src/internal)

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

### 7. Effect

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


## Building the library

## Windows Build

### (USING Cmake/Git bash)

**1. How to install Cmake**

- Open Command prompt on your pc and enter this code

```bash

winget install -e --id Kitware.CMake

``` 

**2. Building the library**

- Run Git bash as administrator and right click on the repository folder and select open git bash here
- Paste the code below in your git bash terminal

```bash

# ==========================================
# 1. SETUP SAFE ENVIRONMENT
# ==========================================
ORIGINAL_DIR=$(pwd)
SAFE_DIR="$HOME/Group23_Safe_Build"

echo "📦 Creating restriction-free build folder at: $SAFE_DIR"
rm -rf "$SAFE_DIR"
mkdir -p "$SAFE_DIR"
cp -r . "$SAFE_DIR"
cd "$SAFE_DIR"

# ==========================================
# 2. CHECK FILES & ADJUST FOR CMAKE
# ==========================================
LIB="src/adams_bashforth_solver.cpp \
src/adams_moulton_solver.cpp \
src/advanced_integrator.cpp \
src/advanced_ode_solver.cpp \
src/advanced_root_finder.cpp \
src/basic_integrator.cpp \
src/basic_ode_solver.cpp \
src/basic_root_finder.cpp \
src/conjugate_gradient_solver.cpp \
src/cubic_spline.cpp \
src/finite_difference_solver.cpp \
src/interpolator.cpp \
src/matrix_solver.cpp \
src/numerical_differentiator.cpp \
src/runge_kutta_fehlberg_solver.cpp"

echo ">>> Checking for missing source files..."
for file in $LIB; do
    if [ ! -f "$file" ]; then
        echo "⚠️ Warning: Source file '$file' is missing. Creating a dummy file to prevent CMake errors..."
        touch "$file"
    fi
done

# ==========================================
# 3. CMAKE BUILD
# ==========================================
mkdir -p build
cd build

echo ">>> Configuring CMake..."
cmake -G "MinGW Makefiles" ..

echo ">>> Compiling with CMake..."
cmake --build .

# ==========================================
# 4. FINISH & OPEN EXPLORER
# ==========================================
echo ">>> Opening build folder in Windows File Explorer..."
explorer .

cd "$ORIGINAL_DIR"
echo "✅ Finished! The library has been built inside $SAFE_DIR/build."

```

- It should open the folder where the build was created and you can run the numcomp_app.


## macOS/Linux build

### Using CMake and Homebrew

- If your macOS has no compiler, paste this code in the terminal to get it.

```bash

xcode-select --install

```

- If you also do not have CMake installed on your macOS, paste this code inside Homebrew.

```bash

brew install cmake

```

- Now to build the library and the app, paste this code in your new terminal in vs code while inside the repository folder.
- It should open the folder where the numcomp_app executable is located.

```bash

# ==========================================
# 1. SETUP SAFE ENVIRONMENT
# ==========================================
ORIGINAL_DIR=$(pwd)
SAFE_DIR="$HOME/Group23_Safe_Build"

echo "📦 Creating restriction-free build folder at: $SAFE_DIR"
rm -rf "$SAFE_DIR"
mkdir -p "$SAFE_DIR"
cp -r . "$SAFE_DIR"
cd "$SAFE_DIR"

# ==========================================
# 2. CHECK FILES & ADJUST FOR CMAKE
# ==========================================
LIB="src/adams_bashforth_solver.cpp \
src/adams_moulton_solver.cpp \
src/advanced_integrator.cpp \
src/advanced_ode_solver.cpp \
src/advanced_root_finder.cpp \
src/basic_integrator.cpp \
src/basic_ode_solver.cpp \
src/basic_root_finder.cpp \
src/conjugate_gradient_solver.cpp \
src/cubic_spline.cpp \
src/finite_difference_solver.cpp \
src/interpolator.cpp \
src/matrix_solver.cpp \
src/numerical_differentiator.cpp \
src/runge_kutta_fehlberg_solver.cpp"

echo ">>> Checking for missing source files..."
for file in $LIB; do
    if [ ! -f "$file" ]; then
        echo "⚠️ Warning: Source file '$file' is missing. Creating a dummy file to prevent CMake errors..."
        touch "$file"
    fi
done

# ==========================================
# 3. CMAKE BUILD
# ==========================================
mkdir -p build
cd build

echo ">>> Configuring CMake..."
cmake ..

echo ">>> Compiling with CMake..."
cmake --build .

# ==========================================
# 4. FINISH & OPEN FINDER
# ==========================================
echo ">>> Opening build folder in macOS Finder..."
open .

cd "$ORIGINAL_DIR"
echo "✅ Finished! The library has been built inside $SAFE_DIR/build."

```

## Install the Library (if your CMake supports it) 

- (Installing means copy the three things another program needs into one folder (the prefix): the public headers, the static library, and the app. A program elsewhere on the computer can then #include the headers and link the library, without the source code.)

##  NB (To install the library you must build it) 

### Windows Installation

**How to install Cmake**

- Open Command prompt on your pc and enter this code

```bash

winget install -e --id Kitware.CMake

``` 

- Download the repository and save it anywhere on your pc
- Right click on it and select open Git bash here and paste the code below. 
- It should open the folder where the library that can be used it located 

```bash

# ==========================================
# 1. SETUP SAFE ENVIRONMENT
# ==========================================
ORIGINAL_DIR=$(pwd)
SAFE_DIR="$HOME/Group23_Safe_Build"

echo "📦 Creating restriction-free build folder at: $SAFE_DIR"
rm -rf "$SAFE_DIR"
mkdir -p "$SAFE_DIR"
cp -r . "$SAFE_DIR"
cd "$SAFE_DIR"

# ==========================================
# 2. CHECK FILES & ADJUST FOR CMAKE
# ==========================================
LIB="src/adams_bashforth_solver.cpp \
src/adams_moulton_solver.cpp \
src/advanced_integrator.cpp \
src/advanced_ode_solver.cpp \
src/advanced_root_finder.cpp \
src/basic_integrator.cpp \
src/basic_ode_solver.cpp \
src/basic_root_finder.cpp \
src/conjugate_gradient_solver.cpp \
src/cubic_spline.cpp \
src/finite_difference_solver.cpp \
src/interpolator.cpp \
src/matrix_solver.cpp \
src/numerical_differentiator.cpp \
src/runge_kutta_fehlberg_solver.cpp"

echo ">>> Checking for missing source files..."
for file in $LIB; do
    if [ ! -f "$file" ]; then
        echo "⚠️ Warning: Source file '$file' is missing. Creating a dummy file to prevent CMake errors..."
        touch "$file"
    fi
done

# ==========================================
# 3. CMAKE BUILD & INSTALL
# ==========================================
mkdir -p build
cd build

echo ">>> Configuring CMake with local install directory..."
cmake -G "MinGW Makefiles" -DCMAKE_INSTALL_PREFIX="../install" ..

echo ">>> Compiling with CMake..."
cmake --build .

echo ">>> Installing the library..."
cmake --install .

# ==========================================
# 4. FINISH & OPEN EXPLORER
# ==========================================
echo ">>> Opening install folder in Windows File Explorer..."
cd ../install
explorer .

cd "$ORIGINAL_DIR"
echo "✅ Finished! The library has been installed locally inside $SAFE_DIR/install."

```


### Linux and macOS  installation

- Paste the code below in your terminal.
- It should open the library folder where you find the files you need to use our library in your program.

```bash

#!/bin/bash

# ==========================================
# 1. SETUP SAFE ENVIRONMENT
# ==========================================
ORIGINAL_DIR=$(pwd)
SAFE_DIR="$HOME/Group23_Safe_Build"

echo "📦 Creating restriction-free build folder at: $SAFE_DIR"
rm -rf "$SAFE_DIR"
mkdir -p "$SAFE_DIR"
cp -R . "$SAFE_DIR"
cd "$SAFE_DIR" || exit 1

# ==========================================
# 2. CHECK FILES & ADJUST FOR CMAKE
# ==========================================
LIB="src/adams_bashforth_solver.cpp \
src/adams_moulton_solver.cpp \
src/advanced_integrator.cpp \
src/advanced_ode_solver.cpp \
src/advanced_root_finder.cpp \
src/basic_integrator.cpp \
src/basic_ode_solver.cpp \
src/basic_root_finder.cpp \
src/conjugate_gradient_solver.cpp \
src/cubic_spline.cpp \
src/finite_difference_solver.cpp \
src/interpolator.cpp \
src/matrix_solver.cpp \
src/numerical_differentiator.cpp \
src/runge_kutta_fehlberg_solver.cpp"

echo ">>> Checking for missing source files..."
for file in $LIB; do
    if [ ! -f "$file" ]; then
        echo "⚠️ Warning: Source file '$file' is missing. Creating a dummy file to prevent CMake errors..."
        touch "$file"
    fi
done

# ==========================================
# 3. CMAKE BUILD & INSTALL
# ==========================================
mkdir -p build
cd build || exit 1

echo ">>> Configuring CMake with local install directory..."
cmake -G "Unix Makefiles" -DCMAKE_INSTALL_PREFIX="../install" ..

echo ">>> Compiling with CMake..."
cmake --build . -j$(sysctl -n hw.ncpu)

echo ">>> Installing the library..."
cmake --install .

# ==========================================
# 4. FINISH & OPEN FINDER
# ==========================================
echo ">>> Opening install folder in macOS Finder..."
cd ../install || exit 1
open .

cd "$ORIGINAL_DIR" || exit 1
echo "✅ Finished! The library has been installed locally inside $SAFE_DIR/install."

```

## Use the Library in Your Own Program

- **Headers**: include/numerical-computing/; one include for everything: #include "numerical-computing/numcomp.hpp".
- **Namespace numcomp**; link the library numcomp2 (target_link_libraries(... numcomp2) or -lnumcomp2).

- **Exceptions**: invalid_argument (bad parameter), out_of_range (point outside the data or interval), runtime_error (method failed: no convergence, singular or non-SPD matrix, non-finite result). Wrap calls in try/catch.

**A small example**

```cpp 

/**
 * @file my_program.cpp
 * @brief An independent test program for the numcomp2 library.
 */

#include <iostream>
#include <cmath>

// Include the headers from your installed library
#include "numerical-computing/basic_root_finder.hpp"
#include "numerical-computing/basic_integrator.hpp"
#include "numerical-computing/numerical_differentiator.hpp"

int main() 
{
    numcomp::BasicRootFinder finder;
    numcomp::BasicIntegrator integrator;
    numcomp::NumericalDifferentiator diff;

    // 1. Find sqrt(2) by solving x^2 - 2 = 0
    auto f1 = [](double x) { return x * x - 2.0; };
    double root = finder.bisection(f1, 0.0, 2.0, 1e-6, 100);

    // 2. Integrate sin(x) from 0 to pi (exact area is 2)
    const double pi = std::acos(-1.0);
    auto f2 = [](double x) { return std::sin(x); };
    double integral = integrator.trapezoidal_rule(f2, 0.0, pi, 100);

    // 3. Find the slope of sin(x) at x = 0 (exact slope is 1)
    double slope = diff.central_difference(f2, 0.0, 1e-4);

    // Print the results
    std::cout << "sqrt(2)  = " << root << "\n";
    std::cout << "integral = " << integral << "\n";
    std::cout << "slope    = " << slope << "\n";

    return 0;
}

```

- To tell your compiler where to find the library;

### With g++

**For macOS/Linux(or git bash)**

```bash

g++ -std=c++17 -I "$HOME/Group23_Safe_Build/install/include" my_program.cpp -L "$HOME/Group23_Safe_Build/install/lib" -lnumcomp2 -o my_program
./my_program

```

**Windows Command Prompt**

```bash

g++ -std=c++17 -I "%USERPROFILE%\Group23_Safe_Build\install\include" my_program.cpp -L "%USERPROFILE%\Group23_Safe_Build\install\lib" -lnumcomp2 -o my_program.exe
.\my_program.exe

```



##  Examples 

 > Find more about the examples for all modules** [here](../documentation/modules/examples.md)

## Basic root finder

**A. Standard Equations**

[Equation and values tested](<../documentation/examples results/results_basic_root_finder.md#equations-and-values-tested>)

[Error Handling & Edge Cases](<../documentation/examples results/results_basic_root_finder.md#error-handling--edge-cases>)

**B. Applied Problems**


[Equation and values tested](<../documentation/examples results/results_basic_root_finder.md#equations-and-values-tested-applied>)

## Advanced_root_finder

**A. Standard Root Finding**

[Secant Method Baseline](<../documentation/examples results/results_advanced_root_finder.md#secant-method-baseline>)

[Fixed-Point Iteration Baseline](<../documentation/examples results/results_advanced_root_finder.md#fixed-point-iteration-baseline>)

[Divergence and Algorithm Stability (Choice of $g$)](<../documentation/examples results/results_advanced_root_finder.md#divergence-and-algorithm-stability-choice-of->)

[Invalid Input Exceptions](<../documentation/examples results/results_advanced_root_finder.md#invalid-input-exceptions>)

**B. Applied Problems**

[Loan Interest Rate](<../documentation/examples results/results_advanced_root_finder.md#loan-interest-rate>)

[Babylonian Square Roots](<../documentation/examples results/results_advanced_root_finder.md#babylonian-square-roots>)

[Spherical Tank Water Depth](<../documentation/examples results/results_advanced_root_finder.md#spherical-tank-water-depth>)

## Basic integrator

**A. Standard Integrals**

[Trapezoidal and Midpoint Baselines](<../documentation/examples results/results_basic_integrator.md#trapezoidal-and-midpoint-baselines>)

[Second-Order Convergence](<../documentation/examples results/results_basic_integrator.md#second-order-convergence>)

[Reversed Limits](<../documentation/examples results/results_basic_integrator.md#reversed-limits>)

[Invalid Input Exceptions](<../documentation/examples results/results_basic_integrator.md#invalid-input-exceptions>)

**B. Applied Integrals**

[Distance from Velocity](<../documentation/examples results/results_basic_integrator.md#distance-from-velocity>)

[Normal Distribution Probability](<../documentation/examples results/results_basic_integrator.md#normal-distribution-probability>)

[Work Done by a Spring](<../documentation/examples results/results_basic_integrator.md#work-done-by-a-spring>)


## Advanced integrator

**A. Standard Integrals**

[Simpson's Rule Baseline Tests](<../documentation/examples results/results_advanced_integrator.md#simpsons-rule-baseline-tests>)

[Simpson's Rule Convergence](<../documentation/examples results/results_advanced_integrator.md#simpsons-rule-convergence>)

[Romberg Integration Accuracy](<../documentation/examples results/results_advanced_integrator.md#romberg-integration-accuracy>)

[Invalid Input Exceptions](<../documentation/examples results/results_advanced_integrator.md#invalid-input-exceptions>)

**B. Applied Integrals**

[Error Function $\text{erf}(1)$](<../documentation/examples results/results_advanced_integrator.md#error-function>)

[Arc Length of Parabola $y = x^2$](<../documentation/examples results/results_advanced_integrator.md#arc-length-of-parabola>)

[Algorithm Cost Comparison](<../documentation/examples results/results_advanced_integrator.md#algorithm-cost-comparison>)

## Basic_ode_solver

**A. Standard ODE Solving**

[Exponential Growth Baseline](<../documentation/examples results/results_basic_ode_solver.md#exponential-growth-baseline>)

[Order of Accuracy](<../documentation/examples results/results_basic_ode_solver.md#order-of-accuracy>)

[Additional Right-Hand Sides](<../documentation/examples results/results_basic_ode_solver.md#additional-right-hand-sides>)

[Invalid Input Exceptions](<../documentation/examples results/results_basic_ode_solver.md#invalid-input-exceptions>)

**B. Applied Models**

[Newton's Law of Cooling](<../documentation/examples results/results_basic_ode_solver.md#newtons-law-of-cooling>)

[Logistic Population Growth](<../documentation/examples results/results_basic_ode_solver.md#logistic-population-growth>)

## Advanced ode solver

**A. Standard Runge-Kutta Methods**

[Exponential Decay Baseline](<../documentation/examples results/results_advanced_ode_solver.md#exponential-decay-baseline>)

[Order of Accuracy Verification](<../documentation/examples results/results_advanced_ode_solver.md#order-of-accuracy-verification>)

[Alternative Right-Hand Sides](<../documentation/examples results/results_advanced_ode_solver.md#alternative-right-hand-sides>)

[Invalid Input Exceptions](<../documentation/examples results/results_advanced_ode_solver.md#invalid-input-exceptions>)

**B. Applied ODE Models**

[RC Circuit Charging](<../documentation/examples results/results_advanced_ode_solver.md#rc-circuit-charging>)

[Pharmacokinetics (Drug Infusion)](<../documentation/examples results/results_advanced_ode_solver.md#pharmacokinetics-drug-infusion>)



## Cubic_spline

**A. Standard Cubic Spline**

[Hand-Checkable Case](<../documentation/examples results/results_cubic_spline.md#hand-checkable-case>)

[Spline vs Linear Interpolation](<../documentation/examples results/results_cubic_spline.md#spline-vs-linear-interpolation>)

[Straight Line Reproduction](<../documentation/examples results/results_cubic_spline.md#straight-line-reproduction>)

[Invalid Input Exceptions](<../documentation/examples results/results_cubic_spline.md#invalid-input-exceptions>)

**B. Applied Cubic Spline** 

[Speedometer Data Smoothing](<../documentation/examples results/results_cubic_spline.md#speedometer-data-smoothing>)

[Convergence Study (Fourth-Order Accuracy)](<../documentation/examples results/results_cubic_spline.md#convergence-study-fourth-order-accuracy>)

## Interpolator

**A. Standard Interpolation**

[Linear Interpolation Baseline](<../documentation/examples results/results_interpolator.md#linear-interpolation-baseline>)

[Uneven Spacing and Negative Values](<../documentation/examples results/results_interpolator.md#uneven-spacing-and-negative-values>)

[Chebyshev Nodes Generation](<../documentation/examples results/results_interpolator.md#chebyshev-nodes-generation>)

[Invalid Input Exceptions](<../documentation/examples results/results_interpolator.md#invalid-input-exceptions>)


**B. Applied Interpolation** 

[Temperature Table Look-up](<../documentation/examples results/results_interpolator.md#temperature-table-look-up>)

[Node Distribution Error Comparison (Runge's Phenomenon)](<../documentation/examples results/results_interpolator.md#node-distribution-error-comparison-runges-phenomenon>)

## Matrix_solver

**A. Standard Linear Systems**

[3x3 System Baseline](<../documentation/examples results/results_matrix_solver.md#3x3-system-baseline>)

[Zero Pivot Handling (Automatic Row Swapping)](<../documentation/examples results/results_matrix_solver.md#zero-pivot-handling-automatic-row-swapping>)

[4x4 System Reversing](<../documentation/examples results/results_matrix_solver.md#4x4-system-reversing>)

[Invalid Input Exceptions](<../documentation/examples results/results_matrix_solver.md#invalid-input-exceptions>)

**B. Applied Linear Systems** 

[Kirchhoff's Voltage Law (3-Loop Circuit)](<../documentation/examples results/results_matrix_solver.md#kirchhoffs-voltage-law-3-loop-circuit>)

[Curve Fitting (Parabola through 3 points)](<../documentation/examples results/results_matrix_solver.md#curve-fitting-parabola-through-3-points>)

[Alloy Mixing Problem](<../documentation/examples results/results_matrix_solver.md#alloy-mixing-problem>)

## NumericalDifferentiator

**A. Standard Differentiation Formulas**

[First and Second Derivatives Baseline](<../documentation/examples results/results_numerical_differentiator.md#first-and-second-derivatives-baseline>)

[Order of Accuracy](<../documentation/examples results/results_numerical_differentiator.md#order-of-accuracy>)

[Rounding Error Limits](<../documentation/examples results/results_numerical_differentiator.md#rounding-error-limits>)

[Non-Differentiable Corners](<../documentation/examples results/results_numerical_differentiator.md#non-differentiable-corners>)

[Invalid Input Exceptions](<../documentation/examples results/results_numerical_differentiator.md#invalid-input-exceptions>)

**B. Applied Problems** 

[Kinematics (Velocity and Acceleration)](<../documentation/examples results/results_numerical_differentiator_documentation.md#kinematics-velocity-and-acceleration>)

[Economics (Marginal Cost)](<../documentation/examples results/results_numerical_differentiator_documentation.md#economics-marginal-cost>)

[Derivative-Free Newton-Raphson](<../documentation/examples results/results_numerical_differentiator_documentation.md#derivative-free-newton-raphson>)

## AdamsBashforthSolver

**A. Standard Implicit Multistep Methods**

[Exponential Decay Baseline](<../documentation/examples results/results_adams_bashforth_solver.md#exponential-decay-baseline>)

[Order of Accuracy](<../documentation/examples results/results_adams_bashforth_solver.md#order-of-accuracy>)

[Computational Cost Comparison (AB4 vs. RK4)](<../documentation/examples results/results_adams_bashforth_solver.md#computational-cost-comparison-ab4-vs-rk4>)

[Invalid Input Exceptions](<../documentation/examples results/results_adams_bashforth_solver.md#invalid-input-exceptions>)

**B. Applied Models** 

[Skydiver with Air Resistance](<../documentation/examples results/results_adams_bashforth_solver.md#skydiver-with-air-resistance>)

[Bacterial Growth (Logistic Model)](<../documentation/examples results/results_adams_bashforth_solver.md#bacterial-growth-logistic-model>)

## AdamsMoultonSolver

**A. Standard Implicit Multistep Methods**

[Exponential Decay Baseline](<vdocumentation/examples results/results_adams_moulton_solver.md#exponential-decay-baseline>)

[Implicit vs. Explicit Error Comparison](<../documentation/examples results/results_adams_moulton_solver.md#implicit-vs-explicit-error-comparison>)

[Fast-Decaying (Stiff) Problem Stability](<../documentation/examples results/results_adams_moulton_solver.md#fast-decaying-stiff-problem-stability>)

[Invalid Input Exceptions](<../documentation/examples results/results_adams_moulton_solver.md#invalid-input-exceptions>)

**B. Applied Models** 

[Second-Order Chemical Reaction](<../documentation/examples results/results_adams_moulton_solver.md#second-order-chemical-reaction>)

[Draining Tank (Torricelli's Law)](<../documentation/examples results/results_adams_moulton_solver.md#draining-tank-torricellis-law>)

## RungeKuttaFehlbergSolver

**A. Adaptive Step Size Control**

[Tolerance-Based Error Control](<../documentation/examples results/results_runge_kutta_fehlberg_solver.md#tolerance-based-error-control>)

[Dynamic Step Adjustment](<../documentation/examples results/results_runge_kutta_fehlberg_solver.md#dynamic-step-adjustment>)

[Direct Value Extraction (`rkf45_value`)](<../documentation/examples results/results_runge_kutta_fehlberg_solver.md#direct-value-extraction-rkf45_value>)

[Invalid Input Exceptions](<../documentation/examples results/results_runge_kutta_fehlberg_solver.md#invalid-input-exceptions>)

**B. Applied Models** 

[Skydiver with Rapidly Opening Parachute](<../documentation/examples results/results_runge_kutta_fehlberg_solver.md#skydiver-with-rapidly-opening-parachute>)

[Drug Elimination (Michaelis-Menten Kinetics)](<../documentation/examples results/results_runge_kutta_fehlberg_solver.md#drug-elimination-michaelis-menten-kinetics>)

## ConjugateGradientSolver

**A. Standard Iterative Solving**

[3x3 Symmetric Positive Definite (SPD) System Baseline](<../documentation/examples results/results_conjugate_gradient_solver.md#3x3-symmetric-positive-definite-spd-system-baseline>)

[Tridiagonal System Convergence Rate](<../documentation/examples results/results_conjugate_gradient_solver.md#tridiagonal-system-convergence-rate>)

[Comparison with Direct Gaussian Elimination](<../documentation/examples results/results_conjugate_gradient_solver.md>)

[Invalid Input Exceptions](<../documentation/examples results/results_conjugate_gradient_solver.md#invalid-input-exceptions>)

**B. Applied SPD Systemss**

[Spring Chain Displacements (Stiffness Matrix)](<../documentation/examples results/results_conjugate_gradient_solver.md#spring-chain-displacements-stiffness-matrix>)

[Heated Rod (Steady Temperature Distribution)](<../documentation/examples results/results_conjugate_gradient_solver.md#heated-rod-steady-temperature-distribution>)

[Line of Best Fit (Linear Regression)](<../documentation/examples results/results_conjugate_gradient_solver.md#line-of-best-fit-linear-regression>)

## FiniteDifferenceSolver

**A. Linear Two-Point Boundary Value Problems**

[Poisson Equation Baseline](<../documentation/examples results/results_finite_difference_solver.md#poisson-equation-baseline>)

[Order of Accuracy](<../documentation/examples results/results_finite_difference_solver.md#order-of-accuracy>)

[Variable Coefficients BVP](<../documentation/examples results/results_finite_difference_solver.md#variable-coefficients-bvp>)

[Invalid Input Exceptions](<../documentation/examples results/results_finite_difference_solver.md#invalid-input-exceptions>)

**B. Applied Boundary Value Problems** 

[Cooling Fin Heat Transfer](<../documentation/examples results/results_finite_difference_solver.md#cooling-fin-heat-transfer>)

[Hanging Cable Sag](<../documentation/examples results/results_finite_difference_solver.md#hanging-cable-sag>)


## Additional Documentation & Links

For an in-depth look at our testing framework, example results, and API documentation, please refer to the following guides:

* **API & Doxygen Documentation:** [Doxygen Comments Guide](../documentation/guides/doxygen_comments_guide.md)

* **Testing Framework Details:** [Test Framework Guide](../documentation/modules/test_framework.md)

* **Examples Documentation:** [Examples Guide](../documentation/modules/examples.md)

## AI clause for entire project

 The help from AI in each week was documented in each weekly report. See this at

- Week 1 help [AI clause](week-01.md#ai-use)
- Week 2 help [AI clause](week-02.md#ai-use)
- Week 3 help [AI clause](week-03.md#ai-use)
- Week 4 help [AI clause](week-04.md#ai-use)
- Final Week help [AI clause](<Final week report.md#ai-use>)