#pragma once
#include <functional>

namespace numcomp
{
    /**
     * @brief Basic root finding: Bisection and Newton-Raphson.
     *
     * A root is an x where f(x) = 0.
     * The class is stateless (it stores no data). Create one object and
     * call its methods as many times as you like.
     */
    class BasicRootFinder
    {
    public:
        BasicRootFinder() = default;

        /**
         * @brief Finds a root of f inside [a, b] by halving the interval again and again.
         *
         * f(a) and f(b) must have opposite signs, so a root lies between them.
         * a and b may be given in either order.
         *
         * @param f         The function whose root we want.
         * @param a         One end of the interval.
         * @param b         The other end of the interval.
         * @param tol       Stop when |f(c)| < tol or the half-width is below tol. Must be > 0.
         * @param max_iter  Maximum number of halvings. Must be > 0.
         * @return An x with f(x) close to 0.
         *
         * @throws std::invalid_argument If tol <= 0, max_iter <= 0, f(a) or f(b) is NaN,
         *         or f(a) and f(b) have the same sign.
         * @throws std::runtime_error If no root is found within max_iter halvings.
         */
        double bisection(std::function<double(double)> f, double a, double b, double tol, int max_iter);

        /**
         * @brief Finds a root of f using its derivative: x_new = x - f(x) / f'(x).
         *
         * @param f         The function whose root we want.
         * @param df        The derivative f'(x).
         * @param x0        Starting guess.
         * @param tol       Stop when two guesses differ by less than tol. Must be > 0.
         * @param max_iter  Maximum number of steps. Must be > 0.
         * @return An x with f(x) close to 0.
         *
         * @throws std::invalid_argument If tol <= 0 or max_iter <= 0.
         * @throws std::runtime_error If f'(x) is (almost) zero, or no root is found
         *         within max_iter steps.
         */
        double newton_raphson(std::function<double(double)> f, std::function<double(double)> df, double x0, double tol, int max_iter);
    };
}
