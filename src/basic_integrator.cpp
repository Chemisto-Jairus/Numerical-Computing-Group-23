#include "numerical-computing/basic_integrator.hpp"
#include "internal/input_checks.hpp"

namespace numcomp
{
    /// Trapezoidal rule. See basic_integrator.hpp.
    /// AdvancedIntegrator::romberg_integration reuses this method for its first column.
    double BasicIntegrator::trapezoidal_rule(std::function<double(double)> f, double a, double b, int n)
    {
        checks::check_integration_inputs(a, b, n);   // shared helper: n > 0, finite a and b

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
        checks::check_integration_inputs(a, b, n);   // shared helper

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
