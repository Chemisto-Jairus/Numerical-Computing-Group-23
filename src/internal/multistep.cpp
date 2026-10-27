/**
 * @file multistep.cpp
 * @brief Implementation of numcomp::ode::run_multistep.
 * @author Chemisto Jairus (Member 1)
 */

#include "multistep.hpp"
#include "input_checks.hpp"
#include "numerical-computing/advanced_ode_solver.hpp"

namespace numcomp
{
    namespace ode
    {
        /**
         * @brief One step of the classic RK4 method, taken from AdvancedODESolver.
         *
         * Reusing the public class keeps a single RK4 implementation in the library.
         *
         * @param f  Right-hand side f(t, y).
         * @param t  Current time.
         * @param y  Current value.
         * @param h  Step size (> 0).
         * @return y at time t + h.
         */
        static double rk4_single_step(const ODEFunction& f, double t, double y, double h)
        {
            AdvancedODESolver rk;
            return rk.rk4(f, t, y, t + h, h).back().second;
        }

        std::vector<std::pair<double, double>> run_multistep(const ODEFunction& f, double t0, double y0, double t_end, double h,
                                                             int steps, MultistepRule rule)
        {
            checks::check_ode_inputs(t0, t_end, h);

            // Points and slopes are stored once each; reserving avoids repeated reallocation.
            std::size_t expected = static_cast<std::size_t>((t_end - t0) / h) + 2;
            std::vector<std::pair<double, double>> result;
            std::vector<double> slopes;
            result.reserve(expected);
            slopes.reserve(expected);

            double t = t0;
            double y = y0;
            result.push_back({t, y});
            slopes.push_back(f(t, y));

            // Remaining time below eps counts as "arrived" (same rule as run_fixed_steps).
            double eps = 1e-9 * h;

            while (t_end - t > eps)
            {
                double step = h;
                bool last_is_short = false;
                if (t + h > t_end)
                {
                    step = t_end - t;
                    last_is_short = true;
                }

                // Starting values and a shortened last step use RK4; the rest use the rule.
                if (static_cast<int>(slopes.size()) < steps || last_is_short)
                {
                    y = rk4_single_step(f, t, y, step);
                }
                else
                {
                    y = rule(f, slopes, t, y, step);
                }
                checks::check_finite_result(y);

                t = t + step;
                if (t_end - t <= eps) t = t_end;   // snap the last time value to exactly t_end

                result.push_back({t, y});
                slopes.push_back(f(t, y));
            }

            return result;
        }
    }
}
