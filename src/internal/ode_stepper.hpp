#pragma once
#include <functional>
#include <utility>
#include <vector>

// =====================================================================
// INTERNAL HELPER: the fixed-step loop shared by all four ODE methods.
//
// Before: euler, heun, rk3 and rk4 each had their own copy of the same
// ~30-line loop (check inputs, store the first point, step, shorten the
// last step, land exactly on t_end). Only the 1-4 lines of the formula
// were different.
//
// Now the loop is written once here. Each method only gives its formula
// as a small "step rule" function.
// Not part of the public API (lives in src/internal/).
// =====================================================================

namespace numcomp
{
    namespace ode
    {
        /// The right-hand side f(t, y) of dy/dt = f(t, y).
        using ODEFunction = std::function<double(double, double)>;

        /// One step of a method: given f, t, y and h, return y at time t + h.
        using StepRule = double (*)(const ODEFunction& f, double t, double y, double h);

        /**
         * @brief Checks the inputs, then applies "step" again and again from t0 to t_end.
         *
         * The last step is shortened if needed, so the curve ends exactly at t_end.
         *
         * @param f      The right-hand side f(t, y).
         * @param t0     Start time.
         * @param y0     Value of y at t0.
         * @param t_end  End time. Must be greater than t0.
         * @param h      Step size. Must be > 0.
         * @param step   The method's formula for one step (Euler, Heun, RK3 or RK4).
         * @return The solution curve as (t, y) pairs, from (t0, y0) to t_end.
         *
         * @throws std::invalid_argument From checks::check_ode_inputs().
         */
        std::vector<std::pair<double, double>> run_fixed_steps(const ODEFunction& f, double t0, double y0, double t_end, double h, StepRule step);
    }
}
