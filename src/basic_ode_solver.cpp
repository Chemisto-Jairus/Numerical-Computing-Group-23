/**
 * @file basic_ode_solver.cpp
 * @brief Implementation of numcomp::BasicODESolver (Euler and Heun).
 * @author Mwanje Ian (Member 6)
 *
 * Each method only supplies its one-step formula ("step rule").
 * Input checking, the time loop and the exact landing on t_end are done
 * once, in ode::run_fixed_steps().
 */

#include "numerical-computing/basic_ode_solver.hpp"
#include "internal/ode_stepper.hpp"

namespace numcomp
{
    /**
     * @brief One Euler step: y + h f(t, y).
     * @param f  Right-hand side f(t, y).
     * @param t  Current time.
     * @param y  Current value.
     * @param h  Step size.
     * @return y at time t + h.
     */
    static double euler_step(const ode::ODEFunction& f, double t, double y, double h)
    {
        return y + h * f(t, y);
    }

    /**
     * @brief One Heun step: average of the slope at the start and at the Euler-predicted end.
     * @param f  Right-hand side f(t, y).
     * @param t  Current time.
     * @param y  Current value.
     * @param h  Step size.
     * @return y at time t + h.
     */
    static double heun_step(const ode::ODEFunction& f, double t, double y, double h)
    {
        double k1 = f(t, y);                // slope at the start of the step
        double k2 = f(t + h, y + h * k1);   // slope at the predicted end point
        return y + (h / 2.0) * (k1 + k2);
    }

    std::vector<std::pair<double, double>> BasicODESolver::euler(std::function<double(double, double)> f, double t0, double y0, double t_end, double h)
    {
        return ode::run_fixed_steps(f, t0, y0, t_end, h, euler_step);
    }

    std::vector<std::pair<double, double>> BasicODESolver::heun(std::function<double(double, double)> f, double t0, double y0, double t_end, double h)
    {
        return ode::run_fixed_steps(f, t0, y0, t_end, h, heun_step);
    }
}
