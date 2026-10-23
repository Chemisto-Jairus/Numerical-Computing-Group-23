#include "numerical-computing/interpolator.hpp"
#include "internal/input_checks.hpp"
#include <cmath>
#include <stdexcept>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

namespace numcomp
{
    /// Linear interpolation. See interpolator.hpp.
    double Interpolator::linear_interpolate(double x, const std::vector<double>& x_vals, const std::vector<double>& y_vals)
    {
        checks::check_data_points(x_vals, y_vals);          // shared with CubicSpline
        std::size_t i = checks::find_interval(x, x_vals);   // shared with CubicSpline

        double x0 = x_vals[i], x1 = x_vals[i+1];
        double y0 = y_vals[i], y1 = y_vals[i+1];

        // Straight line through (x0, y0) and (x1, y1)
        return y0 + (y1 - y0) * (x - x0) / (x1 - x0);
    }

    /// Chebyshev nodes. See interpolator.hpp.
    std::vector<double> Interpolator::chebyshev_nodes(double a, double b, int n)
    {
        // Check 1: we need at least one node
        if (n <= 0)
        {
            throw std::invalid_argument("Number of nodes must be strictly positive.");
        }

        // Check 2: same memory limit as the ODE solvers (shared constant)
        if (n > checks::MAX_POINTS)
        {
            throw std::invalid_argument("Number of nodes must not be more than 1,000,000.");
        }

        // Check 3: a smaller than b ("!(a < b)" also catches NaN)
        if (!(a < b))
        {
            throw std::invalid_argument("a must be less than b.");
        }

        std::vector<double> nodes(n);
        for (int k = 1; k <= n; ++k)
        {
            // Map the node from [-1, 1] onto [a, b]
            nodes[k-1] = 0.5 * (a + b) + 0.5 * (b - a) * std::cos((2.0 * k - 1.0) * M_PI / (2.0 * n));
        }
        return nodes;
    }
}
