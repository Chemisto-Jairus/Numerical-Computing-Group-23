#pragma once
/**
 * @file module_menus.hpp
 * @brief One interactive menu per topic of the library.
 * @author Nabakooza Bridget Maria (Member 9)
 *
 * Every menu follows the same flow:
 *  1. choose a method (or "compare all methods");
 *  2. choose a preset problem from the catalog, or type your own;
 *  3. confirm or change the suggested parameters (Enter keeps the default);
 *  4. read the result next to the exact answer and its error.
 * Each calculation runs inside a handler that reports invalid input or a
 * failed method and returns to the menu, so the app never stops on an error.
 */

namespace app
{
    /// Root finding: Bisection, Newton-Raphson, Secant, Fixed-Point Iteration.
    void root_finding_menu();

    /// Interpolation and approximation: linear interpolation, natural cubic spline,
    /// Chebyshev nodes and Chebyshev approximation.
    void interpolation_menu();

    /// Numerical differentiation: forward, backward and central differences,
    /// the second derivative and Richardson extrapolation.
    void differentiation_menu();

    /// Numerical integration: Trapezoidal, Midpoint, Simpson, Romberg.
    void integration_menu();

    /// Initial value problems, grouped by family: one-step (Euler, Heun, RK3, RK4),
    /// multistep (Adams-Bashforth 2 and 4, Adams-Moulton 2 and 4) and adaptive (RKF45).
    void ode_menu();

    /// Boundary value problems: the finite difference method.
    void bvp_menu();

    /// Linear systems: Gaussian elimination, LU decomposition and conjugate gradient.
    void linear_systems_menu();

    /// Prints a summary of the library's classes, error handling and limits.
    void about_screen();
}
