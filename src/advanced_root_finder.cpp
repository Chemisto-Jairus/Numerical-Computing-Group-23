#include "numerical-computing/advanced_root_finder.hpp"
#include <cmath>
#include <stdexcept>

namespace numcomp
{
    /// Secant method. See advanced_root_finder.hpp.
    double AdvancedRootFinder::secant_method(std::function<double(double)> f, double x0, double x1, double tol, int max_iter)
    {
        // Check the settings first.
        // FIX: "!(tol > 0)" also catches NaN, which "tol <= 0" lets through.
        if (!(tol > 0) || max_iter <= 0)
        {
            throw std::invalid_argument("tol and max_iter must be positive.");
        }

        for (int iter = 0; iter < max_iter; ++iter)
        {
            double f_x0 = f(x0);
            double f_x1 = f(x1);

            // We divide by (f_x1 - f_x0), so it must not be (almost) zero
            if (std::abs(f_x1 - f_x0) < 1e-12)
            {
                throw std::runtime_error("Division by zero in Secant Method.");
            }

            // Where the line through the two points crosses zero
            double x2 = x1 - f_x1 * (x1 - x0) / (f_x1 - f_x0);
            if (std::abs(x2 - x1) < tol) return x2;

            // Move along: the two newest points are used next time
            x0 = x1;
            x1 = x2;
        }
        throw std::runtime_error("Secant method failed to converge.");
    }

    /// Fixed-point iteration. See advanced_root_finder.hpp.
    double AdvancedRootFinder::fixed_point_iteration(std::function<double(double)> g, double x0, double tol, int max_iter)
    {
        // Check the settings first ("!(tol > 0)" also catches NaN)
        if (!(tol > 0) || max_iter <= 0)
        {
            throw std::invalid_argument("tol and max_iter must be positive.");
        }

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
