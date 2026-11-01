# General Report - Numerical Computing 2 (Group 23)

## 1. Group Members and Contributions

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

## 2. Requirements and Dependencies

- **Compiler:** needs C++17 support. 
- **CMake:** version 3.10 or newer. 
- **Git:** only to clone the repository. 
- **Outside libraries:** None, everything is written by us from scratch.

## 3. The Library at a Glance

| Class | Methods | Module guide | 
| ----- | ----- | ----- | 
| `BasicRootFinder` | Bisection, Newton-Raphson | [modules/basic_root_finder.md](documentation/modules/basic_root_finder.md) | 
| `AdvancedRootFinder` | Secant, Fixed-point iteration | [modules/advanced_root_finder.md](documentation/modules/advanced_root_finder.md) | 
| `Interpolator` | Linear interpolation, Chebyshev nodes, Chebyshev approximation | [modules/interpolator.md](documentation/modules/interpolator.md) | 
| `CubicSpline` | Natural cubic spline | [modules/cubic_spline.md](documentation/modules/cubic_spline.md) | 
| `NumericalDifferentiator` | Forward, backward, central differences, second derivative, Richardson | [modules/numerical_differentiator.md](documentation/modules/numerical_differentiator.md) | 
| `BasicIntegrator` | Trapezoidal, Midpoint | [modules/basic_integrator.md](documentation/modules/basic_integrator.md) | 
| `AdvancedIntegrator` | Simpson's 1/3, Romberg | [modules/advanced_integrator.md](documentation/modules/advanced_integrator.md) | 
| `BasicODESolver` | Euler, Heun | [modules/basic_ode_solver.md](documentation/modules/basic_ode_solver.md) | 
| `AdvancedODESolver` | RK3, RK4 | [modules/advanced_ode_solver.md](documentation/modules/advanced_ode_solver.md) | 
| `AdamsBashforthSolver` | Adams-Bashforth 2 and 4 | [modules/adams_bashforth_solver.md](documentation/modules/adams_bashforth_solver.md) | 
| `AdamsMoultonSolver` | Adams-Moulton 2 and 4 | [modules/adams_moulton_solver.md](documentation/modules/adams_moulton_solver.md) | 
| `RungeKuttaFehlbergSolver` | Adaptive RKF45 | [modules/runge_kutta_fehlberg_solver.md](documentation/modules/runge_kutta_fehlberg_solver.md) | 
| `FiniteDifferenceSolver` | Finite difference method for boundary value problems | [modules/finite_difference_solver.md](documentation/modules/finite_difference_solver.md) | 
| `MatrixSolver` | Gaussian elimination, LU decomposition | [modules/matrix_solver.md](documentation/modules/matrix_solver.md) | 
| `ConjugateGradientSolver` | Conjugate gradient | [modules/conjugate_gradient_solver.md](documentation/modules/conjugate_gradient_solver.md) | 

## 4. Project Structure

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

## 5. Building and Installation

### Windows Build & Install

**Building the Library:**

1. Install CMake using the command `winget install -e --id Kitware.CMake` in the Command prompt.

2. Run Git bash as administrator, right-click the repository folder, and select "open git bash here".

3. Paste the provided setup script into the terminal to create a safe environment, adjust files, configure CMake with `cmake -G "MinGW Makefiles" ..`, compile with `cmake --build .`, and open the build folder in Windows File Explorer.

**Installing the Library:**

1. Install CMake via Command prompt using `winget install -e --id Kitware.CMake`.

2. Download the repository, right-click it, and open Git bash.

3. Run the installation script to create a safe environment, configure CMake with `cmake -G "MinGW Makefiles" -DCMAKE_INSTALL_PREFIX="../install" ..`, compile with `cmake --build .`, install with `cmake --install .`, and open the install folder in Windows File Explorer.

### macOS / Linux Build & Install

**Building the Library:**

1. Install the compiler via `xcode-select --install` in the terminal.

