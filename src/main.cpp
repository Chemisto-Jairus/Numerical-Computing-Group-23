// Group 23: Numerical Computing command-line app
// Owners: Nabakooza Bridget Maria (Member 9) with Chemisto Jairus (Member 1)
//
// Every module of the library is reachable from the menu.
// The user types the inputs. Every number is checked before it is used,
// the input limits are shown, and every library call is inside try/catch.
//
// HELPERS: the input reading and result printing that every menu option
// needs are written once (read_number, read_root_settings, read_limits,
// print_integral, read_ode_settings, print_ode_result, read_system,
// print_solution). Each menu option is now only a few lines.

#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <cstdlib>
#include <stdexcept>
#include <limits>
#include <utility>
#include "numerical-computing/interpolator.hpp"
#include "numerical-computing/cubic_spline.hpp"
#include "numerical-computing/basic_root_finder.hpp"
#include "numerical-computing/advanced_root_finder.hpp"
#include "numerical-computing/basic_integrator.hpp"
#include "numerical-computing/advanced_integrator.hpp"
#include "numerical-computing/basic_ode_solver.hpp"
#include "numerical-computing/advanced_ode_solver.hpp"
#include "numerical-computing/matrix_solver.hpp"

using Curve = std::vector<std::pair<double, double>>;
using Matrix = std::vector<std::vector<double>>;

// Sample functions used by the demos
double sample_equation(double x) { return x * x; }              // f(x) = x^2
double root_equation(double x) { return (x * x) - 4.0; }        // f(x) = x^2 - 4, roots at -2 and 2
double root_derivative(double x) { return 2.0 * x; }            // f'(x) = 2x
double fixed_point_g(double x) { return std::sqrt(x + 2.0); }   // g(x) = sqrt(x + 2), fixed point 2
double sample_ode(double t, double y) { return t * y; }         // dy/dt = t * y, exact y = e^(t^2 / 2)

// ------------------------------------------------------------------
// Input helpers
// ------------------------------------------------------------------

/// Reads one number of type T (int or double). Keeps asking until it is valid.
/// If the input ends (Ctrl+Z / Ctrl+D) the program closes cleanly.
/// One template replaces the old read_int() and read_double() copies.
template <typename T>
T read_number(const std::string& prompt) {
    T value;
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
        std::cout << "[!] Please enter a valid number.\n";
    }
}

/// Tolerance and iteration limit, asked by all four root finders.
void read_root_settings(double& tol, int& max_iter) {
    tol = read_number<double>("Tolerance (for example 1e-6, must be > 0): ");
    max_iter = read_number<int>("Maximum iterations (> 0): ");
}

/// Integration limits, asked by all four integrators.
void read_limits(double& a, double& b) {
    a = read_number<double>("Lower limit a: ");
    b = read_number<double>("Upper limit b: ");
}

/// End time and step size, asked by all four ODE solvers.
void read_ode_settings(double& t_end, double& h) {
    t_end = read_number<double>("End time t_end (> 0): ");
    h = read_number<double>("Step size h (> 0, at most 1,000,000 steps): ");
}

/// Uses the example system, or lets the user type a 3 x 3 system.
void read_system(Matrix& A, std::vector<double>& b) {
    int own = read_number<int>("1 = example system, 2 = type your own: ");
    if (own != 2) {
        return;
    }
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            A[i][j] = read_number<double>("A[" + std::to_string(i + 1) + "][" + std::to_string(j + 1) + "] = ");
        }
        b[i] = read_number<double>("b[" + std::to_string(i + 1) + "] = ");
    }
}

// ------------------------------------------------------------------
// Output helpers
// ------------------------------------------------------------------

/// Prints an integration result next to the exact answer for f(x) = x^2.
void print_integral(const std::string& method, double result, double a, double b) {
    std::cout << "\n" << method << " result: " << result << "\n";
    std::cout << "Exact (b^3 - a^3) / 3: " << (b * b * b - a * a * a) / 3.0 << "\n";
}

/// Prints the end of an ODE curve next to the exact answer e^(t^2 / 2).
void print_ode_result(const std::string& method, const Curve& curve, double t_end) {
    std::cout << "\n" << method << ": " << curve.size() << " points.\n";
    std::cout << "y(" << curve.back().first << ") = " << curve.back().second << "\n";
    std::cout << "Exact e^(t^2/2) = " << std::exp(t_end * t_end / 2.0) << "\n";
}

