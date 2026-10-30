#pragma once
/**
 * @file runge_kutta_fehlberg_solver.hpp
 * @brief Adaptive step-size ODE solver: the Runge-Kutta-Fehlberg method (RKF45).
 * @author Wasswa George Mpanga (Member 5)
 */

#include <functional>
#include <utility>
#include <vector>

namespace numcomp
{
    /**
     * @brief Solves dy/dt = f(t, y), y(t0) = y0, choosing the step size automatically.
     *
     * Each RKF45 step evaluates six slopes and combines them in two ways: a
     * fourth-order and a fifth-order estimate of y(t + h). Their difference
     * estimates the error of the step. A step whose error per unit time is
     * within the tolerance is accepted; otherwise it is repeated with a smaller h.
     * After every step h is rescaled by 0.84 (tol / R)^(1/4), where R is the
     * estimated error per unit time, so h grows where the solution is smooth
     * and shrinks where it changes quickly.
     *
     * This is the same "estimate the error, then refine" idea that Romberg
     * integration uses, applied to ODEs.
     *
     * The class is stateless.
     *
     * @code
     * numcomp::RungeKuttaFehlbergSolver rkf;
     * auto curve = rkf.rkf45([](double t, double y) { return -2.0 * y; }, 0.0, 1.0, 1.0, 0.1, 1e-8);
     * double y_end = curve.back().second;   // close to e^-2 = 0.135335
     * @endcode
     */
    class RungeKuttaFehlbergSolver
    {
    public:
        /// Creates a solver. The class holds no state, so nothing needs to be set up.
        RungeKuttaFehlbergSolver() = default;

        /**
         * @brief Solves dy/dt = f(t, y) from t0 to t_end with adaptive RKF45 steps.
         *
         * @details k1 = h f(t, y), ..., k6 = h f(t + h/2, ...) (Fehlberg's coefficients).
         * y4 = y + 25/216 k1 + 1408/2565 k3 + 2197/4104 k4 - 1/5 k5 (order 4) is kept;
         * y5 = y + 16/135 k1 + 6656/12825 k3 + 28561/56430 k4 - 9/50 k5 + 2/55 k6 (order 5)
         * is only used for the error estimate R = |y5 - y4| / h. The step is accepted when R <= tol.
         *
         * @param f          Right-hand side f(t, y).
         * @param t0         Start time.
         * @param y0         Initial value y(t0).
         * @param t_end      End time. Must be greater than t0.
         * @param h_initial  First step size to try. Must be > 0.
         * @param tol        Allowed error per unit time. Must be > 0.
         * @return The accepted points as (t, y) pairs, from (t0, y0) to t = t_end.
         *         The spacing of the points shows where the method needed small steps.
         *
         * @throws std::invalid_argument If h_initial <= 0, t_end <= t0, tol <= 0
         *         (any of them may be NaN), or (t_end - t0) / h_initial exceeds 1,000,000.
         * @throws std::runtime_error If the step size has to shrink below about 1e-12 of t,
         *         more than 1,000,000 steps are needed, or the solution is not finite.
         */
        std::vector<std::pair<double, double>> rkf45(std::function<double(double, double)> f, double t0, double y0,
                                                     double t_end, double h_initial, double tol);

        /**
         * @brief Returns only y(t_end), computed with rkf45() from a first step of (t_end - t0) / 100.
         *
         * @param f      Right-hand side f(t, y).
         * @param t0     Start time.
         * @param y0     Initial value y(t0).
         * @param t_end  End time. Must be greater than t0.
         * @param tol    Allowed error per unit time. Must be > 0.
         * @return The approximation of y(t_end).
         *
         * @throws std::invalid_argument If t_end <= t0 or tol <= 0 (either may be NaN).
         * @throws std::runtime_error For the same reasons as rkf45().
         */
        double rkf45_value(std::function<double(double, double)> f, double t0, double y0, double t_end, double tol);
    };
}
