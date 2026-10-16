#include <iostream>
#include <vector>
#include "numerical-computing/interpolation.hpp"
#include "numerical-computing/basic_integration.hpp"
#include "numerical-computing/adv_integration.hpp"
#include "numerical-computing/adv_roots.hpp"

double sample_equation(double x) { return x * x; }
double root_equation(double x) { return (x * x) - 4.0; }

void display_menu() {
    std::cout << "\n--- GROUP 23: NUMERICAL COMPUTING ---\n";
    std::cout << "1. Basic Interpolation (Linear)\n";
    std::cout << "2. Basic Integration (Trapezoidal)\n";
    std::cout << "3. Advanced Integration (Simpson's)\n";
    std::cout << "4. Advanced Root Finder (Secant Method)\n";
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
                std::cout << "\n--- Running Linear Interpolation ---\n";
                numcomp::Interpolator interp;
                std::vector<double> x = {0.0, 2.0, 4.0};
                std::vector<double> y = {0.0, 4.0, 16.0};
                std::cout << "Value at x=1.0 is " << interp.linear_interpolate(1.0, x, y) << "\n";
                break;
            }
            case 2: {
                std::cout << "\n--- Running Trapezoidal Rule ---\n";
                numcomp::BasicIntegrator integrator;
                std::cout << "Integral is: " << integrator.trapezoidal_rule(sample_equation, 0.0, 2.0, 100) << "\n";
                break;
            }
            case 3: {
                std::cout << "\n--- Running Simpson's Rule ---\n";
                numcomp::AdvancedIntegrator integrator;
                std::cout << "Integral is: " << integrator.simpsons_rule(sample_equation, 0.0, 2.0, 100) << "\n";
                break;
            }
            case 4: {
                std::cout << "\n--- Running Secant Method on f(x)=x^2-4 ---\n";
                numcomp::AdvancedRootFinder finder;
                std::cout << "Root found near x = " << finder.secant_method(root_equation, 1.0, 3.0, 1e-5, 100) << "\n";
                break;
            }
            default: std::cout << "Invalid choice.\n";
        }
    }
    return 0;
}