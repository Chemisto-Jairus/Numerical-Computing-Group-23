#pragma once
#include <vector>
#include <stdexcept>

namespace numcomp
{
    
    double linear_interpolate(double x, const std::vector<double>& x_vals, const std::vector<double>& y_vals);
    std::vector<double> chebyshev_nodes(double a, double b, int n);
}