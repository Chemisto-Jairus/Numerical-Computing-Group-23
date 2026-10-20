#include <iostream>
#include <vector>
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

double sample_equation(double x) { return x * x; }
double root_equation(double x) { return (x * x) - 4.0; }
double root_derivative(double x) { return 2.0 * x; }
double sample_ode(double t, double y) { return t * y; }

void display_menu() {
    std::cout << "\n--- GROUP 23: NUMERICAL COMPUTING ---\n";
    std::cout << "1. Basic Interpolation (Linear)\n";
    std::cout << "2. Basic Integration (Trapezoidal)\n";
    std::cout << "3. Advanced Integration (Simpson's 1/3)\n";
    std::cout << "4. Advanced Root Finder (Secant Method)\n";
    std::cout << "5. Basic ODE Solver (Euler)\n";
    std::cout << "6. Advanced ODE Solver (RK4)\n";
    std::cout << "7. Basic Root Finder (Bisection)\n";
    std::cout << "8. Basic Root Finder (Newton-Raphson)\n";
    std::cout << "9. Cubic Spline Interpolation\n";
    std::cout << "10. Matrix Solver (Gaussian Elimination)\n";
    std::cout << "11. Matrix Solver (LU Decomposition)\n";
    std::cout << "0. Exit Application\n";
    std::cout << "Enter your choice: ";
}

int main() {
    int choice;
    while (true) {
        display_menu();

        if (!(std::cin >> choice)) {
            if (std::cin.eof()) break;
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "\n[!] Error: Please enter a valid number.\n";
            continue;
        }

        if (choice == 0) {
            std::cout << "Exiting application. Goodbye!\n";
            break;
        }

        try {
            switch (choice) {
                case 1: {
                    numcomp::Interpolator interp;
                    std::vector<double> x = {0.0, 2.0, 4.0};
                    std::vector<double> y = {0.0, 4.0, 16.0};
                    // Work out the answer first: if it throws, no half-printed line is left
                    double result = interp.linear_interpolate(1.0, x, y);
                    std::cout << "\nLinear Interpolation result: " << result << "\n";
                    break;
                }
                case 2: {
                    numcomp::BasicIntegrator integrator;
                    double result = integrator.trapezoidal_rule(sample_equation, 0.0, 2.0, 100);
                    std::cout << "\nTrapezoidal Rule result: " << result << "\n";
                    break;
                }
                case 3: {
                    numcomp::AdvancedIntegrator integrator;
                    // Work out the answer first: if it throws, no half-printed line is left
                    double result = integrator.simpsons_rule(sample_equation, 0.0, 2.0, 100);
                    std::cout << "\nSimpson's Rule result: " << result << "\n";
                    break;
                }
                case 4: {
                    numcomp::AdvancedRootFinder finder;
                    // Work out the answer first: if it throws, no half-printed line is left
                    double result = finder.secant_method(root_equation, 1.0, 3.0, 1e-5, 100);
                    std::cout << "\nSecant Method result: " << result << "\n";
                    break;
                }
                case 5: {
                    numcomp::BasicODESolver solver;
                    auto curve = solver.euler(sample_ode, 0.0, 1.0, 1.0, 0.1);
                    std::cout << "\nEuler calculated " << curve.size() << " points.\n";
                    break;
                }
                case 6: {
                    numcomp::AdvancedODESolver solver;
                    auto curve = solver.rk4(sample_ode, 0.0, 1.0, 1.0, 0.1);
                    std::cout << "\nRK4 calculated " << curve.size() << " points.\n";
                    break;
                }
                case 7: {
                    numcomp::BasicRootFinder finder;
                    // Work out the answer first: if it throws, no half-printed line is left
                    double result = finder.bisection(root_equation, 0.0, 3.0, 1e-6, 100);
                    std::cout << "\nBisection result: " << result << "\n";
                    break;
                }
                case 8: {
                    numcomp::BasicRootFinder finder;
                    // Work out the answer first: if it throws, no half-printed line is left
                    double result = finder.newton_raphson(root_equation, root_derivative, 3.0, 1e-6, 100);
                    std::cout << "\nNewton-Raphson result: " << result << "\n";
                    break;
                }
                case 9: {
                    numcomp::CubicSpline spline;
                    std::vector<double> x = {0.0, 1.0, 2.0, 3.0, 4.0};
                    std::vector<double> y = {0.0, 1.0, 4.0, 9.0, 16.0};
                    // Work out the answer first: if it throws, no half-printed line is left
                    double result = spline.interpolate(2.5, x, y);
                    std::cout << "\nCubic Spline result at x = 2.5: " << result << "\n";
                    break;
                }
                case 10: {
                    numcomp::MatrixSolver solver;
                    std::vector<std::vector<double>> A = {{2.0, 1.0, -1.0}, {-3.0, -1.0, 2.0}, {-2.0, 1.0, 2.0}};
                    std::vector<double> b = {8.0, -11.0, -3.0};
                    std::vector<double> x = solver.gaussian_elimination(A, b);
                    std::cout << "\nGaussian Elimination result: x = " << x[0] << ", y = " << x[1] << ", z = " << x[2] << "\n";
                    break;
                }
                case 11: {
                    numcomp::MatrixSolver solver;
                    std::vector<std::vector<double>> A = {{2.0, 1.0, -1.0}, {-3.0, -1.0, 2.0}, {-2.0, 1.0, 2.0}};
                    std::vector<double> b = {8.0, -11.0, -3.0};
                    std::vector<double> x = solver.lu_solve(A, b);
                    std::cout << "\nLU Decomposition result: x = " << x[0] << ", y = " << x[1] << ", z = " << x[2] << "\n";
                    break;
                }
                default:
                    std::cout << "\n[!] Invalid choice. Select 0-11.\n";
            }
        } catch (const std::invalid_argument& e) {
            std::cout << "\n[!] Invalid input: " << e.what() << "\n";
        } catch (const std::out_of_range& e) {
            std::cout << "\n[!] Out of range: " << e.what() << "\n";
        } catch (const std::runtime_error& e) {
            std::cout << "\n[!] Calculation failed: " << e.what() << "\n";
        } catch (const std::exception& e) {
            std::cout << "\n[!] Unexpected error: " << e.what() << "\n";
        }
    }
    return 0;
}
