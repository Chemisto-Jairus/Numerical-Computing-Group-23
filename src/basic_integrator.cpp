/**
 * @file basic_integrator.cpp
 * @brief Implementation of numcomp::BasicIntegrator (Trapezoidal and Midpoint rules).
 * @author Aogon Sharon (Member 3)
 */

#include "numerical-computing/basic_integrator.hpp"
#include "internal/input_checks.hpp"

namespace numcomp
{
    double BasicIntegrator::trapezoidal_rule(std::function<double(double)> f, double a, double b, int n)
    {
        checks::check_integration_inputs(a, b, n);

        double h = (b - a) / n;

        // End points carry weight 1/2, every inner point weight 1.
        double sum = 0.5 * (f(a) + f(b));
        for (int i = 1; i < n; ++i)
        {
            sum += f(a + i * h);
        }
        return sum * h;
    }

    double BasicIntegrator::midpoint_rule(std::function<double(double)> f, double a, double b, int n)
    {
        checks::check_integration_inputs(a, b, n);

        double h = (b - a) / n;
        double sum = 0.0;
        for (int i = 0; i < n; ++i)
        {
            // Sample each strip [a + i h, a + (i + 1) h] at its centre.
            sum += f(a + (i + 0.5) * h);
        }
        return sum * h;
    }
}
