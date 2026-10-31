/**
 * @file adams_moulton_solver.cpp
 * @brief Implementation of numcomp::AdamsMoultonSolver (AM2 and AM4).
 * @author Muwanguzi Samuel (Member 7)
 *
 * Both formulas have the form y_(n+1) = known + beta h f(t_(n+1), y_(n+1)).
 * solve_corrector() finds y_(n+1) for either of them, and ode::run_multistep()
 * provides the starting values, stored slopes and input checks.
 */

#include "numerical-computing/adams_moulton_solver.hpp"
#include "internal/input_checks.hpp"
#include "internal/multistep.hpp"
#include <cmath>
#include <stdexcept>

namespace numcomp
{
    /// Largest number of corrector iterations per step.
    static const int MAX_CORRECTOR_ITERATIONS = 100;

    /**
     * @brief Solves y = known + beta_h f(t_next, y) for y by fixed-point iteration.
     *
     * Starts from @p guess and repeats y = known + beta_h f(t_next, y) until two
     * successive values agree to about 1e-12 relative to |y|.
     *
     * @param f       Right-hand side f(t, y).
     * @param t_next  Time of the new point, t_(n+1).
     * @param known   The part of the formula that does not depend on y_(n+1).
     * @param beta_h  Weight of f_(n+1) multiplied by h.
     * @param guess   Starting value (the predictor).
     * @return y_(n+1).
     * @throws std::runtime_error If the iteration does not converge within
     *         MAX_CORRECTOR_ITERATIONS, or produces a value that is not finite.
     */
    static double solve_corrector(const ode::ODEFunction& f, double t_next, double known, double beta_h, double guess)
    {
        double y = guess;
        for (int i = 0; i < MAX_CORRECTOR_ITERATIONS; ++i)
        {
            double y_new = known + beta_h * f(t_next, y);
            checks::check_finite_result(y_new);
            if (std::abs(y_new - y) <= 1e-12 * (1.0 + std::abs(y_new)))
            {
                return y_new;
            }
            y = y_new;
        }
        throw std::runtime_error("Adams-Moulton corrector did not converge: use a smaller step size h.");
    }

    /**
     * @brief One AM2 (trapezoidal) step.
     * @param f       Right-hand side f(t, y).
     * @param slopes  Slopes at all earlier points; slopes.back() is f_n.
     * @param t       Current time t_n.
     * @param y       Current value y_n.
     * @param h       Step size.
     * @return y_(n+1).
     */
    static double am2_step(const ode::ODEFunction& f, const std::vector<double>& slopes, double t, double y, double h)
    {
        double fn = slopes.back();
        double known = y + (h / 2.0) * fn;
        return solve_corrector(f, t + h, known, h / 2.0, y + h * fn);
    }

    /**
     * @brief One AM4 step (three-step Adams-Moulton).
     * @param f       Right-hand side f(t, y).
     * @param slopes  Slopes at all earlier points; slopes.back() is f_n.
     * @param t       Current time t_n.
     * @param y       Current value y_n.
     * @param h       Step size.
     * @return y_(n+1).
     */
    static double am4_step(const ode::ODEFunction& f, const std::vector<double>& slopes, double t, double y, double h)
    {
        std::size_t n = slopes.size() - 1;   // index of the current point
        double known = y + (h / 24.0) * (19.0 * slopes[n] - 5.0 * slopes[n-1] + slopes[n-2]);
        return solve_corrector(f, t + h, known, 9.0 * h / 24.0, y + h * slopes[n]);
    }

    std::vector<std::pair<double, double>> AdamsMoultonSolver::am2(std::function<double(double, double)> f, double t0, double y0, double t_end, double h)
    {
        return ode::run_multistep(f, t0, y0, t_end, h, 1, am2_step);
    }

    std::vector<std::pair<double, double>> AdamsMoultonSolver::am4(std::function<double(double, double)> f, double t0, double y0, double t_end, double h)
    {
        return ode::run_multistep(f, t0, y0, t_end, h, 3, am4_step);
    }
}
