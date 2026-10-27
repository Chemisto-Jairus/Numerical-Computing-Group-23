#include "numerical-computing/runge_kutta_fehlberg_solver.hpp"
#include "internal/input_checks.hpp"
#include <algorithm>
#include <cmath>
#include <stdexcept>

namespace numcomp
{
    struct FehlbergStep
    {
        double y4;
        double error;
    };

    static FehlbergStep fehlberg_step(const std::function<double(double, double)>& f, double t, double y, double h)
    {
        double k1 = h * f(t, y);
        double k2 = h * f(t + h / 4.0, y + k1 / 4.0);
        double k3 = h * f(t + 3.0 * h / 8.0, y + 3.0 * k1 / 32.0 + 9.0 * k2 / 32.0);
        double k4 = h * f(t + 12.0 * h / 13.0, y + 1932.0 * k1 / 2197.0 - 7200.0 * k2 / 2197.0 + 7296.0 * k3 / 2197.0);
        double k5 = h * f(t + h, y + 439.0 * k1 / 216.0 - 8.0 * k2 + 3680.0 * k3 / 513.0 - 845.0 * k4 / 4104.0);
        double k6 = h * f(t + h / 2.0, y - 8.0 * k1 / 27.0 + 2.0 * k2 - 3544.0 * k3 / 2565.0 + 1859.0 * k4 / 4104.0 - 11.0 * k5 / 40.0);

        double y4 = y + 25.0 * k1 / 216.0 + 1408.0 * k3 / 2565.0 + 2197.0 * k4 / 4104.0 - k5 / 5.0;
        double y5 = y + 16.0 * k1 / 135.0 + 6656.0 * k3 / 12825.0 + 28561.0 * k4 / 56430.0 - 9.0 * k5 / 50.0 + 2.0 * k6 / 55.0;
        return {y4, std::abs(y5 - y4)};
    }

    std::vector<std::pair<double, double>> RungeKuttaFehlbergSolver::rkf45(std::function<double(double, double)> f, double t0, double y0,
                                                                           double t_end, double h_initial, double tol)
    {
        checks::check_ode_inputs(t0, t_end, h_initial);
        checks::check_tolerance(tol, 1);

        std::vector<std::pair<double, double>> result;
        result.push_back({t0, y0});

        double t = t0;
        double y = y0;
        double h = std::min(h_initial, t_end - t0);
        double h_max = t_end - t0;
        int attempts = 0;

        while (t < t_end)
        {
            // Every trial step counts, accepted or not, so a hopeless problem cannot loop forever.
            if (++attempts > checks::MAX_POINTS)
            {
                throw std::runtime_error("RKF45 needed more than 1,000,000 steps: increase tol.");
            }
            // A step this small no longer changes t in double precision.
            if (h < 1e-12 * std::max(1.0, std::abs(t)))
            {
                throw std::runtime_error("RKF45 step size became too small (singular problem or tol too strict).");
            }

            // Do not step past t_end.
            bool last = (t + h >= t_end);
            if (last) h = t_end - t;

            FehlbergStep step = fehlberg_step(f, t, y, h);
            checks::check_finite_result(step.y4);
            double R = step.error / h;   // estimated error per unit time

            if (R <= tol)
            {
                t = last ? t_end : t + h;   // land exactly on t_end
                y = step.y4;
                result.push_back({t, y});
            }

            // Optimal rescaling factor, limited to [0.1, 4] so h never changes abruptly.
            double factor = (R > 0.0) ? 0.84 * std::pow(tol / R, 0.25) : 4.0;
            factor = std::min(4.0, std::max(0.1, factor));
            h = std::min(h * factor, h_max);
        }

        return result;
    }

    double RungeKuttaFehlbergSolver::rkf45_value(std::function<double(double, double)> f, double t0, double y0, double t_end, double tol)
    {
        // Written as !(...) so that a NaN interval is rejected here, before h is computed from it.
        if (!(t_end > t0))
        {
            throw std::invalid_argument("Step size h must be strictly positive and t_end must be greater than t0.");
        }
        return rkf45(f, t0, y0, t_end, (t_end - t0) / 100.0, tol).back().second;
    }
}
