/**
 * @file example_advanced_ode_solver_applied.cpp
 * @brief Demo 2 of 2 for numcomp::AdvancedODESolver: applied models.
 * @author Muwanguzi Samuel (Member 7)
 *
 * - Charging a capacitor through a resistor (RC circuit).
 * - Drug concentration with constant infusion and elimination.
 */

#include "numerical-computing/advanced_ode_solver.hpp"
#include "example_helpers.hpp"
#include <cmath>
#include <utility>
#include <vector>

/**
 * @brief Runs the demo sections.
 * @return 0 if every section completed, 1 otherwise.
 */
int main()
{
    numcomp::AdvancedODESolver solver;
    demo::title("Advanced ODE Solver (applied)", "RC circuit charging and drug infusion");

    demo::section("RC circuit: dV/dt = (12 - V) / (R C), R C = 0.5 s, V(0) = 0", [&] {
        const double tau = 0.5, source = 12.0;
        auto f = [=](double, double V) { return (source - V) / tau; };
        auto exact = [=](double t) { return source * (1.0 - std::exp(-t / tau)); };
        std::vector<std::pair<double, double>> curve = solver.rk4(f, 0, 0, 2.5, 0.05);
        demo::table_header({"time (s)", "RK4 V", "exact V"});
        for (std::size_t i = 0; i < curve.size(); i += 10)
        {
            demo::table_row({curve[i].first, curve[i].second, exact(curve[i].first)}, 9);
        }
    });

    demo::section("Drug infusion: dC/dt = 5 - 0.25 C (mg/L per hour), C(0) = 0", [&] {
        auto f = [](double, double C) { return 5.0 - 0.25 * C; };
        auto exact = [](double t) { return 20.0 * (1.0 - std::exp(-0.25 * t)); };
        demo::compare("RK3, h = 0.5, C(12 h)", solver.rk3(f, 0, 0, 12, 0.5).back().second, exact(12));
        demo::compare("RK4, h = 0.5, C(12 h)", solver.rk4(f, 0, 0, 12, 0.5).back().second, exact(12));
        demo::value("Steady state 5 / 0.25 (mg/L)", 20.0, 2);
    });

    return demo::finish();
}
