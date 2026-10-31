#pragma once
/**
 * @file advanced_root_finder.hpp
 * @brief Derivative-free root finding: Secant method and Fixed-Point Iteration.
 * @author Bakalu Valeria (Member 4)
 */

#include <functional>

namespace numcomp
{
    /**
     * @brief Root finding methods that do not need the derivative f'(x).
     *
     * - Secant method: like Newton-Raphson, but the tangent is replaced by the
     *   line through the last two points. Converges with order about 1.618.
     * - Fixed-Point Iteration: solves x = g(x) by repeating x_new = g(x).
     *   Converges when |g'(x)| < 1 near the answer.
     *
     * The class is stateless: one object can be reused for any number of calls.
     *
     * @code
     * numcomp::AdvancedRootFinder finder;
     * double r = finder.secant_method([](double x) { return x * x - 4.0; }, 1.0, 3.0, 1e-10, 100);   // 2
     * double p = finder.fixed_point_iteration([](double x) { return std::cos(x); }, 1.0, 1e-12, 1000); // 0.739085...
     * @endcode
     */
    class AdvancedRootFinder
    {
    public:
        /// Creates a root finder. The class holds no state, so nothing needs to be set up.
        AdvancedRootFinder() = default;

        /**
         * @brief Finds a root of f from two starting guesses, without a derivative.
         *
         * @details The next guess is where the straight line through
         * (x0, f(x0)) and (x1, f(x1)) crosses zero:
         * x2 = x1 - f(x1) (x1 - x0) / (f(x1) - f(x0)).
         *
         * @param f         The function whose root is wanted.
         * @param x0        First starting guess.
         * @param x1        Second starting guess.
         * @param tol       Stop when two successive guesses differ by less than tol. Must be > 0.
         * @param max_iter  Maximum number of steps. Must be > 0.
         * @return An x with f(x) close to 0.
         *
         * @throws std::invalid_argument If tol <= 0 or is NaN, or max_iter <= 0.
         * @throws std::runtime_error If |f(x1) - f(x0)| < 1e-12 (the secant line is flat),
         *         or no root is found within max_iter steps.
         */
        double secant_method(std::function<double(double)> f, double x0, double x1, double tol, int max_iter);

        /**
         * @brief Solves x = g(x) by repeating x_new = g(x).
         *
         * @details To find a root of f, rewrite f(x) = 0 as x = g(x).
         * The iteration converges when g is a contraction near the answer (|g'(x)| < 1).
         *
         * @param g         The function in x = g(x).
         * @param x0        Starting guess.
         * @param tol       Stop when two successive values differ by less than tol. Must be > 0.
         * @param max_iter  Maximum number of steps. Must be > 0.
         * @return The fixed point x, where x = g(x).
         *
         * @throws std::invalid_argument If tol <= 0 or is NaN, or max_iter <= 0.
         * @throws std::runtime_error If no fixed point is found within max_iter steps.
         */
        double fixed_point_iteration(std::function<double(double)> g, double x0, double tol, int max_iter);
    };
}