/// Prints the solution of a 3 x 3 system.
void print_solution(const std::string& method, const std::vector<double>& x) {
    std::cout << "\n" << method << ": x1 = " << x[0] << ", x2 = " << x[1] << ", x3 = " << x[2] << "\n";
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
    Matrix example_A = {{2.0, 1.0, -1.0}, {-3.0, -1.0, 2.0}, {-2.0, 1.0, 2.0}};
    std::vector<double> example_b = {8.0, -11.0, -3.0};

    // One object of each class, reused by every menu option (they are stateless)
    numcomp::Interpolator interp;
    numcomp::CubicSpline spline;
    numcomp::BasicRootFinder basic_roots;
    numcomp::AdvancedRootFinder advanced_roots;
    numcomp::BasicIntegrator basic_integrator;
    numcomp::AdvancedIntegrator advanced_integrator;
    numcomp::BasicODESolver basic_ode;
    numcomp::AdvancedODESolver advanced_ode;
    numcomp::MatrixSolver matrix;

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

        // Every library call is inside try/catch, so bad input shows a message instead of crashing.
        // Results are worked out BEFORE printing, so an error never leaves a half-printed line.
        try {
            double a, b, tol, t_end, h;
            int max_iter;

            switch (choice) {
                case 1: {
                    double x = read_number<double>("x to estimate (0 to 4): ");
                    double result = interp.linear_interpolate(x, data_x, data_y);
                    std::cout << "\nLinear interpolation at x = " << x << ": " << result << "\nExact x^2: " << x * x << "\n";
                    break;
                }
                case 2: {
                    a = read_number<double>("Left end a: ");
                    b = read_number<double>("Right end b (bigger than a): ");
                    int n = read_number<int>("Number of nodes n (1 to 1,000,000): ");
                    std::vector<double> nodes = interp.chebyshev_nodes(a, b, n);
                    std::cout << "\nChebyshev nodes (first 10 shown):\n";
                    for (std::size_t i = 0; i < nodes.size() && i < 10; ++i) {
                        std::cout << "  x" << i + 1 << " = " << nodes[i] << "\n";
                    }
                    break;
                }
                case 3: {
                    double x = read_number<double>("x to estimate (0 to 4): ");
                    double result = spline.interpolate(x, data_x, data_y);
                    std::cout << "\nCubic spline at x = " << x << ": " << result << "\nExact x^2: " << x * x << "\n";
                    break;
                }
                case 4: {
                    a = read_number<double>("Left end a: ");
                    b = read_number<double>("Right end b: ");
                    read_root_settings(tol, max_iter);
                    double root = basic_roots.bisection(root_equation, a, b, tol, max_iter);
                    std::cout << "\nBisection root: " << root << "\n";
                    break;
                }
                case 5: {
                    double x0 = read_number<double>("Starting guess x0: ");
                    read_root_settings(tol, max_iter);
                    double root = basic_roots.newton_raphson(root_equation, root_derivative, x0, tol, max_iter);
                    std::cout << "\nNewton-Raphson root: " << root << "\n";
                    break;
                }
                case 6: {
                    double x0 = read_number<double>("First guess x0: ");
                    double x1 = read_number<double>("Second guess x1: ");
                    read_root_settings(tol, max_iter);
                    double root = advanced_roots.secant_method(root_equation, x0, x1, tol, max_iter);
                    std::cout << "\nSecant root: " << root << "\n";
                    break;
                }
                case 7: {
                    double x0 = read_number<double>("Starting guess x0 (>= -2): ");
                    read_root_settings(tol, max_iter);
                    double root = advanced_roots.fixed_point_iteration(fixed_point_g, x0, tol, max_iter);
                    std::cout << "\nFixed point: " << root << "\n";
                    break;
                }
                case 8: {
                    read_limits(a, b);
                    int n = read_number<int>("Number of strips n (> 0): ");
                    print_integral("Trapezoidal", basic_integrator.trapezoidal_rule(sample_equation, a, b, n), a, b);
                    break;
                }
                case 9: {
                    read_limits(a, b);
                    int n = read_number<int>("Number of strips n (> 0): ");
                    print_integral("Midpoint", basic_integrator.midpoint_rule(sample_equation, a, b, n), a, b);
                    break;
                }
                case 10: {
                    read_limits(a, b);
                    int n = read_number<int>("Number of strips n (positive and EVEN): ");
                    print_integral("Simpson's", advanced_integrator.simpsons_rule(sample_equation, a, b, n), a, b);
                    break;
                }
                case 11: {
                    read_limits(a, b);
                    max_iter = read_number<int>("Romberg rows (1 to 25): ");
                    print_integral("Romberg", advanced_integrator.romberg_integration(sample_equation, a, b, max_iter), a, b);
                    break;
                }
                case 12:
                    read_ode_settings(t_end, h);
                    print_ode_result("Euler", basic_ode.euler(sample_ode, 0.0, 1.0, t_end, h), t_end);
                    break;
                case 13:
                    read_ode_settings(t_end, h);
                    print_ode_result("Heun", basic_ode.heun(sample_ode, 0.0, 1.0, t_end, h), t_end);
                    break;
                case 14:
                    read_ode_settings(t_end, h);
                    print_ode_result("RK3", advanced_ode.rk3(sample_ode, 0.0, 1.0, t_end, h), t_end);
                    break;
                case 15:
                    read_ode_settings(t_end, h);
                    print_ode_result("RK4", advanced_ode.rk4(sample_ode, 0.0, 1.0, t_end, h), t_end);
                    break;
                case 16: {
                    Matrix A = example_A;
                    std::vector<double> rhs = example_b;
                    read_system(A, rhs);
                    print_solution("Gaussian elimination", matrix.gaussian_elimination(A, rhs));
                    break;
                }
                case 17: {
                    Matrix A = example_A;
                    std::vector<double> rhs = example_b;
                    read_system(A, rhs);
                    print_solution("LU decomposition", matrix.lu_solve(A, rhs));
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
