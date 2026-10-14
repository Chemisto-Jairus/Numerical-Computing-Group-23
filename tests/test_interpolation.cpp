#include "numerical-computing/interpolation.hpp"
#include <iostream>
#include <vector>
#include <cmath>
#include <cassert>

void test_linear_interpolation() 
{
    numcomp::Interpolator interp; // FIXED: Exact class name
    
    // Test data: A straight line y = 2x
    std::vector<double> x_values = {0.0, 1.0, 2.0, 3.0};
    std::vector<double> y_values = {0.0, 2.0, 4.0, 6.0};
    
    // FIXED: Target x (1.5) is passed first, exactly as Derrick defined it
    double result = interp.linear_interpolate(1.5, x_values, y_values);
    double expected = 3.0;
    
    assert(std::abs(result - expected) < 1e-5 && "Linear interpolation failed!");
    std::cout << "[-] Linear Interpolation test passed!" << std::endl;
}

int main() 
{
    std::cout << "[*] Running Interpolation Tests..." << std::endl;
    
    test_linear_interpolation();
    
    std::cout << "[+] Interpolation tests (Part 1) passed successfully!" << std::endl;
    
    return 0;
}