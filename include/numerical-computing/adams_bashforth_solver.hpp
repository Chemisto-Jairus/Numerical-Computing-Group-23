#pragma once
/**
 * @file adams_bashforth_solver.hpp
 * @brief Explicit linear multistep ODE solvers: Adams-Bashforth of order 2 and 4.
 * @author Mwanje Ian (Member 6)
 */

#include <functional>
#include <utility>
#include <vector>

namespace numcomp
{
    /**
     * @brief Solves dy/dt = f(t, y), y(t0) = y0, with the explicit Adams-Bashforth methods.
     *
     * A one-step method such as RK4 throws away its slopes after every step.
     * Adams-Bashforth keeps the slopes f_k = f(t_k, y_k) of the last few points
     * and fits a polynomial through them, so every new step needs only one new
     * evaluation of f:
     * - ab2(): y_(n+1) = y_n + h/2 (3 f_n - f_(n-1)), error ~ h^2.
     * - ab4(): y_(n+1) = y_n + h/24 (55 f_n - 59 f_(n-1) + 37 f_(n-2) - 9 f_(n-3)), error ~ h^4.
     *
     * The missing earlier points at the start are produced with RK4, and a final
     * step shorter than h also uses RK4 (see src/internal/multistep.hpp).
     *
     * The class is stateless.
     *
     * @code
     * numcomp::AdamsBashforthSolver ab;
     * auto curve = ab.ab4([](double t, double y) { return -2.0 * y; }, 0.0, 1.0, 1.0, 0.05);
     * double y_end = curve.back().second;   // close to e^-2 = 0.135335
     * @endcode
     *
     * @see AdamsMoultonSolver for the implicit (more stable) members of the Adams family.
     */
    class AdamsBashforthSolver
    {
    public:
        /// Creates a solver. The class holds no state, so nothing needs to be set up.
        AdamsBashforthSolver() = default;

        /**
         * @brief Solves dy/dt = f(t, y) with the two-step Adams-Bashforth method.
         *
         * @details y_(n+1) = y_n + h/2 (3 f_n - f_(n-1)). The first step uses RK4.
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
         * @throws std::runtime_error If the solution stops being a finite number.
         */
        std::vector<std::pair<double, double>> ab2(std::function<double(double, double)> f, double t0, double y0, double t_end, double h);

        /**
         * @brief Solves dy/dt = f(t, y) with the four-step Adams-Bashforth method.
         *
         * @details y_(n+1) = y_n + h/24 (55 f_n - 59 f_(n-1) + 37 f_(n-2) - 9 f_(n-3)).
         * The first three steps use RK4, which has the same order of accuracy.
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
         * @throws std::runtime_error If the solution stops being a finite number.
         */
        std::vector<std::pair<double, double>> ab4(std::function<double(double, double)> f, double t0, double y0, double t_end, double h);
    };
}
