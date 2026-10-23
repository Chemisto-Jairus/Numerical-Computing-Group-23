#include "numerical-computing/advanced_ode_solver.hpp"
#include "internal/ode_stepper.hpp"

namespace numcomp
{
    // -----------------------------------------------------------------
    // Step rules: ONE step of each method. The shared loop in
    // internal/ode_stepper.cpp does the checks, the stepping and the
    // landing on t_end (the same loop Euler and Heun use).
    // "static" keeps these helpers private to this file.
    // -----------------------------------------------------------------

    /// Classic RK4: four slopes, weights 1, 2, 2, 1
    static double rk4_step(const ode::ODEFunction& f, double t, double y, double h)
    {
        double k1 = f(t, y);
        double k2 = f(t + h / 2.0, y + (h / 2.0) * k1);
        double k3 = f(t + h / 2.0, y + (h / 2.0) * k2);
        double k4 = f(t + h, y + h * k3);
        return y + (h / 6.0) * (k1 + 2.0 * k2 + 2.0 * k3 + k4);
    }

    /// Kutta's RK3: three slopes, weights 1, 4, 1
    static double rk3_step(const ode::ODEFunction& f, double t, double y, double h)
    {
        double k1 = f(t, y);
        double k2 = f(t + h / 2.0, y + (h / 2.0) * k1);
        double k3 = f(t + h, y - h * k1 + 2.0 * h * k2);
        return y + (h / 6.0) * (k1 + 4.0 * k2 + k3);
    }

    /// Classic RK4. See advanced_ode_solver.hpp.
    std::vector<std::pair<double, double>> AdvancedODESolver::rk4(std::function<double(double, double)> f, double t0, double y0, double t_end, double h)
    {
        return ode::run_fixed_steps(f, t0, y0, t_end, h, rk4_step);
    }

    /// Kutta's RK3. See advanced_ode_solver.hpp.
    std::vector<std::pair<double, double>> AdvancedODESolver::rk3(std::function<double(double, double)> f, double t0, double y0, double t_end, double h)
    {
        return ode::run_fixed_steps(f, t0, y0, t_end, h, rk3_step);
    }
}
