#include <iostream>
#include <vector>
#include "numerical-computing/interpolation.hpp"
#include "numerical-computing/basic_integration.hpp"
#include "numerical-computing/adv_integration.hpp"

double sample_equation(double x) { return x * x; }

void display_menu() {
    std::cout << "\n--- GROUP 23: NUMERICAL COMPUTING ---\n";
    std::cout << "1. Basic Interpolation (Linear)\n";
    std::cout << "2. Basic Integration (Trapezoidal)\n";
    std::cout << "3. Advanced Integration (Simpson's 1/3)\n";
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
                std::vector<double> x_vals = {0.0, 2.0, 4.0};
                std::vector<double> y_vals = {0.0, 4.0, 16.0};
                std::cout << "Interpolated value at x = 1.0 is " << interp.linear_interpolate(1.0, x_vals, y_vals) << "\n";
                break;
            }
            case 2: {
                std::cout << "\n--- Running Trapezoidal Rule on f(x) = x^2 ---\n";
                numcomp::BasicIntegrator integrator;
                std::cout << "Integral from 0 to 2 is: " << integrator.trapezoidal_rule(sample_equation, 0.0, 2.0, 100) << "\n";
                break;
            }
            case 3: {
                std::cout << "\n--- Running Simpson's 1/3 Rule on f(x) = x^2 ---\n";
                numcomp::AdvancedIntegrator integrator;
                std::cout << "Integral from 0 to 2 is: " << integrator.simpsons_rule(sample_equation, 0.0, 2.0, 100) << "\n";
                break;
            }
            default: std::cout << "Invalid choice. Please try again.\n";
        }
    }
    return 0;
}