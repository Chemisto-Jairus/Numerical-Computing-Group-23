#include "numerical-computing/basic_integration.hpp"
#include <stdexcept>

namespace numcomp 
{
    double BasicIntegrator::trapezoidal_rule(std::function<double(double)> f, double a, double b, int n) 
    {
        if (n <= 0) throw std::invalid_argument("Number of intervals must be strictly positive.");
        
        double h = (b - a) / n;
        double sum = 0.5 * (f(a) + f(b));
        for (int i = 1; i < n; ++i) {
            sum += f(a + i * h);
        }
        return sum * h;
    }

    double BasicIntegrator::midpoint_rule(std::function<double(double)> f, double a, double b, int n) 
    {
        if (n <= 0) throw std::invalid_argument("Number of intervals must be strictly positive.");
        
        double h = (b - a) / n;
        double sum = 0.0;
        for (int i = 0; i < n; ++i) {
            double midpoint = a + (i + 0.5) * h;
            sum += f(midpoint);
        }
        return sum * h;
    }
}