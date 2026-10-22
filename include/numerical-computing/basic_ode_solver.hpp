#pragma once
#include <vector>
#include <functional>
#include <utility>

namespace numcomp
{
    /**
     * @brief Basic ODE solvers for dy/dt = f(t, y): Euler and Heun.
     *
     * Both methods use a fixed step h from t0 to t_end. The last step is
     * shortened if needed, so the curve always ends exactly at t_end.
     *
     * The class is stateless (it stores no data). Create one object and
     * call its methods as many times as you like.
     */
    class BasicODESolver
    {
    public:
        BasicODESolver() = default;

        /**
         * @brief Solves dy/dt = f(t, y) with Euler's method: y_new = y + h * f(t, y).
         *
         * First-order: halving h roughly halves the error.
         *
         * @param f      The right-hand side f(t, y).
         * @param t0     Start time.
         * @param y0     Value of y at t0.
         * @param t_end  End time. Must be greater than t0.
         * @param h      Step size. Must be > 0.
         * @return The solution curve as (t, y) pairs, from (t0, y0) to t_end.
         *
         * @throws std::invalid_argument If h <= 0, t_end <= t0 (or any of them is NaN),
         *         or the run would need more than 1,000,000 steps.
         */
        std::vector<std::pair<double, double>> euler(std::function<double(double, double)> f, double t0, double y0, double t_end, double h);

        /**
         * @brief Solves dy/dt = f(t, y) with Heun's method (improved Euler).
         *
         * Predicts with an Euler step, then averages the start and end slopes.
         * Second-order: halving h divides the error by about 4.
         *
         * @param f      The right-hand side f(t, y).
         * @param t0     Start time.
         * @param y0     Value of y at t0.
         * @param t_end  End time. Must be greater than t0.
         * @param h      Step size. Must be > 0.
         * @return The solution curve as (t, y) pairs, from (t0, y0) to t_end.
         *
         * @throws std::invalid_argument If h <= 0, t_end <= t0 (or any of them is NaN),
         *         or the run would need more than 1,000,000 steps.
         */
        std::vector<std::pair<double, double>> heun(std::function<double(double, double)> f, double t0, double y0, double t_end, double h);
    };
}
