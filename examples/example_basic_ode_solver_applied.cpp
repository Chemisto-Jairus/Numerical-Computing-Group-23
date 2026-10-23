/**
 * @file example_basic_ode_solver_applied.cpp
 * @brief Demo 2 of 2 for numcomp::BasicODESolver: applied models.
 * @author Mwanje Ian (Member 6)
 *
 * - Newton's law of cooling: a cup of tea in a room.
 * - Logistic population growth with a carrying capacity.
 */

#include "numerical-computing/basic_ode_solver.hpp"
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
    numcomp::BasicODESolver solver;
    demo::title("Basic ODE Solver (applied)", "Cooling tea and logistic population growth");

    demo::section("Newton cooling: dT/dt = -k (T - 22), T(0) = 90 C, k = 0.08 per min", [&] {
        const double k = 0.08, room = 22.0;
        auto f = [=](double, double T) { return -k * (T - room); };
        auto exact = [=](double t) { return room + (90.0 - room) * std::exp(-k * t); };
        std::vector<std::pair<double, double>> curve = solver.heun(f, 0, 90, 30, 0.5);
        demo::table_header({"minute", "Heun T", "exact T"});
        for (std::size_t i = 0; i < curve.size(); i += 10)
        {
            demo::table_row({curve[i].first, curve[i].second, exact(curve[i].first)}, 7);
        }
    });

    demo::section("Logistic growth: dP/dt = r P (1 - P / K), P(0) = 100, r = 0.4, K = 5000", [&] {
        const double r = 0.4, K = 5000.0, P0 = 100.0;
        auto f = [=](double, double P) { return r * P * (1.0 - P / K); };
        auto exact = [=](double t) { return K / (1.0 + (K / P0 - 1.0) * std::exp(-r * t)); };
        demo::compare("Euler, h = 0.1, P(20)", solver.euler(f, 0, P0, 20, 0.1).back().second, exact(20));
        demo::compare("Heun,  h = 0.1, P(20)", solver.heun(f, 0, P0, 20, 0.1).back().second, exact(20));
        demo::compare("Heun,  h = 0.01, P(20)", solver.heun(f, 0, P0, 20, 0.01).back().second, exact(20));
    });

    return demo::finish();
}
