#include "numerical-computing/basic_integration.hpp"
#include <stdexcept>

namespace numcomp 
{
    double trapezoidal_rule(std::function<double(double)> f, double a, double b, int n) 
    {
        if (n <= 0) 
        {
            throw std::invalid_argument("Number of intervals must be strictly positive.");
        }
        
        double h = (b - a) / n;
        double sum = 0.5 * (f(a) + f(b));
        
        for (int i = 1; i < n; ++i) 
        {
            sum += f(a + i * h);
        }
        
        return sum * h;
    }
}