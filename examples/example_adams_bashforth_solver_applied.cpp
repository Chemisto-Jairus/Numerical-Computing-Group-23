/**
 * @file example_adams_bashforth_solver_applied.cpp
 * @brief Demo 2 of 2 for numcomp::AdamsBashforthSolver: applied models.
 * @author Mwanje Ian (Member 6)
 *
 * - A skydiver falling with air resistance (terminal velocity).
 * - Bacteria growing towards the capacity of a dish (logistic model).
 */

#include "numerical-computing/adams_bashforth_solver.hpp"
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
    numcomp::AdamsBashforthSolver ab;
    demo::title("Adams-Bashforth Solver (applied)", "Skydiver with air resistance, bacterial growth");

    demo::section("Skydiver: dv/dt = 9.81 - 0.0039 v^2 (m/s^2), v(0) = 0", [&] {
        const double g = 9.81, k = 0.0039;
        const double v_terminal = std::sqrt(g / k);
        auto f = [=](double, double v) { return g - k * v * v; };
        auto exact = [=](double t) { return v_terminal * std::tanh(std::sqrt(g * k) * t); };
        std::vector<std::pair<double, double>> curve = ab.ab4(f, 0, 0, 30, 0.1);
        demo::table_header({"time (s)", "AB4 v (m/s)", "exact v"});
        for (std::size_t i = 0; i < curve.size(); i += 50)
        {
            demo::table_row({curve[i].first, curve[i].second, exact(curve[i].first)}, 8);
        }
        demo::value("Terminal velocity sqrt(g/k) (m/s)", v_terminal, 4);
    });

    demo::section("Bacteria (thousands of cells): dN/dt = 0.8 N (1 - N / 1000), N(0) = 1", [&] {
        auto f = [](double, double N) { return 0.8 * N * (1.0 - N / 1000.0); };
        auto exact = [](double t) { return 1000.0 / (1.0 + 999.0 * std::exp(-0.8 * t)); };
        demo::compare("AB2, h = 0.05, N(12 h)", ab.ab2(f, 0, 1, 12, 0.05).back().second, exact(12));
        demo::compare("AB4, h = 0.05, N(12 h)", ab.ab4(f, 0, 1, 12, 0.05).back().second, exact(12));
        std::cout << "  The higher-order method is far closer for the same step size.\n";
    });

    return demo::finish();
}
