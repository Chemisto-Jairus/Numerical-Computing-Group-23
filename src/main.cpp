#include <iostream>
#include <vector>
#include "numerical-computing/interpolation.hpp"
#include "numerical-computing/basic_integration.hpp"
#include "numerical-computing/adv_integration.hpp"
#include "numerical-computing/adv_roots.hpp"
#include "numerical-computing/basic_ode.hpp"
#include "numerical-computing/advanced_ode.hpp"

double sample_equation(double x) { return x * x; }
double root_equation(double x) { return (x * x) - 4.0; }
double sample_ode(double t, double y) { return t * y; }

void display_menu() {
    std::cout << "\n--- GROUP 23: NUMERICAL COMPUTING ---\n";
    std::cout << "1. Basic Interpolation (Linear)\n";
    std::cout << "2. Basic Integration (Trapezoidal)\n";
    std::cout << "3. Advanced Integration (Simpson's)\n";
    std::cout << "4. Advanced Root Finder (Secant Method)\n";
    std::cout << "5. Basic ODE (Euler's Method)\n";
    std::cout << "6. Advanced ODE (Runge-Kutta 4)\n";
    std::cout << "0. Exit Application\n";
    std::cout << "Enter your choice: ";
}

int main() {
    int choice;
    while (true) {
        display_menu();
        std::cin >> choice;
        if (choice == 0) break;
        
        switch (choice) {
            case 1: {
                numcomp::Interpolator interp;
                std::vector<double> x = {0.0, 2.0, 4.0}; std::vector<double> y = {0.0, 4.0, 16.0};
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
                std::cout << "\n--- Running Euler's Method ---\n";
                numcomp::BasicODESolver solver;
                auto curve = solver.euler(sample_ode, 0.0, 1.0, 1.0, 0.1);
                std::cout << "Calculated " << curve.size() << " points.\n";
                break;
            }
            case 6: {
                std::cout << "\n--- Running Runge-Kutta 4 (RK4) ---\n";
                numcomp::AdvancedODESolver solver;
                auto curve = solver.rk4(sample_ode, 0.0, 1.0, 1.0, 0.1);
                std::cout << "Calculated " << curve.size() << " points.\n";
                break;
            }
            default: std::cout << "Invalid choice.\n";
        }
    }
    return 0;
}