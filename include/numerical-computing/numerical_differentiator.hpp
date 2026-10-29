#pragma once
/**
 * @file numerical_differentiator.hpp
 * @brief Finite difference formulas for first and second derivatives.
 * @author Chemisto Jairus (Member 1)
 */

#include <functional>

namespace numcomp
{
    /**
     * @brief Approximates derivatives of a function from a few of its values.
     *
     * Every formula replaces the limit in the definition of the derivative with
     * a small, fixed step h:
     * - forward_difference() and backward_difference(): one-sided, error ~ h.
     * - central_difference(): symmetric, error ~ h^2.
     * - second_derivative(): symmetric formula for f''(x), error ~ h^2.
     * - richardson_extrapolation(): combines two central differences, error ~ h^4.
     *
     * Choosing h is a balance: a large h gives a large truncation error, and a
     * very small h loses digits to rounding, because f(x + h) and f(x - h) become
     * almost equal. h around 1e-5 suits the first-derivative formulas in double precision.
     *
     * The class is stateless.
     *
     * @code
     * numcomp::NumericalDifferentiator diff;
     * double d = diff.central_difference([](double x) { return x * x * x; }, 2.0, 1e-4);   // close to 12
     * @endcode
     */
    class NumericalDifferentiator
    {
    public:
        /// Creates a differentiator. The class holds no state, so nothing needs to be set up.
        NumericalDifferentiator() = default;

        /**
         * @brief Forward difference: f'(x) ~ (f(x + h) - f(x)) / h.
         *
         * @details Taylor's theorem gives an error of (h/2) f''(xi), so the method is first order.
         * It only needs f at x and to the right of x, which is useful at a left boundary.
         *
         * @param f  The function to differentiate.
         * @param x  Point where the derivative is wanted.
         * @param h  Step size. Must be > 0.
         * @return The approximation of f'(x).
         * @throws std::invalid_argument If x is not finite, or h is not a positive finite number.
         * @throws std::runtime_error If f returns a value that is not a finite number.
         */
        double forward_difference(std::function<double(double)> f, double x, double h);

        /**
         * @brief Backward difference: f'(x) ~ (f(x) - f(x - h)) / h.
         *
         * @details First order, like the forward difference, but it uses values to the
         * left of x, which is useful at a right boundary.
         *
         * @param f  The function to differentiate.
         * @param x  Point where the derivative is wanted.
         * @param h  Step size. Must be > 0.
         * @return The approximation of f'(x).
         * @throws std::invalid_argument If x is not finite, or h is not a positive finite number.
         * @throws std::runtime_error If f returns a value that is not a finite number.
         */
        double backward_difference(std::function<double(double)> f, double x, double h);

        /**
         * @brief Central difference: f'(x) ~ (f(x + h) - f(x - h)) / (2h).
         *
         * @details The h^2 terms of the two Taylor series cancel, leaving an error of
         * (h^2/6) f'''(xi): second order for the same two function evaluations.
         *
         * @param f  The function to differentiate.
         * @param x  Point where the derivative is wanted.
         * @param h  Step size. Must be > 0.
         * @return The approximation of f'(x).
         * @throws std::invalid_argument If x is not finite, or h is not a positive finite number.
         * @throws std::runtime_error If f returns a value that is not a finite number.
         */
        double central_difference(std::function<double(double)> f, double x, double h);

        /**
         * @brief Second derivative: f''(x) ~ (f(x + h) - 2 f(x) + f(x - h)) / h^2.
         *
         * @details Error (h^2/12) f''''(xi), second order. Because of the division by h^2,
         * rounding errors grow fast for small h; h around 1e-3 to 1e-4 works well.
         *
         * @param f  The function to differentiate twice.
         * @param x  Point where the second derivative is wanted.
         * @param h  Step size. Must be > 0.
         * @return The approximation of f''(x).
         * @throws std::invalid_argument If x is not finite, or h is not a positive finite number.
         * @throws std::runtime_error If f returns a value that is not a finite number.
         */
        double second_derivative(std::function<double(double)> f, double x, double h);

        /**
         * @brief Richardson extrapolation of the central difference: error ~ h^4.
         *
         * @details With D(h) = central_difference(f, x, h), the leading error terms of
         * D(h) and D(h/2) cancel in (4 D(h/2) - D(h)) / 3. This is the same idea that
         * Romberg integration applies to the trapezoidal rule.
         *
         * @param f  The function to differentiate.
         * @param x  Point where the derivative is wanted.
         * @param h  Step size of the coarser central difference. Must be > 0.
         * @return The approximation of f'(x).
         * @throws std::invalid_argument If x is not finite, or h is not a positive finite number.
         * @throws std::runtime_error If f returns a value that is not a finite number.
         */
        double richardson_extrapolation(std::function<double(double)> f, double x, double h);
    };
}
