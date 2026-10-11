#include "numerical-computing/root_finding.hpp"
#include <cmath>
#include <stdexcept>

namespace numcomp 
{
    double BasicRootFinder::bisection(std::function<double(double)> f, double a, double b, double tol, int max_iter) 
    {
        if (f(a) * f(b) >= 0) 
        {
            throw std::invalid_argument("Function must have opposite signs at interval endpoints.");
        }
        
        double c = a;
        for (int iter = 0; iter < max_iter; ++iter) 
        {
            c = (a + b) / 2.0;
            if (std::abs(f(c)) < tol || (b - a) / 2.0 < tol) 
            {
                return c;
            }
            if (f(c) * f(a) < 0) 
            {
                b = c;
            } 
            else 
            {
                a = c;
            }
        }
        throw std::runtime_error("Bisection method failed to converge.");
    }

    double BasicRootFinder::newton_raphson(std::function<double(double)> f, std::function<double(double)> df, double x0, double tol, int max_iter) 
    {
        double x = x0;
        for (int iter = 0; iter < max_iter; ++iter) 
        {
            double derivative = df(x);
            if (std::abs(derivative) < 1e-12) 
            {
                throw std::runtime_error("Derivative too close to zero.");
            }
            
            double next_x = x - f(x) / derivative;
            if (std::abs(next_x - x) < tol) 
            {
                return next_x;
            }
            x = next_x;
        }
        throw std::runtime_error("Newton-Raphson failed to converge.");
    }
}