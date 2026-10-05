#include "numerical-computing/interpolation.hpp"

namespace numcomp {
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
}