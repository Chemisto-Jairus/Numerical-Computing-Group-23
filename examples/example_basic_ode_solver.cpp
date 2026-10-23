/**
 * @file example_basic_ode_solver.cpp
 * @brief Demo 1 of 2 for numcomp::BasicODESolver: Euler and Heun.
 * @author Mwanje Ian (Member 6)
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
    demo::title("Basic ODE Solver", "Euler and Heun on dy/dt = f(t, y)");

    demo::section("Growth dy/dt = y, y(0) = 1, h = 0.1: the full curve", [&] {
        auto f = [](double, double y) { return y; };
        std::vector<std::pair<double, double>> e = solver.euler(f, 0, 1, 1, 0.1);
        std::vector<std::pair<double, double>> h = solver.heun(f, 0, 1, 1, 0.1);
        demo::table_header({"t", "Euler", "Heun", "exact e^t"});
        for (std::size_t i = 0; i < e.size(); i += 2)
        {
            demo::table_row({e[i].first, e[i].second, h[i].second, std::exp(e[i].first)});
        }
    });

    demo::section("Order of accuracy at t = 1 (dy/dt = y)", [&] {
        auto f = [](double, double y) { return y; };
        demo::table_header({"h", "Euler error", "Heun error"});
        for (double h : {0.1, 0.05, 0.025, 0.0125})
        {
            double ee = std::abs(solver.euler(f, 0, 1, 1, h).back().second - std::exp(1.0));
            double he = std::abs(solver.heun(f, 0, 1, 1, h).back().second - std::exp(1.0));
            demo::table_row({h, ee, he}, 5);
        }
        std::cout << "  Halving h halves Euler's error (order 1) and quarters Heun's (order 2).\n";
    });

    demo::section("Other right-hand sides at t = 2, h = 0.01", [&] {
        demo::compare("Heun: dy/dt = cos t, y(0) = 0", solver.heun([](double t, double) { return std::cos(t); }, 0, 0, 2, 0.01).back().second, std::sin(2.0));
        demo::compare("Heun: dy/dt = -2ty, y(0) = 1", solver.heun([](double t, double y) { return -2 * t * y; }, 0, 1, 2, 0.01).back().second, std::exp(-4.0));
        demo::compare("Euler: dy/dt = 2t, y(0) = 0", solver.euler([](double t, double) { return 2 * t; }, 0, 0, 2, 0.01).back().second, 4.0);
    });

    demo::section("Invalid input is rejected", [&] {
        auto f = [](double, double y) { return y; };
        demo::show_rejected("Step size h = 0", [&] { solver.euler(f, 0, 1, 1, 0.0); });
        demo::show_rejected("t_end before t0", [&] { solver.heun(f, 1, 1, 0, 0.1); });
        demo::show_rejected("Too many steps (h = 1e-8)", [&] { solver.euler(f, 0, 1, 1, 1e-8); });
    });

    return demo::finish();
}
