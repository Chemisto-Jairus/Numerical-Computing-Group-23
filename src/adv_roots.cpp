#include "numerical-computing/adv_roots.hpp"
#include <cmath>
#include <stdexcept>

namespace numcomp 
{
    double AdvancedRootFinder::secant_method(std::function<double(double)> f, double x0, double x1, double tol, int max_iter) {
        for (int iter = 0; iter < max_iter; ++iter) 
        {
            double f_x0 = f(x0);
            double f_x1 = f(x1);
            
            if (std::abs(f_x1 - f_x0) < 1e-12) 
            {
                throw std::runtime_error("Division by zero in Secant Method.");
            }
            
            double x2 = x1 - f_x1 * (x1 - x0) / (f_x1 - f_x0);
            if (std::abs(x2 - x1) < tol) return x2;
            
            x0 = x1;
            x1 = x2;
        }
        throw std::runtime_error("Secant method failed to converge.");
    }

    double AdvancedRootFinder::fixed_point_iteration(std::function<double(double)> g, double x0, double tol, int max_iter) 
    {
        double x_current = x0;
        for (int iter = 0; iter < max_iter; ++iter) 
        {
            double x_next = g(x_current);
            if (std::abs(x_next - x_current) < tol) return x_next;
            x_current = x_next;
        }
        throw std::runtime_error("Fixed-Point Iteration failed to converge.");
    }
}