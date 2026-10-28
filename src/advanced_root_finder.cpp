/**
 * @file advanced_root_finder.cpp
 * @brief Implementation of numcomp::AdvancedRootFinder (Secant and Fixed-Point Iteration).
 * @author Bakalu Valeria (Member 4)
 */

#include "numerical-computing/advanced_root_finder.hpp"
#include "internal/input_checks.hpp"
#include <cmath>
#include <stdexcept>

namespace numcomp
{
    double AdvancedRootFinder::secant_method(std::function<double(double)> f, double x0, double x1, double tol, int max_iter)
    {
        checks::check_tolerance(tol, max_iter);

        for (int iter = 0; iter < max_iter; ++iter)
        {
            double f_x0 = f(x0);
            double f_x1 = f(x1);

            // Equal function values give a horizontal secant line with no zero crossing.
            if (std::abs(f_x1 - f_x0) < 1e-12)
            {
                throw std::runtime_error("Division by zero in Secant Method.");
            }

            // Zero crossing of the line through (x0, f(x0)) and (x1, f(x1)).
            double x2 = x1 - f_x1 * (x1 - x0) / (f_x1 - f_x0);
            if (std::abs(x2 - x1) < tol) return x2;

            // The two most recent points define the next secant line.
            x0 = x1;
            x1 = x2;
        }
        throw std::runtime_error("Secant method failed to converge.");
    }

    double AdvancedRootFinder::fixed_point_iteration(std::function<double(double)> g, double x0, double tol, int max_iter)
    {
        checks::check_tolerance(tol, max_iter);

        double x_current = x0;
        for (int iter = 0; iter < max_iter; ++iter)
        {
            double x_next = g(x_current);

            // Successive values agree to within tol, so x_next is (nearly) a fixed point.
            if (std::abs(x_next - x_current) < tol) return x_next;
            x_current = x_next;
        }
        throw std::runtime_error("Fixed-Point Iteration failed to converge.");
    }
}
