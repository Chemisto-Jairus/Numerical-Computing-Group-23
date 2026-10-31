#pragma once
/**
 * @file advanced_ode_solver.hpp
 * @brief Higher-order fixed-step ODE solvers: Runge-Kutta 3 and Runge-Kutta 4.
 * @author Muwanguzi Samuel (Member 7)
 */

#include <functional>
#include <utility>
#include <vector>

namespace numcomp
{
    /**
     * @brief Solves first-order initial value problems dy/dt = f(t, y), y(t0) = y0,
     *        with explicit Runge-Kutta methods.
     *
     * - RK3 (Kutta's third-order method): three slopes per step, error ~ h^3.
     * - RK4 (classic fourth-order method): four slopes per step, error ~ h^4.
     *
     * Both march from t0 to t_end with a fixed step h and end exactly at t_end.
     * The stepping loop is shared with BasicODESolver (see src/internal/ode_stepper.hpp).
     *
     * The class is stateless.
     *
     * @code
     * numcomp::AdvancedODESolver solver;
     * auto curve = solver.rk4([](double t, double y) { return -2.0 * y; }, 0.0, 1.0, 1.0, 0.1);
     * double y_end = curve.back().second;   // close to e^-2 = 0.135335
     * @endcode
     */
    class AdvancedODESolver
    {
    public:
        /// Creates a solver. The class holds no state, so nothing needs to be set up.
        AdvancedODESolver() = default;

        /**
         * @brief Solves dy/dt = f(t, y) with the classic fourth-order Runge-Kutta method.
         *
         * @details k1 = f(t, y), k2 = f(t + h/2, y + h k1/2), k3 = f(t + h/2, y + h k2/2),
         * k4 = f(t + h, y + h k3); y_(k+1) = y_k + h/6 (k1 + 2 k2 + 2 k3 + k4).
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
        std::vector<std::pair<double, double>> rk4(std::function<double(double, double)> f, double t0, double y0, double t_end, double h);

        /**
         * @brief Solves dy/dt = f(t, y) with Kutta's third-order Runge-Kutta method.
         *
         * @details k1 = f(t, y), k2 = f(t + h/2, y + h k1/2), k3 = f(t + h, y - h k1 + 2 h k2);
         * y_(k+1) = y_k + h/6 (k1 + 4 k2 + k3).
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
        std::vector<std::pair<double, double>> rk3(std::function<double(double, double)> f, double t0, double y0, double t_end, double h);
    };
}
