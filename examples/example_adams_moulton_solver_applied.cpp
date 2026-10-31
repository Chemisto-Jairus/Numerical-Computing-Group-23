/**
 * @file example_adams_moulton_solver_applied.cpp
 * @brief Demo 2 of 2 for numcomp::AdamsMoultonSolver: applied models.
 * @author Muwanguzi Samuel (Member 7)
 *
 * - A second-order chemical reaction.
 * - Water draining from a tank through a hole (Torricelli's law).
 */

#include "numerical-computing/adams_moulton_solver.hpp"
#include "example_helpers.hpp"
#include <algorithm>
#include <cmath>
#include <utility>
#include <vector>

/**
 * @brief Runs the demo sections.
 * @return 0 if every section completed, 1 otherwise.
 */
int main()
{
    numcomp::AdamsMoultonSolver am;
    demo::title("Adams-Moulton Solver (applied)", "Reaction kinetics and a draining tank");

    demo::section("Reaction 2A -> products: dC/dt = -0.5 C^2 (mol/L per min), C(0) = 2", [&] {
        auto f = [](double, double C) { return -0.5 * C * C; };
        auto exact = [](double t) { return 2.0 / (1.0 + t); };
        std::vector<std::pair<double, double>> curve = am.am4(f, 0, 2, 10, 0.1);
        demo::table_header({"time (min)", "AM4 C", "exact C"});
        for (std::size_t i = 0; i < curve.size(); i += 20)
        {
            demo::table_row({curve[i].first, curve[i].second, exact(curve[i].first)}, 9);
        }
    });

    demo::section("Tank: dh/dt = -0.2 sqrt(h) (m per min), h(0) = 4 m", [&] {
        // max(h, 0) guards against a tiny negative h from rounding near the end.
        auto f = [](double, double h) { return -0.2 * std::sqrt(std::max(h, 0.0)); };
        auto exact = [](double t) { return std::pow(2.0 - 0.1 * t, 2); };   // empty at t = 20 min
        std::cout << "  Step size h = 0.1 min.\n";
        demo::compare("AM2: level at 10 min (m)", am.am2(f, 0, 4, 10, 0.1).back().second, exact(10));
        demo::compare("AM4: level at 10 min (m)", am.am4(f, 0, 4, 10, 0.1).back().second, exact(10));
        demo::value("Tank is empty after (min)", 20.0, 1);
    });

    return demo::finish();
}
