#include <iostream>
#include <vector>
#include <stdexcept>
#include <limits>
#include "numerical-computing/interpolator.hpp"
#include "numerical-computing/basic_integrator.hpp"
#include "numerical-computing/advanced_integrator.hpp"
#include "numerical-computing/advanced_root_finder.hpp"
#include "numerical-computing/basic_ode_solver.hpp"
#include "numerical-computing/advanced_ode_solver.hpp"

// Sample functions used by the demos
double sample_equation(double x) { return x * x; }          // f(x) = x^2
double root_equation(double x) { return (x * x) - 4.0; }    // f(x) = x^2 - 4
double sample_ode(double t, double y) { return t * y; }     // dy/dt = t * y

void display_menu() {
    std::cout << "\n--- GROUP 23: NUMERICAL COMPUTING ---\n";
    std::cout << "1. Basic Interpolation (Linear)\n";
    std::cout << "2. Basic Integration (Trapezoidal)\n";
    std::cout << "3. Advanced Integration (Simpson's 1/3)\n";
    std::cout << "4. Advanced Root Finder (Secant Method)\n";
    std::cout << "5. Basic ODE Solver (Euler)\n";
    std::cout << "6. Advanced ODE Solver (RK4)\n";
    std::cout << "0. Exit Application\n";
    std::cout << "Enter your choice: ";
}

int main() {
    int choice;
    while (true) {
        display_menu();

        // If the input is not a number, clear the error and ask again
        if (!(std::cin >> choice)) {
            if (std::cin.eof()) break;   // the input has ended (for example Ctrl+Z), so stop
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
                    numcomp::Interpolator interp;
                    std::vector<double> x = {0.0, 2.0, 4.0};
                    std::vector<double> y = {0.0, 4.0, 16.0};
                    std::cout << "\nLinear Interpolation result: " << interp.linear_interpolate(1.0, x, y) << "\n";
                    break;
                }
                case 2: {
                    numcomp::BasicIntegrator integrator;
                    std::cout << "\nTrapezoidal Rule result: " << integrator.trapezoidal_rule(sample_equation, 0.0, 2.0, 100) << "\n";
                    break;
                }
                case 3: {
                    numcomp::AdvancedIntegrator integrator;
                    std::cout << "\nSimpson's Rule result: " << integrator.simpsons_rule(sample_equation, 0.0, 2.0, 100) << "\n";
                    break;
                }
                case 4: {
                    numcomp::AdvancedRootFinder finder;
                    std::cout << "\nSecant Method result: " << finder.secant_method(root_equation, 1.0, 3.0, 1e-5, 100) << "\n";
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
                default:
                    std::cout << "\n[!] Invalid choice. Select 0-6.\n";
            }
        } catch (const std::invalid_argument& e) {
            std::cout << "\n[!] Invalid input: " << e.what() << "\n";
        } catch (const std::out_of_range& e) {
            std::cout << "\n[!] Out of range: " << e.what() << "\n";
        } catch (const std::runtime_error& e) {
            std::cout << "\n[!] Calculation failed: " << e.what() << "\n";
        }
    }
    return 0;
}
