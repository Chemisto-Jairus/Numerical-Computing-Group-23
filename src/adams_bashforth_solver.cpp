/**
 * @file adams_bashforth_solver.cpp
 * @brief Implementation of numcomp::AdamsBashforthSolver (AB2 and AB4).
 * @author Mwanje Ian (Member 6)
 *
 * Each method only supplies its multistep formula. The starting values,
 * the stored slopes, the input checks and the landing on t_end are handled
 * once, in ode::run_multistep().
 */

#include "numerical-computing/adams_bashforth_solver.hpp"
#include "internal/multistep.hpp"

namespace numcomp
{
    /**
     * @brief One AB2 step from the two most recent slopes.
     * @param slopes  Slopes at all earlier points; slopes.back() is f_n.
     * @param y       Current value y_n.
     * @param h       Step size.
     * @return y_(n+1).
     */
    static double ab2_step(const ode::ODEFunction&, const std::vector<double>& slopes, double, double y, double h)
    {
        std::size_t n = slopes.size() - 1;   // index of the current point
        return y + (h / 2.0) * (3.0 * slopes[n] - slopes[n-1]);
    }

    /**
     * @brief One AB4 step from the four most recent slopes.
     * @param slopes  Slopes at all earlier points; slopes.back() is f_n.
     * @param y       Current value y_n.
     * @param h       Step size.
     * @return y_(n+1).
     */
    static double ab4_step(const ode::ODEFunction&, const std::vector<double>& slopes, double, double y, double h)
    {
        std::size_t n = slopes.size() - 1;   // index of the current point
        return y + (h / 24.0) * (55.0 * slopes[n] - 59.0 * slopes[n-1] + 37.0 * slopes[n-2] - 9.0 * slopes[n-3]);
    }

    std::vector<std::pair<double, double>> AdamsBashforthSolver::ab2(std::function<double(double, double)> f, double t0, double y0, double t_end, double h)
    {
        return ode::run_multistep(f, t0, y0, t_end, h, 2, ab2_step);
    }

    std::vector<std::pair<double, double>> AdamsBashforthSolver::ab4(std::function<double(double, double)> f, double t0, double y0, double t_end, double h)
    {
        return ode::run_multistep(f, t0, y0, t_end, h, 4, ab4_step);
    }
}
