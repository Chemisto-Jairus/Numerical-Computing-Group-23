#include <iostream>
#include <vector>
#include "numerical-computing/interpolation.hpp"

void display_menu() {
    std::cout << "\n--- GROUP 23: NUMERICAL COMPUTING ---\n";
    std::cout << "1. Basic Interpolation (Linear)\n";
    std::cout << "0. Exit Application\n";
    std::cout << "Enter your choice: ";
}

int main() {
    int choice;
    while (true) {
        display_menu();
        std::cin >> choice;
        
        if (choice == 0) {
            std::cout << "Exiting application. Goodbye!\n";
            break;
        }
        
        switch (choice) {
            case 1: {
                std::cout << "\n--- Running Linear Interpolation ---\n";
                numcomp::Interpolator interp; // FIXED: Using Derrick's exact class name
                std::vector<double> x_vals = {0.0, 2.0, 4.0};
                std::vector<double> y_vals = {0.0, 4.0, 16.0};
                
                // FIXED: Derrick's target 'x' goes first!
                double result = interp.linear_interpolate(1.0, x_vals, y_vals);
                std::cout << "Interpolated value at x = 1.0 is " << result << "\n";
                break;
            }
            default:
                std::cout << "Invalid choice. Please try again.\n";
        }
    }
    return 0;
}