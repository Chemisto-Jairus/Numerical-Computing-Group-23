#include "numerical-computing/adv_roots.hpp"
#include <iostream>
#include <cmath>
#include <cassert>

// Test equation: f(x) = x^2 - 4. 
// The exact positive root is x = 2.0
double test_equation(double x) 
{
    return x * x - 4.0;
}

void test_secant_method() 
{
    numcomp::AdvancedRootFinder finder; // Instantiate the stateless class
    
    // Test with initial guesses x0 = 1.0, x1 = 3.0, tolerance = 1e-6, max iterations = 100
    double root = finder.secant_method(test_equation, 1.0, 3.0, 1e-6, 100);
    double expected = 2.0;
    
    assert(std::abs(root - expected) < 1e-5 && "Secant method output is outside acceptable tolerance!");
    std::cout << "[-] Secant Method test passed!" << std::endl;
}

// For Fixed-Point Iteration, we solve x = g(x).
// We'll use g(x) = sqrt(x + 2), which corresponds to finding the root of f(x) = x^2 - x - 2 = 0.
// The exact positive root is x = 2.0
double g_function(double x) 
{
    return std::sqrt(x + 2.0);
}

void test_fixed_point_iteration() 
{
    numcomp::AdvancedRootFinder finder;
    
    // Test with initial guess x0 = 1.0, tolerance = 1e-6, max iterations = 100
    double root = finder.fixed_point_iteration(g_function, 1.0, 1e-6, 100);
    double expected = 2.0;
    
    assert(std::abs(root - expected) < 1e-5 && "Fixed-Point Iteration output is outside acceptable tolerance!");
    std::cout << "[-] Fixed-Point Iteration test passed!" << std::endl;
}

int main() 
{
    std::cout << "[*] Running Advanced Root Finder Tests..." << std::endl;
    
    test_secant_method();
    test_fixed_point_iteration(); // Newly added test
    
    std::cout << "[+] All Advanced Root Finder tests passed successfully!" << std::endl;
    
    return 0;
}