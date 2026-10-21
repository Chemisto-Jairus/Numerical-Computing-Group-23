#include "numerical-computing/advanced_integrator.hpp"
#include <stdexcept>
#include <vector>
#include <cmath>

namespace numcomp
{
    /// Simpson's 1/3 rule. See advanced_integrator.hpp.
    double AdvancedIntegrator::simpsons_rule(std::function<double(double)> f, double a, double b, int n)
    {
        // Check 1: Simpson pairs up the strips, so n must be positive and even
        if (n <= 0 || n % 2 != 0) throw std::invalid_argument("Requires positive even number of intervals.");

        // Check 2 (FIX): NaN or infinite limits gave "nan" with no warning
        if (!std::isfinite(a) || !std::isfinite(b)) throw std::invalid_argument("Limits a and b must be finite numbers.");

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

        // Check 3 (FIX): NaN or infinite limits gave "nan" with no warning
        if (!std::isfinite(a) || !std::isfinite(b)) throw std::invalid_argument("Limits a and b must be finite numbers.");

        std::vector<std::vector<double>> R(max_iter, std::vector<double>(max_iter, 0.0));

        // Column 0: trapezoidal rule with 1, 2, 4, 8, ... strips
        for (int i = 0; i < max_iter; ++i)
        {
            int n = 1 << i;                     // 2^i strips
            double h = (b - a) / n;
            double sum = 0.5 * (f(a) + f(b));
            for (int k = 1; k < n; ++k) sum += f(a + k * h);
            R[i][0] = sum * h;
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
