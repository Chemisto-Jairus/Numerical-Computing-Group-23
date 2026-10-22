#pragma once
#include <vector>
#include <functional>
#include <utility>

namespace numcomp
{
    /**
     * @brief Advanced ODE solvers for dy/dt = f(t, y): Runge-Kutta 3 and 4.
     *
     * Both methods use a fixed step h from t0 to t_end. The last step is
     * shortened if needed, so the curve always ends exactly at t_end.
     *
     * The class is stateless (it stores no data). Create one object and
     * call its methods as many times as you like.
     */
    class AdvancedODESolver
    {
    public:
        AdvancedODESolver() = default;

        /**
         * @brief Solves dy/dt = f(t, y) with the classic 4th-order Runge-Kutta method.
         *
         * Uses four slopes per step: y_new = y + h/6 * (k1 + 2k2 + 2k3 + k4).
         * Fourth-order: halving h divides the error by about 16.
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
        std::vector<std::pair<double, double>> rk4(std::function<double(double, double)> f, double t0, double y0, double t_end, double h);

        /**
         * @brief Solves dy/dt = f(t, y) with Kutta's 3rd-order Runge-Kutta method.
         *
         * Uses three slopes per step: y_new = y + h/6 * (k1 + 4k2 + k3).
         * Third-order: halving h divides the error by about 8.
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
        std::vector<std::pair<double, double>> rk3(std::function<double(double, double)> f, double t0, double y0, double t_end, double h);
    };
}
