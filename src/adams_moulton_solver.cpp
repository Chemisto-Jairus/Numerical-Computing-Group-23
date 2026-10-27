#include "numerical-computing/adams_moulton_solver.hpp"
#include "internal/input_checks.hpp"
#include "internal/multistep.hpp"
#include <cmath>
#include <stdexcept>

namespace numcomp
{
    static const int MAX_CORRECTOR_ITERATIONS = 100;

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

    static double am2_step(const ode::ODEFunction& f, const std::vector<double>& slopes, double t, double y, double h)
    {
        double fn = slopes.back();
        double known = y + (h / 2.0) * fn;
        return solve_corrector(f, t + h, known, h / 2.0, y + h * fn);
    }

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
