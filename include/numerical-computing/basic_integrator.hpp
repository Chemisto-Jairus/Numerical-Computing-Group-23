#pragma once
/**
 * @file basic_integrator.hpp
 * @brief Composite Newton-Cotes integration: Trapezoidal rule and Midpoint rule.
 * @author Aogon Sharon (Member 3)
 */

#include <functional>

namespace numcomp
{
    /**
     * @brief Approximates definite integrals with the composite Trapezoidal and Midpoint rules.
     *
     * Both rules split [a, b] into n equal strips of width h = (b - a) / n.
     * Both are exact for straight lines and have error proportional to h^2,
     * so doubling n divides the error by about 4.
     * If b < a the result is negative, matching the usual sign convention.
     *
     * The class is stateless. AdvancedIntegrator reuses trapezoidal_rule()
     * for the first column of the Romberg table.
     *
     * @code
     * numcomp::BasicIntegrator integrator;
     * double t = integrator.trapezoidal_rule([](double x) { return x * x; }, 0.0, 1.0, 100);  // 0.33335
     * double m = integrator.midpoint_rule([](double x) { return x * x; }, 0.0, 1.0, 100);     // 0.333325
     * @endcode
     */
    class BasicIntegrator
    {
    public:
        /// Creates an integrator. The class holds no state, so nothing needs to be set up.
        BasicIntegrator() = default;

        /**
         * @brief Integrates f from a to b by joining the sample points with straight lines.
         *
         * @details Formula: h * ( f(a)/2 + f(x_1) + ... + f(x_(n-1)) + f(b)/2 ).
         * Error: -(b - a) h^2 f''(xi) / 12.
         *
         * @param f  The function to integrate.
         * @param a  Lower limit. Must be finite.
         * @param b  Upper limit. Must be finite.
         * @param n  Number of strips. Must be > 0.
         * @return The approximate integral.
         *
         * @throws std::invalid_argument If n <= 0, or a or b is NaN or infinite.
         */
        double trapezoidal_rule(std::function<double(double)> f, double a, double b, int n);

        /**
         * @brief Integrates f from a to b using the value at the middle of each strip.
         *
         * @details Formula: h * ( f(m_1) + f(m_2) + ... + f(m_n) ), where m_i is the
         * middle of strip i. Error: (b - a) h^2 f''(xi) / 24, about half the
         * trapezoidal error and of opposite sign. f is never evaluated at a or b.
         *
         * @param f  The function to integrate.
         * @param a  Lower limit. Must be finite.
         * @param b  Upper limit. Must be finite.
         * @param n  Number of strips. Must be > 0.
         * @return The approximate integral.
         *
         * @throws std::invalid_argument If n <= 0, or a or b is NaN or infinite.
         */
        double midpoint_rule(std::function<double(double)> f, double a, double b, int n);
    };
}
