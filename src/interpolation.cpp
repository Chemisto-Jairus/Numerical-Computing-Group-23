#include "numerical-computing/interpolation.hpp"
#include <cmath>
#include <stdexcept>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

namespace numcomp 
{
    double linear_interpolate(double x, const std::vector<double>& x_vals, const std::vector<double>& y_vals) 
    {
        if (x_vals.size() != y_vals.size() || x_vals.size() < 2) 
        {
            throw std::invalid_argument("Input vectors must have the same size and at least 2 points.");
        }

        for (size_t i = 0; i < x_vals.size() - 1; ++i) 
        {
            if (x >= x_vals[i] && x <= x_vals[i+1]) 
            {
                double x0 = x_vals[i], x1 = x_vals[i+1];
                double y0 = y_vals[i], y1 = y_vals[i+1];
                return y0 + (y1 - y0) * (x - x0) / (x1 - x0);
            }
        }
        throw std::out_of_range("Target x is out of the interpolation bounds.");
    }

    std::vector<double> chebyshev_nodes(double a, double b, int n) 
    {
        if (n <= 0) 
        {
            throw std::invalid_argument("Number of nodes must be strictly positive.");
        }
        
        std::vector<double> nodes(n);
        for (int k = 1; k <= n; ++k) 
        {
            // Formula for Chebyshev nodes on the interval [a, b]
            nodes[k-1] = 0.5 * (a + b) + 0.5 * (b - a) * std::cos((2.0 * k - 1.0) * M_PI / (2.0 * n));
        }
        return nodes;
    }
}