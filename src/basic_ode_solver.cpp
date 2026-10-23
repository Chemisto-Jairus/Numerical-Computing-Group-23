#include "numerical-computing/basic_ode_solver.hpp"
#include "internal/ode_stepper.hpp"

namespace numcomp
{
    // -----------------------------------------------------------------
    // Step rules: ONE step of each method. The shared loop in
    // internal/ode_stepper.cpp does the checks, the stepping and the
    // landing on t_end, so it is no longer copied into every method.
    // "static" keeps these helpers private to this file.
    // -----------------------------------------------------------------

    /// Euler: y_new = y + h * f(t, y)
    static double euler_step(const ode::ODEFunction& f, double t, double y, double h)
    {
        return y + h * f(t, y);
    }

    /// Heun: predict with Euler, then average the start and end slopes
    static double heun_step(const ode::ODEFunction& f, double t, double y, double h)
    {
        double k1 = f(t, y);              // predictor slope (standard Euler)
        double k2 = f(t + h, y + h * k1); // corrector slope at the predicted point
        return y + (h / 2.0) * (k1 + k2);
    }

    /// Euler's method. See basic_ode_solver.hpp.
    std::vector<std::pair<double, double>> BasicODESolver::euler(std::function<double(double, double)> f, double t0, double y0, double t_end, double h)
    {
        return ode::run_fixed_steps(f, t0, y0, t_end, h, euler_step);
    }

    /// Heun's method. See basic_ode_solver.hpp.
    std::vector<std::pair<double, double>> BasicODESolver::heun(std::function<double(double, double)> f, double t0, double y0, double t_end, double h)
    {
        return ode::run_fixed_steps(f, t0, y0, t_end, h, heun_step);
    }
}
