#include "numerical-computing/interpolator.hpp"
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
        // Check 1: same number of x and y values, and at least 2 points
        if (x_vals.size() != y_vals.size() || x_vals.size() < 2)
        {
            throw std::invalid_argument("Input vectors must have the same size and at least 2 points.");
        }

        // Check 2: x values must be strictly increasing (no repeats, not unsorted).
        // FIX: "!(x[i] < x[i+1])" also catches NaN, which "x[i] >= x[i+1]" lets through.
        for (size_t i = 0; i < x_vals.size() - 1; ++i)
        {
            if (!(x_vals[i] < x_vals[i+1]))
            {
                throw std::invalid_argument("x values must be strictly increasing.");
            }
        }

        // Find the interval [x_i, x_(i+1)] that contains x
        for (size_t i = 0; i < x_vals.size() - 1; ++i)
        {
            if (x >= x_vals[i] && x <= x_vals[i+1])
            {
                double x0 = x_vals[i], x1 = x_vals[i+1];
                double y0 = y_vals[i], y1 = y_vals[i+1];

                // Straight line through (x0, y0) and (x1, y1)
                return y0 + (y1 - y0) * (x - x0) / (x1 - x0);
            }
        }

        // x was not inside any interval, so it is outside the data
        throw std::out_of_range("Target x is out of the interpolation bounds.");
    }

    /// Chebyshev nodes. See interpolator.hpp.
    std::vector<double> Interpolator::chebyshev_nodes(double a, double b, int n)
    {
        // Check 1: we need at least one node
        if (n <= 0)
        {
            throw std::invalid_argument("Number of nodes must be strictly positive.");
        }

        // Check 2 (FIX): a huge n tried to allocate gigabytes and crashed the program.
        // Same limit as the ODE solvers' step cap.
        if (n > 1000000)
        {
            throw std::invalid_argument("Number of nodes must not be more than 1,000,000.");
        }

        // Check 3: the interval must be valid (a smaller than b).
        // FIX: "!(a < b)" also catches NaN, which "a >= b" lets through.
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
