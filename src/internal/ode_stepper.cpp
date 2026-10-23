#include "ode_stepper.hpp"
#include "input_checks.hpp"

namespace numcomp
{
    namespace ode
    {
        std::vector<std::pair<double, double>> run_fixed_steps(const ODEFunction& f, double t0, double y0, double t_end, double h, StepRule step)
        {
            // Same checks for every ODE method, written once
            checks::check_ode_inputs(t0, t_end, h);

            std::vector<std::pair<double, double>> result;
            result.push_back({t0, y0});    // store the initial condition

            double t = t0;
            double y = y0;
            double eps = 1e-9 * h;         // if less time than this is left, we are at the end

            while (t_end - t > eps)
            {
                // Prevent overshooting the target time on the final step
                if (t + h > t_end)
                {
                    h = t_end - t;
                }

                y = step(f, t, y, h);      // the only line that differs between methods
                t = t + h;
                if (t_end - t <= eps) t = t_end;   // land exactly on t_end

                result.push_back({t, y});
            }

            return result;
        }
    }
}
