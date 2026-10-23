// Group 23: Numerical Computing command-line app
// Owners: Nabakooza Bridget Maria (Member 9) with Chemisto Jairus (Member 1)
//
// Every module of the library is reachable from the menu.
// The user types the inputs. Every number is checked before it is used,
// the input limits are shown, and every library call is inside try/catch.

#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <cstdlib>
#include <stdexcept>
#include <limits>
#include "numerical-computing/interpolator.hpp"
#include "numerical-computing/cubic_spline.hpp"
#include "numerical-computing/basic_root_finder.hpp"
#include "numerical-computing/advanced_root_finder.hpp"
#include "numerical-computing/basic_integrator.hpp"
#include "numerical-computing/advanced_integrator.hpp"
#include "numerical-computing/basic_ode_solver.hpp"
#include "numerical-computing/advanced_ode_solver.hpp"
#include "numerical-computing/matrix_solver.hpp"

// Sample functions used by the demos
double sample_equation(double x) { return x * x; }              // f(x) = x^2
double root_equation(double x) { return (x * x) - 4.0; }        // f(x) = x^2 - 4, roots at -2 and 2
double root_derivative(double x) { return 2.0 * x; }            // f'(x) = 2x
double fixed_point_g(double x) { return std::sqrt(x + 2.0); }   // g(x) = sqrt(x + 2), fixed point 2
double sample_ode(double t, double y) { return t * y; }         // dy/dt = t * y, exact y = e^(t^2 / 2)

// Reads one number. Keeps asking until the user types a valid number.
// If the input ends (Ctrl+Z / Ctrl+D) the program closes cleanly.
double read_double(const std::string& prompt) {
    double value;
    while (true) {
        std::cout << prompt;
        if (std::cin >> value) {
            return value;
        }
        if (std::cin.eof()) {
            std::cout << "\nInput ended. Goodbye!\n";
            std::exit(0);
        }
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "[!] Please enter a number.\n";
    }
}

// Reads one whole number. Keeps asking until the user types a valid integer.
int read_int(const std::string& prompt) {
    int value;
    while (true) {
        std::cout << prompt;
        if (std::cin >> value) {
            return value;
        }
        if (std::cin.eof()) {
            std::cout << "\nInput ended. Goodbye!\n";
            std::exit(0);
        }
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "[!] Please enter a whole number.\n";
    }
}

void display_menu() {
    std::cout << "\n--- GROUP 23: NUMERICAL COMPUTING ---\n";
    std::cout << " Interpolation  (data: y = x^2 at x = 0, 1, 2, 3, 4)\n";
    std::cout << "  1. Linear interpolation\n";
    std::cout << "  2. Chebyshev nodes\n";
    std::cout << "  3. Cubic spline\n";
    std::cout << " Root finding  (f(x) = x^2 - 4)\n";
    std::cout << "  4. Bisection\n";
    std::cout << "  5. Newton-Raphson\n";
    std::cout << "  6. Secant method\n";
    std::cout << "  7. Fixed-point iteration (g(x) = sqrt(x + 2))\n";
    std::cout << " Integration  (f(x) = x^2)\n";
    std::cout << "  8. Trapezoidal rule\n";
    std::cout << "  9. Midpoint rule\n";
    std::cout << " 10. Simpson's 1/3 rule\n";
    std::cout << " 11. Romberg integration\n";
    std::cout << " ODE solvers  (dy/dt = t * y, y(0) = 1)\n";
    std::cout << " 12. Euler\n";
    std::cout << " 13. Heun\n";
    std::cout << " 14. Runge-Kutta 3\n";
    std::cout << " 15. Runge-Kutta 4\n";
    std::cout << " Linear systems  (3 x 3)\n";
    std::cout << " 16. Gaussian elimination\n";
    std::cout << " 17. LU decomposition\n";
    std::cout << "  0. Exit\n";
    std::cout << "Enter your choice: ";
}

