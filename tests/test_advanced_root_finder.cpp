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

int main() 
{
    std::cout << "[*] Running Advanced Root Finder Tests..." << std::endl;
    
    test_secant_method();
    
    std::cout << "[+] Advanced Root Finder tests (Part 1) passed successfully!" << std::endl;
    
    return 0;
}