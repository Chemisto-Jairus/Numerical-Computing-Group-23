#pragma once
/**
 * @file basic_ode_solver.hpp
 * @brief Low-order fixed-step ODE solvers: Euler and Heun.
 * @author Mwanje Ian (Member 6)
 */

#include <functional>
#include <utility>
#include <vector>

namespace numcomp
{
    /**
     * @brief Solves first-order initial value problems dy/dt = f(t, y), y(t0) = y0.
     *
     * - Euler: one slope per step, global error ~ h (first order).
     * - Heun (improved Euler / explicit trapezoid): two slopes per step, error ~ h^2.
     *
     * Both march from t0 to t_end with a fixed step h. The last step is shortened
     * when needed so the curve ends exactly at t_end. The stepping loop itself is
     * shared with AdvancedODESolver (see src/internal/ode_stepper.hpp).
     *
     * The class is stateless.
     *
     * @code
     * numcomp::BasicODESolver solver;
     * auto curve = solver.heun([](double t, double y) { return y; }, 0.0, 1.0, 1.0, 0.01);
     * double y_end = curve.back().second;   // close to e = 2.71828
     * @endcode
     */
    class BasicODESolver
    {
    public:
        /// Creates a solver. The class holds no state, so nothing needs to be set up.
        BasicODESolver() = default;

        /**
         * @brief Solves dy/dt = f(t, y) with the explicit Euler method.
         *
         * @details One step: y_(k+1) = y_k + h f(t_k, y_k).
         *
         * @param f      Right-hand side f(t, y).
         * @param t0     Start time.
         * @param y0     Initial value y(t0).
         * @param t_end  End time. Must be greater than t0.
         * @param h      Step size. Must be > 0.
         * @return The solution curve as (t, y) pairs, from (t0, y0) to t = t_end.
         *
         * @throws std::invalid_argument If h <= 0, t_end <= t0 (either may be NaN),
         *         or more than 1,000,000 steps would be needed.
         */
        std::vector<std::pair<double, double>> euler(std::function<double(double, double)> f, double t0, double y0, double t_end, double h);

        /**
         * @brief Solves dy/dt = f(t, y) with Heun's method (predictor-corrector).
         *
         * @details Predictor: k1 = f(t, y). Corrector: k2 = f(t + h, y + h k1).
         * Step: y_(k+1) = y_k + h/2 (k1 + k2).
         *
         * @param f      Right-hand side f(t, y).
         * @param t0     Start time.
         * @param y0     Initial value y(t0).
         * @param t_end  End time. Must be greater than t0.
         * @param h      Step size. Must be > 0.
         * @return The solution curve as (t, y) pairs, from (t0, y0) to t = t_end.
         *
         * @throws std::invalid_argument If h <= 0, t_end <= t0 (either may be NaN),
         *         or more than 1,000,000 steps would be needed.
         */
        std::vector<std::pair<double, double>> heun(std::function<double(double, double)> f, double t0, double y0, double t_end, double h);
    };
}
