#pragma once
/**
 * @file advanced_integrator.hpp
 * @brief Higher-order integration: Simpson's 1/3 rule and Romberg integration.
 * @author Wasswa George Mpanga (Member 5)
 */

#include <functional>

namespace numcomp
{
    /**
     * @brief Approximates definite integrals with Simpson's 1/3 rule and Romberg integration.
     *
     * - Simpson's rule fits a parabola through each pair of strips. Error ~ h^4,
     *   exact for polynomials up to degree 3.
     * - Romberg integration takes trapezoidal results with 1, 2, 4, ... strips and
     *   combines them by Richardson extrapolation, removing one error term per column.
     *
     * The class is stateless. Romberg's first column is computed with
     * BasicIntegrator::trapezoidal_rule (composition, not inheritance).
     *
     * @code
     * numcomp::AdvancedIntegrator integrator;
     * double s = integrator.simpsons_rule([](double x) { return x * x * x; }, 0.0, 2.0, 2);    // exactly 4
     * double r = integrator.romberg_integration([](double x) { return std::sin(x); }, 0.0, 3.14159265358979, 8); // 2
     * @endcode
     */
    class AdvancedIntegrator
    {
    public:
        /// Creates an integrator. The class holds no state, so nothing needs to be set up.
        AdvancedIntegrator() = default;

        /**
         * @brief Integrates f from a to b by fitting a parabola through each pair of strips.
         *
         * @details Formula: h/3 * ( f(x_0) + 4f(x_1) + 2f(x_2) + 4f(x_3) + ... + 4f(x_(n-1)) + f(x_n) ).
         * Error: -(b - a) h^4 f''''(xi) / 180.
         *
         * @param f  The function to integrate.
         * @param a  Lower limit. Must be finite.
         * @param b  Upper limit. Must be finite.
         * @param n  Number of strips. Must be positive and even.
         * @return The approximate integral.
         *
         * @throws std::invalid_argument If n <= 0, n is odd, or a or b is NaN or infinite.
         */
        double simpsons_rule(std::function<double(double)> f, double a, double b, int n);

        /**
         * @brief Integrates f from a to b with Romberg integration.
         *
         * @details Row i of the table starts with the trapezoidal rule on 2^i strips:
         * R[i][0]. Each further column applies Richardson extrapolation:
         * R[i][j] = (4^j R[i][j-1] - R[i-1][j-1]) / (4^j - 1).
         * The bottom-right entry R[m-1][m-1] is returned.
         *
         * @param f         The function to integrate.
         * @param a         Lower limit. Must be finite.
         * @param b         Upper limit. Must be finite.
         * @param max_iter  Number of rows m in the table. Must be 1 to 25.
         * @return The approximate integral.
         *
         * @throws std::invalid_argument If max_iter <= 0, max_iter > 25,
         *         or a or b is NaN or infinite.
         *
         * @note The last row uses 2^(m-1) strips, so the cost doubles with every row.
         *       The cap of 25 rows also keeps 2^i inside the range of int.
         */
        double romberg_integration(std::function<double(double)> f, double a, double b, int max_iter);
    };
}
