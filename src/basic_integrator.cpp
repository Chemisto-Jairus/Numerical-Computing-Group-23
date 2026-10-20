#include "numerical-computing/basic_integrator.hpp"
#include <cmath>
#include <stdexcept>

namespace numcomp
{
    /// Trapezoidal rule. See basic_integrator.hpp.
    double BasicIntegrator::trapezoidal_rule(std::function<double(double)> f, double a, double b, int n)
    {
        // Check 1: we need at least one strip
        if (n <= 0) throw std::invalid_argument("Number of intervals must be strictly positive.");

        // Check 2 (FIX): NaN or infinite limits gave "nan" with no warning
        if (!std::isfinite(a) || !std::isfinite(b)) throw std::invalid_argument("Limits a and b must be finite numbers.");

        double h = (b - a) / n;                 // width of one strip
        double sum = 0.5 * (f(a) + f(b));       // the two ends count half
        for (int i = 1; i < n; ++i)
        {
            sum += f(a + i * h);                // inner points count fully
        }
        return sum * h;
    }

    /// Midpoint rule. See basic_integrator.hpp.
    double BasicIntegrator::midpoint_rule(std::function<double(double)> f, double a, double b, int n)
    {
        // Check 1: we need at least one strip
        if (n <= 0) throw std::invalid_argument("Number of intervals must be strictly positive.");

        // Check 2 (FIX): NaN or infinite limits gave "nan" with no warning
        if (!std::isfinite(a) || !std::isfinite(b)) throw std::invalid_argument("Limits a and b must be finite numbers.");

        double h = (b - a) / n;                 // width of one strip
        double sum = 0.0;
        for (int i = 0; i < n; ++i)
        {
            double midpoint = a + (i + 0.5) * h;    // middle of strip i
            sum += f(midpoint);
        }
        return sum * h;
    }
}
