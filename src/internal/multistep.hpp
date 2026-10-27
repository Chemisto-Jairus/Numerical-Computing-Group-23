#pragma once
/**
 * @file multistep.hpp
 * @brief Internal time-marching loop shared by the Adams-Bashforth and Adams-Moulton methods.
 * @author Chemisto Jairus (Member 1)
 *
 * A linear multistep method computes the next value of y from the slopes
 * f(t, y) at several earlier points. Every such method needs the same
 * surrounding work: input checks, a one-step method to create the first
 * few points (the "starting values"), a record of the slopes, a shortened
 * final step and an exact landing on t_end. run_multistep() does all of it;
 * each method only supplies its formula as a MultistepRule.
 * Not part of the public API (lives in src/internal/).
 */

#include "ode_stepper.hpp"
#include <utility>
#include <vector>

namespace numcomp
{
    namespace ode
    {
        /**
         * @brief Signature of a multistep formula.
         *
         * @p slopes holds f(t_i, y_i) for every point computed so far, so
         * slopes.back() is f at the current point (t, y). The rule returns
         * the approximation of y at time t + h.
         */
        using MultistepRule = double (*)(const ODEFunction& f, const std::vector<double>& slopes, double t, double y, double h);

        /**
         * @brief Integrates dy/dt = f(t, y) from t0 to t_end with a multistep rule.
         *
         * @details The first (steps - 1) steps use the classic fourth-order
         * Runge-Kutta method (AdvancedODESolver::rk4), because a method that
         * needs @p steps earlier slopes cannot start from y0 alone. RK4 is also
         * used for a final step that is shorter than h, because the multistep
         * formulas assume equally spaced points. Every other step uses @p rule.
         * f is evaluated once per point and the value is stored, so an explicit
         * multistep step costs a single new evaluation of f.
         *
         * @param f      Right-hand side f(t, y).
         * @param t0     Start time.
         * @param y0     Initial value y(t0).
         * @param t_end  End time. Must be greater than t0.
         * @param h      Step size. Must be > 0.
         * @param steps  Number of earlier slopes the rule uses (1 to 4).
         * @param rule   The multistep formula.
         * @return The solution curve as (t, y) pairs, starting with (t0, y0) and ending at t_end.
         * @throws std::invalid_argument From checks::check_ode_inputs().
         * @throws std::runtime_error If the solution stops being a finite number.
         */
        std::vector<std::pair<double, double>> run_multistep(const ODEFunction& f, double t0, double y0, double t_end, double h,
                                                             int steps, MultistepRule rule);
    }
}
