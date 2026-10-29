/**
 * @file ode_stepper.cpp
 * @brief Implementation of numcomp::ode::run_fixed_steps.
 * @author Mwanje Ian (Member 6) and Muwanguzi Samuel (Member 7)
 */

#include "ode_stepper.hpp"
#include "input_checks.hpp"

namespace numcomp
{
    namespace ode
    {
        std::vector<std::pair<double, double>> run_fixed_steps(const ODEFunction& f, double t0, double y0, double t_end, double h, StepRule step)
        {
            checks::check_ode_inputs(t0, t_end, h);

            // One (t, y) pair per step plus the start point; reserving the memory
            // once avoids repeated reallocation of a long curve.
            std::vector<std::pair<double, double>> result;
            result.reserve(static_cast<std::size_t>((t_end - t0) / h) + 2);
            result.push_back({t0, y0});

            double t = t0;
            double y = y0;

            // Remaining time below eps counts as "arrived": avoids a tiny extra step
            // caused by floating-point rounding in t + h + h + ...
            double eps = 1e-9 * h;

            while (t_end - t > eps)
            {
                // Shorten the final step so it does not overshoot t_end.
                if (t + h > t_end)
                {
                    h = t_end - t;
                }

                y = step(f, t, y, h);
                t = t + h;

                // Snap the last time value to exactly t_end.
                if (t_end - t <= eps) t = t_end;

                result.push_back({t, y});
            }

            return result;
        }
    }
}
