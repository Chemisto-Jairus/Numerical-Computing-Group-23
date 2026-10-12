#include "numerical-computing/adv_integration.hpp"
#include <iostream>
#include <cmath>
#include <cassert>

// A simple test equation: f(x) = x^3
// The exact mathematical integral of x^3 from 0 to 2 is 4.0
double test_equation(double x) 
{
    return x * x * x;
}

void test_simpsons_rule() 
{
    numcomp::AdvancedIntegrator integrator; // Instantiate the stateless class
    
    // Test with 100 intervals from 0.0 to 2.0
    // Note: Simpson's 1/3 rule requires an even number of intervals
    double result = integrator.simpsons_rule(test_equation, 0.0, 2.0, 100);
    double expected = 4.0;
    
    // We can use a very tight tolerance (1e-6) because Simpson's is highly accurate here
    double tolerance = 1e-6;
    
    assert(std::abs(result - expected) < tolerance && "Simpson's rule output is outside acceptable tolerance!");
    std::cout << "[-] Simpson's Rule test passed!" << std::endl;
}

int main() 
{
    std::cout << "[*] Running Advanced Integration Tests..." << std::endl;
    
    test_simpsons_rule();
    
    std::cout << "[+] Advanced Integration tests (Part 1) passed successfully!" << std::endl;
    
    return 0;
}