/**
 * @file ode_stepper.hpp
 * @brief Internal fixed-step time-marching loop shared by all four ODE methods.
 * @author Mwanje Ian (Member 6) and Muwanguzi Samuel (Member 7)
 *
 * Euler, Heun, RK3 and RK4 differ only in the formula for a single step.
 * run_fixed_steps() performs everything they have in common: input checks,
 * storing the curve, shortening the final step and landing exactly on t_end.
 * Each method passes its formula as a StepRule function pointer.
 * The multistep methods use a separate loop (see multistep.hpp) that reuses
 * the ODEFunction type defined here.
 * Not part of the public API (lives in src/internal/).
 */

#include <functional>
#include <utility>
#include <vector>

namespace numcomp
{
    /**
     * @namespace numcomp::ode
     * @brief Shared machinery for the fixed-step ODE solvers.
     */
    namespace ode
    {
        /// Right-hand side f(t, y) of the ODE dy/dt = f(t, y).
        using ODEFunction = std::function<double(double, double)>;

        /**
         * @brief Signature of a one-step formula.
         *
         * Given f, the current time t, the current value y and the step h,
         * a step rule returns the approximation of y at time t + h.
         */
        using StepRule = double (*)(const ODEFunction& f, double t, double y, double h);

        /**
         * @brief Integrates dy/dt = f(t, y) from t0 to t_end by repeating a step rule.
         *
         * The final step is shortened when (t_end - t0) is not a multiple of h,
         * and the last time value is set to exactly t_end.
         *
         * @param f      Right-hand side f(t, y).
         * @param t0     Start time.
         * @param y0     Initial value y(t0).
         * @param t_end  End time. Must be greater than t0.
         * @param h      Step size. Must be > 0.
         * @param step   One-step formula (Euler, Heun, RK3 or RK4).
         * @return The solution curve as (t, y) pairs, starting with (t0, y0).
         * @throws std::invalid_argument From checks::check_ode_inputs().
         */
        std::vector<std::pair<double, double>> run_fixed_steps(const ODEFunction& f, double t0, double y0, double t_end, double h, StepRule step);
    }
}
