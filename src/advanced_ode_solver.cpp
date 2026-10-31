/**
 * @file advanced_ode_solver.cpp
 * @brief Implementation of numcomp::AdvancedODESolver (RK3 and RK4).
 * @author Muwanguzi Samuel (Member 7)
 *
 * Each method only supplies its one-step formula ("step rule").
 * Input checking, the time loop and the exact landing on t_end are done
 * once, in ode::run_fixed_steps().
 */

#include "numerical-computing/advanced_ode_solver.hpp"
#include "internal/ode_stepper.hpp"

namespace numcomp
{
    /**
     * @brief One classic RK4 step: weighted average of four slopes (weights 1, 2, 2, 1).
     * @param f  Right-hand side f(t, y).
     * @param t  Current time.
     * @param y  Current value.
     * @param h  Step size.
     * @return y at time t + h.
     */
    static double rk4_step(const ode::ODEFunction& f, double t, double y, double h)
    {
        double k1 = f(t, y);                             // slope at the start
        double k2 = f(t + h / 2.0, y + (h / 2.0) * k1);  // slope at the midpoint, using k1
        double k3 = f(t + h / 2.0, y + (h / 2.0) * k2);  // slope at the midpoint, using k2
        double k4 = f(t + h, y + h * k3);                // slope at the end, using k3
        return y + (h / 6.0) * (k1 + 2.0 * k2 + 2.0 * k3 + k4);
    }

    /**
     * @brief One Kutta RK3 step: weighted average of three slopes (weights 1, 4, 1).
     * @param f  Right-hand side f(t, y).
     * @param t  Current time.
     * @param y  Current value.
     * @param h  Step size.
     * @return y at time t + h.
     */
    static double rk3_step(const ode::ODEFunction& f, double t, double y, double h)
    {
        double k1 = f(t, y);                                // slope at the start
        double k2 = f(t + h / 2.0, y + (h / 2.0) * k1);     // slope at the midpoint
        double k3 = f(t + h, y - h * k1 + 2.0 * h * k2);    // slope at the end
        return y + (h / 6.0) * (k1 + 4.0 * k2 + k3);
    }

    std::vector<std::pair<double, double>> AdvancedODESolver::rk4(std::function<double(double, double)> f, double t0, double y0, double t_end, double h)
    {
        return ode::run_fixed_steps(f, t0, y0, t_end, h, rk4_step);
    }

    std::vector<std::pair<double, double>> AdvancedODESolver::rk3(std::function<double(double, double)> f, double t0, double y0, double t_end, double h)
    {
        return ode::run_fixed_steps(f, t0, y0, t_end, h, rk3_step);
    }
}