int main() {
    // Data used by the interpolation options
    std::vector<double> data_x = {0.0, 1.0, 2.0, 3.0, 4.0};
    std::vector<double> data_y = {0.0, 1.0, 4.0, 9.0, 16.0};

    // Example 3 x 3 system with answer x = 2, y = 3, z = -1
    std::vector<std::vector<double>> example_A = {{2.0, 1.0, -1.0}, {-3.0, -1.0, 2.0}, {-2.0, 1.0, 2.0}};
    std::vector<double> example_b = {8.0, -11.0, -3.0};

    while (true) {
        display_menu();

        int choice;
        // If the input is not a number, clear the error and ask again
        if (!(std::cin >> choice)) {
            if (std::cin.eof()) break;   // the input has ended, so stop
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "\n[!] Error: Please enter a valid number.\n";
            continue;
        }

        if (choice == 0) {
            std::cout << "Exiting application. Goodbye!\n";
            break;
        }

        // Every library call is inside try/catch, so bad input shows a message instead of crashing
        try {
            switch (choice) {
                case 1: {
                    double x = read_double("x to estimate (0 to 4): ");
                    numcomp::Interpolator interp;
                    // Work out the answer first: if it throws, no half-printed line is left
                    double result = interp.linear_interpolate(x, data_x, data_y);
                    std::cout << "\nLinear interpolation at x = " << x << ": " << result << "\n";
                    std::cout << "Exact x^2: " << x * x << "\n";
                    break;
                }
                case 2: {
                    double a = read_double("Left end a: ");
                    double b = read_double("Right end b (bigger than a): ");
                    int n = read_int("Number of nodes n (1 to 1,000,000): ");
                    numcomp::Interpolator interp;
                    std::vector<double> nodes = interp.chebyshev_nodes(a, b, n);
                    std::cout << "\nChebyshev nodes (first 10 shown):\n";
                    for (size_t i = 0; i < nodes.size() && i < 10; ++i) {
                        std::cout << "  x" << i + 1 << " = " << nodes[i] << "\n";
                    }
                    break;
                }
                case 3: {
                    double x = read_double("x to estimate (0 to 4): ");
                    numcomp::CubicSpline spline;
                    // Work out the answer first: if it throws, no half-printed line is left
                    double result = spline.interpolate(x, data_x, data_y);
                    std::cout << "\nCubic spline at x = " << x << ": " << result << "\n";
                    std::cout << "Exact x^2: " << x * x << "\n";
                    break;
                }
                case 4: {
                    double a = read_double("Left end a: ");
                    double b = read_double("Right end b: ");
                    double tol = read_double("Tolerance (for example 1e-6, must be > 0): ");
                    int max_iter = read_int("Maximum iterations (> 0): ");
                    numcomp::BasicRootFinder finder;
                    // Work out the answer first: if it throws, no half-printed line is left
                    double result = finder.bisection(root_equation, a, b, tol, max_iter);
                    std::cout << "\nBisection root: " << result << "\n";
                    break;
                }
                case 5: {
                    double x0 = read_double("Starting guess x0: ");
                    double tol = read_double("Tolerance (for example 1e-6, must be > 0): ");
                    int max_iter = read_int("Maximum iterations (> 0): ");
                    numcomp::BasicRootFinder finder;
                    // Work out the answer first: if it throws, no half-printed line is left
                    double result = finder.newton_raphson(root_equation, root_derivative, x0, tol, max_iter);
                    std::cout << "\nNewton-Raphson root: " << result << "\n";
                    break;
                }
                case 6: {
                    double x0 = read_double("First guess x0: ");
                    double x1 = read_double("Second guess x1: ");
                    double tol = read_double("Tolerance (for example 1e-6, must be > 0): ");
                    int max_iter = read_int("Maximum iterations (> 0): ");
                    numcomp::AdvancedRootFinder finder;
                    // Work out the answer first: if it throws, no half-printed line is left
                    double result = finder.secant_method(root_equation, x0, x1, tol, max_iter);
                    std::cout << "\nSecant root: " << result << "\n";
                    break;
                }
                case 7: {
                    double x0 = read_double("Starting guess x0 (>= -2): ");
                    double tol = read_double("Tolerance (for example 1e-6, must be > 0): ");
                    int max_iter = read_int("Maximum iterations (> 0): ");
                    numcomp::AdvancedRootFinder finder;
                    // Work out the answer first: if it throws, no half-printed line is left
                    double result = finder.fixed_point_iteration(fixed_point_g, x0, tol, max_iter);
                    std::cout << "\nFixed point: " << result << "\n";
                    break;
                }
                case 8: {
                    double a = read_double("Lower limit a: ");
                    double b = read_double("Upper limit b: ");
                    int n = read_int("Number of strips n (> 0): ");
                    numcomp::BasicIntegrator integrator;
                    // Work out the answer first: if it throws, no half-printed line is left
                    double result = integrator.trapezoidal_rule(sample_equation, a, b, n);
                    std::cout << "\nTrapezoidal result: " << result << "\n";
                    std::cout << "Exact (b^3 - a^3) / 3: " << (b * b * b - a * a * a) / 3.0 << "\n";
                    break;
                }
                case 9: {
                    double a = read_double("Lower limit a: ");
                    double b = read_double("Upper limit b: ");
                    int n = read_int("Number of strips n (> 0): ");
                    numcomp::BasicIntegrator integrator;
                    // Work out the answer first: if it throws, no half-printed line is left
                    double result = integrator.midpoint_rule(sample_equation, a, b, n);
                    std::cout << "\nMidpoint result: " << result << "\n";
                    std::cout << "Exact (b^3 - a^3) / 3: " << (b * b * b - a * a * a) / 3.0 << "\n";
                    break;
                }
                case 10: {
                    double a = read_double("Lower limit a: ");
                    double b = read_double("Upper limit b: ");
                    int n = read_int("Number of strips n (positive and EVEN): ");
                    numcomp::AdvancedIntegrator integrator;
                    // Work out the answer first: if it throws, no half-printed line is left
                    double result = integrator.simpsons_rule(sample_equation, a, b, n);
                    std::cout << "\nSimpson's result: " << result << "\n";
                    std::cout << "Exact (b^3 - a^3) / 3: " << (b * b * b - a * a * a) / 3.0 << "\n";
                    break;
                }
                case 11: {
                    double a = read_double("Lower limit a: ");
                    double b = read_double("Upper limit b: ");
                    int max_iter = read_int("Romberg rows (1 to 25): ");
                    numcomp::AdvancedIntegrator integrator;
                    // Work out the answer first: if it throws, no half-printed line is left
                    double result = integrator.romberg_integration(sample_equation, a, b, max_iter);
                    std::cout << "\nRomberg result: " << result << "\n";
                    std::cout << "Exact (b^3 - a^3) / 3: " << (b * b * b - a * a * a) / 3.0 << "\n";
                    break;
                }
                case 12: {
                    double t_end = read_double("End time t_end (> 0): ");
                    double h = read_double("Step size h (> 0, at most 1,000,000 steps): ");
                    numcomp::BasicODESolver solver;
                    std::vector<std::pair<double, double>> curve = solver.euler(sample_ode, 0.0, 1.0, t_end, h);
                    std::cout << "\nEuler: " << curve.size() << " points.\n";
                    std::cout << "y(" << curve.back().first << ") = " << curve.back().second << "\n";
                    std::cout << "Exact e^(t^2/2) = " << std::exp(t_end * t_end / 2.0) << "\n";
                    break;
                }
                case 13: {
                    double t_end = read_double("End time t_end (> 0): ");
                    double h = read_double("Step size h (> 0, at most 1,000,000 steps): ");
                    numcomp::BasicODESolver solver;
                    std::vector<std::pair<double, double>> curve = solver.heun(sample_ode, 0.0, 1.0, t_end, h);
                    std::cout << "\nHeun: " << curve.size() << " points.\n";
                    std::cout << "y(" << curve.back().first << ") = " << curve.back().second << "\n";
                    std::cout << "Exact e^(t^2/2) = " << std::exp(t_end * t_end / 2.0) << "\n";
                    break;
                }
                case 14: {
                    double t_end = read_double("End time t_end (> 0): ");
                    double h = read_double("Step size h (> 0, at most 1,000,000 steps): ");
                    numcomp::AdvancedODESolver solver;
                    std::vector<std::pair<double, double>> curve = solver.rk3(sample_ode, 0.0, 1.0, t_end, h);
                    std::cout << "\nRK3: " << curve.size() << " points.\n";
                    std::cout << "y(" << curve.back().first << ") = " << curve.back().second << "\n";
                    std::cout << "Exact e^(t^2/2) = " << std::exp(t_end * t_end / 2.0) << "\n";
                    break;
                }
                case 15: {
                    double t_end = read_double("End time t_end (> 0): ");
                    double h = read_double("Step size h (> 0, at most 1,000,000 steps): ");
                    numcomp::AdvancedODESolver solver;
                    std::vector<std::pair<double, double>> curve = solver.rk4(sample_ode, 0.0, 1.0, t_end, h);
                    std::cout << "\nRK4: " << curve.size() << " points.\n";
                    std::cout << "y(" << curve.back().first << ") = " << curve.back().second << "\n";
                    std::cout << "Exact e^(t^2/2) = " << std::exp(t_end * t_end / 2.0) << "\n";
                    break;
                }
                case 16: {
                    std::vector<std::vector<double>> A = example_A;
                    std::vector<double> b = example_b;
                    int own = read_int("1 = example system, 2 = type your own: ");
                    if (own == 2) {
                        for (int i = 0; i < 3; ++i) {
                            for (int j = 0; j < 3; ++j) {
                                A[i][j] = read_double("A[" + std::to_string(i + 1) + "][" + std::to_string(j + 1) + "] = ");
                            }
                            b[i] = read_double("b[" + std::to_string(i + 1) + "] = ");
                        }
                    }
                    numcomp::MatrixSolver solver;
                    std::vector<double> x = solver.gaussian_elimination(A, b);
                    std::cout << "\nGaussian elimination: x1 = " << x[0] << ", x2 = " << x[1] << ", x3 = " << x[2] << "\n";
                    break;
                }
                case 17: {
                    std::vector<std::vector<double>> A = example_A;
                    std::vector<double> b = example_b;
                    int own = read_int("1 = example system, 2 = type your own: ");
                    if (own == 2) {
                        for (int i = 0; i < 3; ++i) {
                            for (int j = 0; j < 3; ++j) {
                                A[i][j] = read_double("A[" + std::to_string(i + 1) + "][" + std::to_string(j + 1) + "] = ");
                            }
                            b[i] = read_double("b[" + std::to_string(i + 1) + "] = ");
                        }
                    }
                    numcomp::MatrixSolver solver;
                    std::vector<double> x = solver.lu_solve(A, b);
                    std::cout << "\nLU decomposition: x1 = " << x[0] << ", x2 = " << x[1] << ", x3 = " << x[2] << "\n";
                    break;
                }
                default:
                    std::cout << "\n[!] Invalid choice. Select 0-17.\n";
            }
        } catch (const std::invalid_argument& e) {
            std::cout << "\n[!] Invalid input: " << e.what() << "\n";
        } catch (const std::out_of_range& e) {
            std::cout << "\n[!] Out of range: " << e.what() << "\n";
        } catch (const std::runtime_error& e) {
            std::cout << "\n[!] Calculation failed: " << e.what() << "\n";
        } catch (const std::exception& e) {
            // Anything else (for example running out of memory) no longer crashes the program
            std::cout << "\n[!] Unexpected error: " << e.what() << "\n";
        }
    }
    return 0;
}
