/**
 * @file advanced_integrator.cpp
 * @brief Implementation of numcomp::AdvancedIntegrator (Simpson's rule and Romberg integration).
 * @author Wasswa George Mpanga (Member 5)
 */

#include "numerical-computing/advanced_integrator.hpp"
#include "numerical-computing/basic_integrator.hpp"
#include "internal/input_checks.hpp"
#include <cmath>
#include <stdexcept>
#include <vector>

namespace numcomp
{
    double AdvancedIntegrator::simpsons_rule(std::function<double(double)> f, double a, double b, int n)
    {
        checks::check_integration_inputs(a, b, n);

        // Each parabola spans two strips, so the strips must pair up.
        if (n % 2 != 0) throw std::invalid_argument("Requires positive even number of intervals.");

        double h = (b - a) / n;
        double sum = f(a) + f(b);

        // Weights alternate 4, 2, 4, 2, ... over the inner points.
        for (int i = 1; i < n; ++i)
        {
            double x = a + i * h;
            sum += (i % 2 == 0) ? 2.0 * f(x) : 4.0 * f(x);
        }
        return sum * h / 3.0;
    }

    double AdvancedIntegrator::romberg_integration(std::function<double(double)> f, double a, double b, int max_iter)
    {
        if (max_iter <= 0) throw std::invalid_argument("max_iter must be positive.");

        // 2^i strips: the work doubles each row and 1 << i must stay within int.
        if (max_iter > 25) throw std::invalid_argument("max_iter must not be more than 25.");

        checks::check_finite_limits(a, b);

        std::vector<std::vector<double>> R(max_iter, std::vector<double>(max_iter, 0.0));

        // Column 0: trapezoidal rule with 1, 2, 4, 8, ... strips.
        BasicIntegrator trapezoid;
        for (int i = 0; i < max_iter; ++i)
        {
            R[i][0] = trapezoid.trapezoidal_rule(f, a, b, 1 << i);
        }

        // Column j removes the h^(2j) error term from column j - 1 (Richardson extrapolation).
        for (int j = 1; j < max_iter; ++j)
        {
            double factor = std::pow(4, j);
            for (int i = j; i < max_iter; ++i)
            {
                R[i][j] = (factor * R[i][j-1] - R[i-1][j-1]) / (factor - 1.0);
            }
        }
        return R[max_iter - 1][max_iter - 1];
    }
}
