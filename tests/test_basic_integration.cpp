#include "numerical-computing/basic_integration.hpp"
#include <iostream>
#include <cmath>
#include <cassert>

// A simple test equation: f(x) = x^2
// The exact mathematical integral of x^2 from 0 to 1 is 1/3 (approx 0.333333)
double test_equation(double x) 
{
    return x * x;
}

void test_trapezoidal_rule() 
{
    numcomp::BasicIntegrator integrator; // Instantiate the stateless class
    
    // Test with 100 intervals from 0.0 to 1.0
    double result = integrator.trapezoidal_rule(test_equation, 0.0, 1.0, 100);
    double expected = 1.0 / 3.0;
    
    // We use a small tolerance (1e-4) because numerical integration is an approximation
    double tolerance = 1e-4;
    
    assert(std::abs(result - expected) < tolerance && "Trapezoidal rule output is outside acceptable tolerance!");
    std::cout << "[-] Trapezoidal Rule test passed!" << std::endl;
}

int main() 
{
    std::cout << "[*] Running Basic Integration Tests..." << std::endl;
    
    test_trapezoidal_rule();
    
    std::cout << "[+] Basic Integration tests (Part 1) passed successfully!" << std::endl;
    
    return 0;
}