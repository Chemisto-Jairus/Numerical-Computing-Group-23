#pragma once
#include <functional>

namespace numcomp
{
    /**
     * @brief Advanced root finding: Secant method and Fixed-Point Iteration.
     *
     * The class is stateless (it stores no data). Create one object and
     * call its methods as many times as you like.
     */
    class AdvancedRootFinder
    {
    public:
        AdvancedRootFinder() = default;

        /**
         * @brief Finds a root of f using two starting guesses and no derivative.
         *
         * Each step draws a straight line through the last two points
         * and uses where it crosses zero as the next guess.
         *
         * @param f         The function whose root we want.
         * @param x0        First starting guess.
         * @param x1        Second starting guess.
         * @param tol       Stop when two guesses differ by less than tol. Must be > 0.
         * @param max_iter  Maximum number of steps. Must be > 0.
         * @return An x with f(x) close to 0.
         *
         * @throws std::invalid_argument If tol <= 0 or max_iter <= 0.
         * @throws std::runtime_error If f(x1) - f(x0) is (almost) zero (division by zero),
         *         or no root is found within max_iter steps.
         */
        double secant_method(std::function<double(double)> f, double x0, double x1, double tol, int max_iter);

        /**
         * @brief Solves x = g(x) by repeating x_new = g(x).
         *
         * Only converges when g does not change too fast near the answer (|g'(x)| < 1).
         *
         * @param g         The function in x = g(x).
         * @param x0        Starting guess.
         * @param tol       Stop when two guesses differ by less than tol. Must be > 0.
         * @param max_iter  Maximum number of steps. Must be > 0.
         * @return The fixed point x, where x = g(x).
         *
         * @throws std::invalid_argument If tol <= 0 or max_iter <= 0.
         * @throws std::runtime_error If no fixed point is found within max_iter steps.
         */
        double fixed_point_iteration(std::function<double(double)> g, double x0, double tol, int max_iter);
    };
}
