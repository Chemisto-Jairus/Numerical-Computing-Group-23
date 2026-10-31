#pragma once
/**
 * @file adams_moulton_solver.hpp
 * @brief Implicit linear multistep ODE solvers: Adams-Moulton of order 2 and 4.
 * @author Muwanguzi Samuel (Member 7)
 */

#include <functional>
#include <utility>
#include <vector>

namespace numcomp
{
    /**
     * @brief Solves dy/dt = f(t, y), y(t0) = y0, with the implicit Adams-Moulton methods.
     *
     * Adams-Moulton formulas also use the new slope f_(n+1) = f(t_(n+1), y_(n+1)),
     * so y_(n+1) appears on both sides of the equation (the method is "implicit"):
     * - am2() (trapezoidal rule): y_(n+1) = y_n + h/2 (f_(n+1) + f_n), error ~ h^2.
     * - am4(): y_(n+1) = y_n + h/24 (9 f_(n+1) + 19 f_n - 5 f_(n-1) + f_(n-2)), error ~ h^4.
     *
     * The equation for y_(n+1) is solved by fixed-point iteration (the "corrector"),
     * starting from the explicit Euler guess y_n + h f_n (the "predictor"). The
     * iteration converges when h is small enough that h |beta df/dy| < 1, where
     * beta is the weight of f_(n+1) (1/2 or 9/24). Implicit methods have smaller
     * error constants and are more stable than the explicit Adams-Bashforth methods.
     *
     * Starting values and a shortened final step use RK4 (see src/internal/multistep.hpp).
     *
     * The class is stateless.
     *
     * @code
     * numcomp::AdamsMoultonSolver am;
     * auto curve = am.am4([](double t, double y) { return -2.0 * y; }, 0.0, 1.0, 1.0, 0.05);
     * double y_end = curve.back().second;   // close to e^-2 = 0.135335
     * @endcode
     *
     * @see AdamsBashforthSolver for the explicit members of the Adams family.
     */
    class AdamsMoultonSolver
    {
    public:
        /// Creates a solver. The class holds no state, so nothing needs to be set up.
        AdamsMoultonSolver() = default;

        /**
         * @brief Solves dy/dt = f(t, y) with the one-step Adams-Moulton method (trapezoidal rule).
         *
         * @details y_(n+1) = y_n + h/2 (f(t_(n+1), y_(n+1)) + f_n), solved for y_(n+1)
         * by fixed-point iteration.
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
         * @throws std::runtime_error If the corrector iteration does not converge
         *         (h is too large for this problem) or the solution stops being finite.
         */
        std::vector<std::pair<double, double>> am2(std::function<double(double, double)> f, double t0, double y0, double t_end, double h);

        /**
         * @brief Solves dy/dt = f(t, y) with the three-step, fourth-order Adams-Moulton method.
         *
         * @details y_(n+1) = y_n + h/24 (9 f(t_(n+1), y_(n+1)) + 19 f_n - 5 f_(n-1) + f_(n-2)),
         * solved for y_(n+1) by fixed-point iteration. The first two steps use RK4.
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
         * @throws std::runtime_error If the corrector iteration does not converge
         *         (h is too large for this problem) or the solution stops being finite.
         */
        std::vector<std::pair<double, double>> am4(std::function<double(double, double)> f, double t0, double y0, double t_end, double h);
    };
}