2. Install CMake using `brew install cmake` via Homebrew.

3. Run the provided setup script in the VS Code terminal inside the repository folder to create a safe environment, configure CMake with `cmake ..`, compile with `cmake --build .`, and open the build folder in Finder.

**Installing the Library:**

1. Paste the installation script into the terminal to create a safe environment, configure CMake with `cmake -G "Unix Makefiles" -DCMAKE_INSTALL_PREFIX="../install" ..`, compile with `cmake --build . -j$(sysctl -n hw.ncpu)`, install with `cmake --install .`, and open the install folder in Finder.

## 6. Testing

### Running Tests Individually

To run tests individually, navigate to the `tests` folder inside the `build` folder, right-click, open in the terminal (Windows PowerShell), and run `.\test_(module name).exe` (e.g., `.\test_matrix_solver.exe`).

### Running All Tests and Examples (Windows)

1. Install CMake using `winget install -e --id Kitware.CMake`.

2. Run Git bash as administrator in the repository folder.

3. Execute the testing script to configure CMake with `cmake -G "MinGW Makefiles" ..`, compile with `cmake --build .`, and run the test suite with `ctest --output-on-failure`.

### Running All Tests and Examples (macOS)

Execute the testing script to configure CMake with `cmake -G "Unix Makefiles" ..`, compile across multiple cores with `cmake --build . -j$(sysctl -n hw.ncpu)`, and run the test suite with `ctest --output-on-failure`.

##  Examples 

**Find more about the examples for all modules** [here](documentation/modules/examples.md)

## Basic root finder

**A. Standard Equations**

