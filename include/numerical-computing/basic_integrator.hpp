#pragma once
#include <functional>

namespace numcomp
{
    /**
     * @brief Basic numerical integration: Trapezoidal rule and Midpoint rule.
     *
     * Both methods split [a, b] into n equal strips of width h = (b - a) / n.
     * If b < a the answer is negative, just like a normal integral.
     *
     * The class is stateless (it stores no data). Create one object and
     * call its methods as many times as you like.
     */
    class BasicIntegrator
    {
    public:
        BasicIntegrator() = default;

        /**
         * @brief Approximates the integral of f from a to b with trapezoids.
         *
         * Formula: h * ( f(a)/2 + f(x_1) + ... + f(x_(n-1)) + f(b)/2 )
         * Exact for straight lines.
         *
         * @param f  The function to integrate.
         * @param a  Lower limit. Must be a finite number.
         * @param b  Upper limit. Must be a finite number.
         * @param n  Number of strips. Must be > 0.
         * @return The approximate integral.
         *
         * @throws std::invalid_argument If n <= 0, or a or b is NaN or infinite.
         */
        double trapezoidal_rule(std::function<double(double)> f, double a, double b, int n);

        /**
         * @brief Approximates the integral of f from a to b using the middle of each strip.
         *
         * Formula: h * ( f(m_1) + f(m_2) + ... + f(m_n) ), where m_i is the middle of strip i.
         * Exact for straight lines.
         *
         * @param f  The function to integrate.
         * @param a  Lower limit. Must be a finite number.
         * @param b  Upper limit. Must be a finite number.
         * @param n  Number of strips. Must be > 0.
         * @return The approximate integral.
         *
         * @throws std::invalid_argument If n <= 0, or a or b is NaN or infinite.
         */
        double midpoint_rule(std::function<double(double)> f, double a, double b, int n);
    };
}
