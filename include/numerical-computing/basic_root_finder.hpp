#pragma once
/**
 * @file basic_root_finder.hpp
 * @brief Bracketing and derivative-based root finding: Bisection and Newton-Raphson.
 * @author Chemisto Jairus (Member 1)
 */

#include <functional>

namespace numcomp
{
    /**
     * @brief Finds a root of a function f, that is, an x with f(x) = 0.
     *
     * Two classic methods are provided:
     * - Bisection: very reliable, needs an interval where f changes sign, converges linearly.
     * - Newton-Raphson: very fast near the root (quadratic convergence), needs f'(x).
     *
     * The class is stateless: it stores no data, so one object can be reused
     * for any number of calls.
     *
     * @code
     * numcomp::BasicRootFinder finder;
     * auto f  = [](double x) { return x * x - 2.0; };
     * auto df = [](double x) { return 2.0 * x; };
     * double r1 = finder.bisection(f, 0.0, 2.0, 1e-10, 100);       // 1.41421356...
     * double r2 = finder.newton_raphson(f, df, 1.0, 1e-12, 50);    // 1.41421356...
     * @endcode
     */
    class BasicRootFinder
    {
    public:
        /// Creates a root finder. The class holds no state, so nothing needs to be set up.
        BasicRootFinder() = default;

        /**
         * @brief Finds a root of f inside [a, b] by repeatedly halving the interval.
         *
         * @details f(a) and f(b) must have opposite signs, so by the intermediate value
         * theorem a root lies between them. Each step keeps the half in which the sign
         * still changes. After k steps the interval width is (b - a) / 2^k.
         * The two ends may be given in either order.
         *
         * @param f         The function whose root is wanted.
         * @param a         One end of the interval.
         * @param b         The other end of the interval.
         * @param tol       Stop when |f(c)| < tol or the half-width is below tol. Must be > 0.
         * @param max_iter  Maximum number of halvings. Must be > 0.
         * @return An x with f(x) close to 0.
         *
         * @throws std::invalid_argument If tol <= 0 or is NaN, max_iter <= 0,
         *         f(a) or f(b) is NaN, or f(a) and f(b) have the same sign.
         * @throws std::runtime_error If no root is found within max_iter halvings.
         */
        double bisection(std::function<double(double)> f, double a, double b, double tol, int max_iter);

        /**
         * @brief Finds a root of f with Newton's iteration x_new = x - f(x) / f'(x).
         *
         * @details Each step follows the tangent line at x down to the x-axis.
         * Close to a simple root the number of correct digits roughly doubles
         * every step. A poor starting guess or f'(x) close to 0 can stop it.
         *
         * @param f         The function whose root is wanted.
         * @param df        The derivative f'(x).
         * @param x0        Starting guess.
         * @param tol       Stop when two successive guesses differ by less than tol. Must be > 0.
         * @param max_iter  Maximum number of steps. Must be > 0.
         * @return An x with f(x) close to 0.
         *
         * @throws std::invalid_argument If tol <= 0 or is NaN, or max_iter <= 0.
         * @throws std::runtime_error If |f'(x)| < 1e-12 at some step, or no root is
         *         found within max_iter steps.
         */
        double newton_raphson(std::function<double(double)> f, std::function<double(double)> df, double x0, double tol, int max_iter);
    };
}