[Equation and values tested](<documentation/examples results/results_basic_root_finder.md#equations-and-values-tested>)

[Error Handling & Edge Cases](<documentation/examples results/results_basic_root_finder.md#error-handling--edge-cases>)

**B. Applied Problems**


[Equation and values tested](<documentation/examples results/results_basic_root_finder.md#equations-and-values-tested-applied>)

## Advanced_root_finder

**A. Standard Root Finding**

[Secant Method Baseline](<documentation/examples results/results_advanced_root_finder.md#secant-method-baseline>)

[Fixed-Point Iteration Baseline](<documentation/examples results/results_advanced_root_finder.md#fixed-point-iteration-baseline>)

[Divergence and Algorithm Stability (Choice of $g$)](<documentation/examples results/results_advanced_root_finder.md#divergence-and-algorithm-stability-choice-of->)

[Invalid Input Exceptions](<documentation/examples results/results_advanced_root_finder.md#invalid-input-exceptions>)

**B. Applied Problems**

[Loan Interest Rate](<documentation/examples results/results_advanced_root_finder.md#loan-interest-rate>)

[Babylonian Square Roots](<documentation/examples results/results_advanced_root_finder.md#babylonian-square-roots>)

[Spherical Tank Water Depth](<documentation/examples results/results_advanced_root_finder.md#spherical-tank-water-depth>)

## Basic integrator

**A. Standard Integrals**

[Trapezoidal and Midpoint Baselines](<documentation/examples results/results_basic_integrator.md#trapezoidal-and-midpoint-baselines>)

[Second-Order Convergence](<documentation/examples results/results_basic_integrator.md#second-order-convergence>)

[Reversed Limits](<documentation/examples results/results_basic_integrator.md#reversed-limits>)

[Invalid Input Exceptions](<documentation/examples results/results_basic_integrator.md#invalid-input-exceptions>)

**B. Applied Integrals**

[Distance from Velocity](<documentation/examples results/results_basic_integrator.md#distance-from-velocity>)

[Normal Distribution Probability](<documentation/examples results/results_basic_integrator.md#normal-distribution-probability>)

[Work Done by a Spring](<documentation/examples results/results_basic_integrator.md#work-done-by-a-spring>)


## Advanced integrator

**A. Standard Integrals**

[Simpson's Rule Baseline Tests](<documentation/examples results/results_advanced_integrator.md#simpsons-rule-baseline-tests>)

[Simpson's Rule Convergence](<documentation/examples results/results_advanced_integrator.md#simpsons-rule-convergence>)

[Romberg Integration Accuracy](<documentation/examples results/results_advanced_integrator.md#romberg-integration-accuracy>)

[Invalid Input Exceptions](<documentation/examples results/results_advanced_integrator.md#invalid-input-exceptions>)

**B. Applied Integrals**

[Error Function $\text{erf}(1)$](<documentation/examples results/results_advanced_integrator.md#error-function>)

[Arc Length of Parabola $y = x^2$](<documentation/examples results/results_advanced_integrator.md#arc-length-of-parabola>)

[Algorithm Cost Comparison](<documentation/examples results/results_advanced_integrator.md#algorithm-cost-comparison>)

## Basic_ode_solver

**A. Standard ODE Solving**

[Exponential Growth Baseline](<documentation/examples results/results_basic_ode_solver.md#exponential-growth-baseline>)

[Order of Accuracy](<documentation/examples results/results_basic_ode_solver.md#order-of-accuracy>)

[Additional Right-Hand Sides](<documentation/examples results/results_basic_ode_solver.md#additional-right-hand-sides>)

[Invalid Input Exceptions](<documentation/examples results/results_basic_ode_solver.md#invalid-input-exceptions>)

**B. Applied Models**

[Newton's Law of Cooling](<documentation/examples results/results_basic_ode_solver.md#newtons-law-of-cooling>)

[Logistic Population Growth](<documentation/examples results/results_basic_ode_solver.md#logistic-population-growth>)

## Advanced ode solver

**A. Standard Runge-Kutta Methods**

[Exponential Decay Baseline](<documentation/examples results/results_advanced_ode_solver.md#exponential-decay-baseline>)

[Order of Accuracy Verification](<documentation/examples results/results_advanced_ode_solver.md#order-of-accuracy-verification>)

[Alternative Right-Hand Sides](<documentation/examples results/results_advanced_ode_solver.md#alternative-right-hand-sides>)

[Invalid Input Exceptions](<documentation/examples results/results_advanced_ode_solver.md#invalid-input-exceptions>)

**B. Applied ODE Models**

[RC Circuit Charging](<documentation/examples results/results_advanced_ode_solver.md#rc-circuit-charging>)

[Pharmacokinetics (Drug Infusion)](<documentation/examples results/results_advanced_ode_solver.md#pharmacokinetics-drug-infusion>)



## Cubic_spline

**A. Standard Cubic Spline**

[Hand-Checkable Case](<documentation/examples results/results_cubic_spline.md#hand-checkable-case>)

[Spline vs Linear Interpolation](<documentation/examples results/results_cubic_spline.md#spline-vs-linear-interpolation>)

[Straight Line Reproduction](<documentation/examples results/results_cubic_spline.md#straight-line-reproduction>)

[Invalid Input Exceptions](<documentation/examples results/results_cubic_spline.md#invalid-input-exceptions>)

**B. Applied Cubic Spline** 

[Speedometer Data Smoothing](<documentation/examples results/results_cubic_spline.md#speedometer-data-smoothing>)

[Convergence Study (Fourth-Order Accuracy)](<documentation/examples results/results_cubic_spline.md#convergence-study-fourth-order-accuracy>)

## Interpolator

**A. Standard Interpolation**

[Linear Interpolation Baseline](<documentation/examples results/results_interpolator.md#linear-interpolation-baseline>)

[Uneven Spacing and Negative Values](<documentation/examples results/results_interpolator.md#uneven-spacing-and-negative-values>)

[Chebyshev Nodes Generation](<documentation/examples results/results_interpolator.md#chebyshev-nodes-generation>)

[Invalid Input Exceptions](<documentation/examples results/results_interpolator.md#invalid-input-exceptions>)


**B. Applied Interpolation** 

[Temperature Table Look-up](<documentation/examples results/results_interpolator.md#temperature-table-look-up>)

[Node Distribution Error Comparison (Runge's Phenomenon)](<documentation/examples results/results_interpolator.md#node-distribution-error-comparison-runges-phenomenon>)

## Matrix_solver

**A. Standard Linear Systems**

[3x3 System Baseline](<documentation/examples results/results_matrix_solver.md#3x3-system-baseline>)

[Zero Pivot Handling (Automatic Row Swapping)](<documentation/examples results/results_matrix_solver.md#zero-pivot-handling-automatic-row-swapping>)

[4x4 System Reversing](<documentation/examples results/results_matrix_solver.md#4x4-system-reversing>)

[Invalid Input Exceptions](<documentation/examples results/results_matrix_solver.md#invalid-input-exceptions>)

**B. Applied Linear Systems** 

[Kirchhoff's Voltage Law (3-Loop Circuit)](<documentation/examples results/results_matrix_solver.md#kirchhoffs-voltage-law-3-loop-circuit>)

[Curve Fitting (Parabola through 3 points)](<documentation/examples results/results_matrix_solver.md#curve-fitting-parabola-through-3-points>)

[Alloy Mixing Problem](<documentation/examples results/results_matrix_solver.md#alloy-mixing-problem>)

## NumericalDifferentiator

**A. Standard Differentiation Formulas**

[First and Second Derivatives Baseline](<documentation/examples results/results_numerical_differentiator.md#first-and-second-derivatives-baseline>)

[Order of Accuracy](<documentation/examples results/results_numerical_differentiator.md#order-of-accuracy>)

[Rounding Error Limits](<documentation/examples results/results_numerical_differentiator.md#rounding-error-limits>)

[Non-Differentiable Corners](<documentation/examples results/results_numerical_differentiator.md#non-differentiable-corners>)

[Invalid Input Exceptions](<documentation/examples results/results_numerical_differentiator.md#invalid-input-exceptions>)

**B. Applied Problems** 

[Kinematics (Velocity and Acceleration)](<documentation/examples results/results_numerical_differentiator_documentation.md#kinematics-velocity-and-acceleration>)

[Economics (Marginal Cost)](<documentation/examples results/results_numerical_differentiator_documentation.md#economics-marginal-cost>)

[Derivative-Free Newton-Raphson](<documentation/examples results/results_numerical_differentiator_documentation.md#derivative-free-newton-raphson>)

## AdamsBashforthSolver

**A. Standard Implicit Multistep Methods**

[Exponential Decay Baseline](<documentation/examples results/results_adams_bashforth_solver.md#exponential-decay-baseline>)

[Order of Accuracy](<documentation/examples results/results_adams_bashforth_solver.md#order-of-accuracy>)

[Computational Cost Comparison (AB4 vs. RK4)](<documentation/examples results/results_adams_bashforth_solver.md#computational-cost-comparison-ab4-vs-rk4>)

[Invalid Input Exceptions](<documentation/examples results/results_adams_bashforth_solver.md#invalid-input-exceptions>)

**B. Applied Models** 

[Skydiver with Air Resistance](<documentation/examples results/results_adams_bashforth_solver.md#skydiver-with-air-resistance>)

[Bacterial Growth (Logistic Model)](<documentation/examples results/results_adams_bashforth_solver.md#bacterial-growth-logistic-model>)

## AdamsMoultonSolver

**A. Standard Implicit Multistep Methods**

[Exponential Decay Baseline](<documentation/examples results/results_adams_moulton_solver.md#exponential-decay-baseline>)

[Implicit vs. Explicit Error Comparison](<documentation/examples results/results_adams_moulton_solver.md#implicit-vs-explicit-error-comparison>)

[Fast-Decaying (Stiff) Problem Stability](<documentation/examples results/results_adams_moulton_solver.md#fast-decaying-stiff-problem-stability>)

[Invalid Input Exceptions](<documentation/examples results/results_adams_moulton_solver.md#invalid-input-exceptions>)

**B. Applied Models** 

[Second-Order Chemical Reaction](<documentation/examples results/results_adams_moulton_solver.md#second-order-chemical-reaction>)

[Draining Tank (Torricelli's Law)](<documentation/examples results/results_adams_moulton_solver.md#draining-tank-torricellis-law>)

## RungeKuttaFehlbergSolver

**A. Adaptive Step Size Control**

[Tolerance-Based Error Control](<documentation/examples results/results_runge_kutta_fehlberg_solver.md#tolerance-based-error-control>)

[Dynamic Step Adjustment](<documentation/examples results/results_runge_kutta_fehlberg_solver.md#dynamic-step-adjustment>)

[Direct Value Extraction (`rkf45_value`)](<documentation/examples results/results_runge_kutta_fehlberg_solver.md#direct-value-extraction-rkf45_value>)

[Invalid Input Exceptions](<documentation/examples results/results_runge_kutta_fehlberg_solver.md#invalid-input-exceptions>)

**B. Applied Models** 

[Skydiver with Rapidly Opening Parachute](<documentation/examples results/results_runge_kutta_fehlberg_solver.md#skydiver-with-rapidly-opening-parachute>)

[Drug Elimination (Michaelis-Menten Kinetics)](<documentation/examples results/results_runge_kutta_fehlberg_solver.md#drug-elimination-michaelis-menten-kinetics>)

## ConjugateGradientSolver

**A. Standard Iterative Solving**

[3x3 Symmetric Positive Definite (SPD) System Baseline](<documentation/examples results/results_conjugate_gradient_solver.md#3x3-symmetric-positive-definite-spd-system-baseline>)

[Tridiagonal System Convergence Rate](<documentation/examples results/results_conjugate_gradient_solver.md#tridiagonal-system-convergence-rate>)

[Comparison with Direct Gaussian Elimination](<documentation/examples results/results_conjugate_gradient_solver.md>)

[Invalid Input Exceptions](<documentation/examples results/results_conjugate_gradient_solver.md#invalid-input-exceptions>)

**B. Applied SPD Systemss**

[Spring Chain Displacements (Stiffness Matrix)](<documentation/examples results/results_conjugate_gradient_solver.md#spring-chain-displacements-stiffness-matrix>)

[Heated Rod (Steady Temperature Distribution)](<documentation/examples results/results_conjugate_gradient_solver.md#heated-rod-steady-temperature-distribution>)

[Line of Best Fit (Linear Regression)](<documentation/examples results/results_conjugate_gradient_solver.md#line-of-best-fit-linear-regression>)

## FiniteDifferenceSolver

**A. Linear Two-Point Boundary Value Problems**

[Poisson Equation Baseline](<documentation/examples results/results_finite_difference_solver.md#poisson-equation-baseline>)

[Order of Accuracy](<documentation/examples results/results_finite_difference_solver.md#order-of-accuracy>)

[Variable Coefficients BVP](<documentation/examples results/results_finite_difference_solver.md#variable-coefficients-bvp>)

[Invalid Input Exceptions](<documentation/examples results/results_finite_difference_solver.md#invalid-input-exceptions>)

**B. Applied Boundary Value Problems** 

[Cooling Fin Heat Transfer](<documentation/examples results/results_finite_difference_solver.md#cooling-fin-heat-transfer>)

[Hanging Cable Sag](<documentation/examples results/results_finite_difference_solver.md#hanging-cable-sag>)

## Additional Documentation & Links

For an in-depth look at our testing framework, example results, and API documentation, please refer to the following guides:

* **API & Doxygen Documentation:** [Doxygen Comments Guide](documentation/guides/doxygen_comments_guide.md)

* **Testing Framework Details:** [Test Framework Guide](documentation/modules/test_framework.md)

* **Examples Documentation:** [Examples Guide](documentation/modules/examples.md)

## Algorithm Implementation Summary - Group 23

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