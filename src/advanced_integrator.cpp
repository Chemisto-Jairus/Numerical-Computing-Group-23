#include "numerical-computing/advanced_integrator.hpp"
#include "numerical-computing/basic_integrator.hpp"   // Romberg reuses the trapezoidal rule
#include "internal/input_checks.hpp"
#include <stdexcept>
#include <vector>
#include <cmath>

namespace numcomp
{
    /// Simpson's 1/3 rule. See advanced_integrator.hpp.
    double AdvancedIntegrator::simpsons_rule(std::function<double(double)> f, double a, double b, int n)
    {
        checks::check_integration_inputs(a, b, n);   // shared helper: n > 0, finite a and b

        // Simpson pairs up the strips, so n must also be even
        if (n % 2 != 0) throw std::invalid_argument("Requires positive even number of intervals.");

        double h = (b - a) / n;
        double sum = f(a) + f(b);

        for (int i = 1; i < n; ++i)
        {
            double x = a + i * h;
            // Odd points have weight 4, even inner points have weight 2
            sum += (i % 2 == 0) ? 2.0 * f(x) : 4.0 * f(x);
        }
        return sum * h / 3.0;
    }

    /// Romberg integration. See advanced_integrator.hpp.
    double AdvancedIntegrator::romberg_integration(std::function<double(double)> f, double a, double b, int max_iter)
    {
        // Check 1: we need at least one row
        if (max_iter <= 0) throw std::invalid_argument("max_iter must be positive.");

        // Check 2: 1 << i overflows an int at 32, and the work doubles every row
        if (max_iter > 25) throw std::invalid_argument("max_iter must not be more than 25.");

        checks::check_finite_limits(a, b);   // shared helper

        std::vector<std::vector<double>> R(max_iter, std::vector<double>(max_iter, 0.0));

        // Column 0: trapezoidal rule with 1, 2, 4, 8, ... strips.
        // HELPER REUSE: instead of copying the trapezoid loop here,
        // call Sharon's BasicIntegrator::trapezoidal_rule.
        BasicIntegrator trapezoid;
        for (int i = 0; i < max_iter; ++i)
        {
            R[i][0] = trapezoid.trapezoidal_rule(f, a, b, 1 << i);   // 2^i strips
        }

        // Other columns: Richardson extrapolation
        for (int j = 1; j < max_iter; ++j)
        {
            for (int i = j; i < max_iter; ++i)
            {
                double factor = std::pow(4, j);
                R[i][j] = (factor * R[i][j-1] - R[i-1][j-1]) / (factor - 1.0);
            }
        }
        return R[max_iter - 1][max_iter - 1];
    }
}
