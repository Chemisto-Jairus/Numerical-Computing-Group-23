#pragma once
#include <functional>

namespace numcomp
{
    /**
     * @brief Advanced numerical integration: Simpson's 1/3 rule and Romberg integration.
     *
     * The class is stateless (it stores no data). Create one object and
     * call its methods as many times as you like.
     */
    class AdvancedIntegrator
    {
    public:
        AdvancedIntegrator() = default;

        /**
         * @brief Approximates the integral of f from a to b by fitting parabolas.
         *
         * Formula: h/3 * ( f(x_0) + 4f(x_1) + 2f(x_2) + 4f(x_3) + ... + f(x_n) )
         * Exact for polynomials up to degree 3 (cubics).
         *
         * @param f  The function to integrate.
         * @param a  Lower limit. Must be a finite number.
         * @param b  Upper limit. Must be a finite number.
         * @param n  Number of strips. Must be positive and even.
         * @return The approximate integral.
         *
         * @throws std::invalid_argument If n <= 0, n is odd, or a or b is NaN or infinite.
         */
        double simpsons_rule(std::function<double(double)> f, double a, double b, int n);

        /**
         * @brief Approximates the integral of f from a to b with Romberg integration.
         *
         * Row i uses the trapezoidal rule with 2^i strips. Richardson
         * extrapolation then combines the rows into a much better answer.
         *
         * @param f         The function to integrate.
         * @param a         Lower limit. Must be a finite number.
         * @param b         Upper limit. Must be a finite number.
         * @param max_iter  Number of rows in the Romberg table. Must be 1 to 25.
         * @return The approximate integral (bottom-right entry of the table).
         *
         * @throws std::invalid_argument If max_iter <= 0, max_iter > 25,
         *         or a or b is NaN or infinite.
         */
        double romberg_integration(std::function<double(double)> f, double a, double b, int max_iter);
    };
}
