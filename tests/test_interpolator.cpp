#undef NDEBUG  // keep assert() working even in Release builds, so the tests always check
#include "numerical-computing/interpolator.hpp"
#include <iostream>
#include <vector>
#include <cmath>
#include <cassert>

void test_linear_interpolation() 
{
    numcomp::Interpolator interp;
    
    // Test data: A straight line y = 2x
    std::vector<double> x_values = {0.0, 1.0, 2.0, 3.0};
    std::vector<double> y_values = {0.0, 2.0, 4.0, 6.0};
    
    // The target x (1.5) is passed first
    double result = interp.linear_interpolate(1.5, x_values, y_values);
    double expected = 3.0;
    
    assert(std::abs(result - expected) < 1e-5 && "Linear interpolation failed!");
    std::cout << "[-] Linear Interpolation test passed!" << std::endl;
}
void test_chebyshev_nodes() 
{
    numcomp::Interpolator interp;
    
    // Generate 2 nodes between -1.0 and 1.0
    int n = 2;
    std::vector<double> nodes = interp.chebyshev_nodes(-1.0, 1.0, n);
    
    // Test 1: Did it generate exactly 2 nodes?
    assert(nodes.size() == static_cast<size_t>(n) && "Chebyshev nodes returned the wrong number of points!");
    
    // Test 2: For n=2 between -1 and 1, the exact nodes are at +/- 0.707106 (cos(pi/4))
    double expected_val = std::cos(std::acos(-1.0) / 4.0); 
    
    assert(std::abs(std::abs(nodes[0]) - expected_val) < 1e-5 && "Chebyshev node mathematical values are incorrect!");
    std::cout << "[-] Chebyshev Nodes generation test passed!" << std::endl;
}

int main() 
{
    std::cout << "[*] Running Interpolation Tests..." << std::endl;
    
    test_linear_interpolation();
    test_chebyshev_nodes(); 
    
    std::cout << "[+] All Interpolation tests passed successfully!" << std::endl;
    
    return 0;
}
