# 1. Numerical Computing 2 - C++ Library (Group 23)

The objective of this project is to build a reusable numerical computing library from scratch using C++. It provides a set of mathematical tools for finding roots, interpolating data, numerical integration, numerical differentiation, and solving ordinary differential equations (ODEs). We built this to demonstrate object-oriented programming concepts and collaborative software development.

## Table of Contents
- [2. Requirements and Dependencies](#2-requirements-and-dependencies)
- [3. Main Features](#3-main-features)
- [4. Project Structure](#4-project-structure)
- [5. Build the Library](#5-build-the-library)
- [6. Install the Library](#6-install-the-library-if-your-cmake-supports-it)
- [7. Use the Library in Your Own Program](#7-use-the-library-in-your-own-program)
- [8. Examples](#8-examples)
- [9. Run the Tests](#9-run-the-tests)
- [10. Limitations](#10-limitations)
- [11. Contributions by Group Members](#11-contributions-by-group-members)
- [12. How We Collaborated as a Team](#12-how-we-collaborated-as-a-team)

## 2. Requirements and Dependencies

- **Compiler:** needs C++17 support. 
- **CMake:** version 3.10 or newer. 
- **Git:** only to clone the repository. 
- **Outside libraries:** None, everything is written by us from scratch.

## 3. Main Features

- **Root Finding:** Find roots of equations using Bisection, Newton-Raphson, Secant, and Fixed-Point Iteration methods.
- **Interpolation & Approximation:** Estimate values between known data points using Linear Interpolation and Cubic Splines, plus Chebyshev Approximation for generating optimal nodes.
- **Numerical Integration:** Approximate definite integrals using the Trapezoidal Rule, Midpoint Rule, Simpson's Rule, and Romberg Integration.
- **Numerical Differentiation:** Approximate derivatives of given functions.
- **ODE Solvers:** Solve ordinary differential equations using Euler's Method, Runge-Kutta-Fehlberg, Adams-Bashforth, Adams-Moulton, and the Finite Difference Method.
- **Linear Algebra:** Solve systems of linear equations using the Conjugate Gradient method.

## 4. Project Structure

Use the following structure for your project:

```text
Numerical-Computing-Group-23/
│
├── README.md
├── CMakeLists.txt
├── LICENSE
├── .gitignore
│
├── include/
│   └── numerical-computing/
│       ├── adv_integration.hpp
│       ├── advanced_root_finder.hpp
│       ├── advanced_ode_solver.hpp
│       ├── basic_integrator.hpp
│       ├── basic_ode.hpp
│       ├── interpolator.hpp
│       └── basic_root_finder.hpp
│
├── src/
│   ├── adv_integration.cpp
│   ├── advanced_root_finder.cpp
│   ├── advanced_ode_solver.cpp
│   ├── basic_integrator.cpp
│   ├── basic_ode_solver.cpp
│   ├── interpolator.cpp
│   ├── main.cpp
│   └── basic_root_finder.cpp
│
├── tests/
│   ├── CMakeLists.txt
│   ├── run_all_tests.sh
│   ├── test_advanced_integration.cpp
│   ├── test_advanced_ode.cpp (to be added in week 4)
│   ├── test_advanced_root_finder.cpp
│   ├── test_basic_integrator.cpp
│   ├── test_basic_interpolator.cpp
│   ├── test_basic_ode_solver.cpp (to be added in week 4)
│   └── test_root_finding.cpp (to be added in week 4)
│
├── examples/ (to be added in week 4)
│   ├── example_advanced_integration.cpp
│   ├── example_advanced_ode_solver.cpp
│   ├── example_basic_integration.cpp
│   ├── example_basic_ode_solver.cpp
│   ├── example_interpolation.cpp
│   ├── example_matrix.cpp
│   └── example_roots.cpp
│
└── reports/
    ├── week-01.md
    ├── week-02.md
    ├── week-03.md
    ├── week-04.md
    └── final-report.md
```

## 5. Build the Library

### Linux and macOS
*(To be filled in Week 3/4 once our CMake setup is tested across OS platforms)*

### Windows
*(To be filled in Week 3/4 - currently compiling with MinGW-w64 via VS Code)*

## 6. Install the Library (if your CMake supports it)

### Linux and macOS
*(Placeholder - will be added later)*

### Windows
*(Placeholder - will be added later)*

## 7. Use the Library in Your Own Program
*(Placeholder - we will add a quick start guide here showing how to link the compiled library once it is fully built)*

## 8. Examples
*(Placeholder - in Week 4, we will add instructions here on how to run the demo files from the `examples/` folder)*

## 9. Run the Tests
*(Placeholder - instructions for running the unit tests will be added here once the `tests/` directory is populated in Week 3)*

## 10. Limitations
*(Placeholder - we will document any edge cases or math limitations we find during testing)*

## 11. Contributions by Group Members

| Name | Student Number | Project Role / Assigned Module |
| :--- | :--- | :--- |
| CHEMISTO JAIRUS | 25/U/07928/PS | Member 1 (Lead) / Basic Root Finding |
| BATTE DERRICK CALVIN | 25/U/0717 | Member 2 / Interpolation (Linear, Chebyshev, Spline) |
| Aogon Sharon | 25/U/08690/PS | Member 3 / Basic Numerical Integration |
| BAKALU VALERIA | 25/U/07924/PS | Member 4 / Advanced Root Finding |
| WASSWA GEORGE MPANGA | 25/U/08793/PS | Member 5 / Advanced Numerical Integration |
| MWANJE IAN | 25/U/08757/PS | Member 6 / Basic ODE Solvers |
| MUWANGUZI SAMUEL | 25/U/0843 | Member 7 / Advanced ODE Solvers |
| KYEYUNE PRAISE PAULINE | 25/U/07943/PS | Member 8 / Linear Algebra & Unit Testing |
| Nabakooza Bridget Maria | 25/U/07592/PS | Member 9 / Demo Examples & Final Documentation |

## 12. How We Collaborated as a Team

Because we have a large group of 9 people, we had to be very organized so we didn't overwrite each other's code. Here is how we divided the work and collaborated:

- **Division of Work:** We broke the project into logical modules and divided the workload across all 9 group members. We assigned 7 members to write the core C++ math modules, 1 member to build the unit testing framework, and 1 member to handle the final examples and documentation.
- **Version Control Strategy:** We used Git and GitHub. Our Project Lead (Chemisto Jairus) set up the initial repository, the empty folder structure (`include`, `src`, `tests`, `examples`, `reports`), and the basic `CMakeLists.txt` on the `main` branch. 
- **Branching and Pull Requests:** The rest of the team cloned the repository to their local machines. Nobody is allowed to push code directly to `main`. Instead, everyone creates a new branch for their specific assigned module (for example, `feature-interp` for interpolation). Once their C++ code works locally, they push their branch to GitHub and open a Pull Request (PR).
- **Code Review:** Before a PR is merged into the main branch, at least one other group member reviews the code on GitHub, leaves a comment confirming it looks correct, and then hits the merge button. This ensures everyone understands how the whole library is coming together, not just their own part.