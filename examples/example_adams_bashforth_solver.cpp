/**
 * @file example_adams_bashforth_solver.cpp
 * @brief Demo 1 of 2 for numcomp::AdamsBashforthSolver: AB2 and AB4.
 * @author Mwanje Ian (Member 6)
 */

#include "numerical-computing/adams_bashforth_solver.hpp"
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
    numcomp::AdamsBashforthSolver ab;
    demo::title("Adams-Bashforth Solver", "Explicit multistep methods AB2 and AB4");

    demo::section("Decay dy/dt = -2y, y(0) = 1, h = 0.1", [&] {
        auto f = [](double, double y) { return -2.0 * y; };
        std::vector<std::pair<double, double>> a = ab.ab2(f, 0, 1, 1, 0.1);
        std::vector<std::pair<double, double>> b = ab.ab4(f, 0, 1, 1, 0.1);
        demo::table_header({"t", "AB2", "AB4", "exact"});
        for (std::size_t i = 0; i < a.size(); i += 2)
        {
            demo::table_row({a[i].first, a[i].second, b[i].second, std::exp(-2.0 * a[i].first)}, 9);
        }
    });

    demo::section("Order of accuracy at t = 1 (dy/dt = -2y)", [&] {
        auto f = [](double, double y) { return -2.0 * y; };
        demo::table_header({"h", "AB2 error", "AB4 error"});
        for (double h : {0.1, 0.05, 0.025, 0.0125})
        {
            demo::table_row({h, std::abs(ab.ab2(f, 0, 1, 1, h).back().second - std::exp(-2.0)),
                                std::abs(ab.ab4(f, 0, 1, 1, h).back().second - std::exp(-2.0))}, 5);
        }
        std::cout << "  Halving h divides AB2's error by about 4 and AB4's by about 16.\n";
    });

    demo::section("Cost: evaluations of f for dy/dt = y - t on [0, 2], h = 0.01", [&] {
        int calls = 0;
        auto f = [&calls](double t, double y) { ++calls; return y - t; };
        double exact = std::exp(2.0) + 3.0;   // y(0) = 2 gives y = e^t + t + 1

        double y_ab4 = ab.ab4(f, 0, 2, 2, 0.01).back().second;
        int ab4_calls = calls;
        calls = 0;
        numcomp::AdvancedODESolver rk;
        double y_rk4 = rk.rk4(f, 0, 2, 2, 0.01).back().second;
        int rk4_calls = calls;

        demo::compare("AB4 y(2)", y_ab4, exact);
        demo::compare("RK4 y(2)", y_rk4, exact);
        demo::value("AB4 evaluations of f", ab4_calls, 0);
        demo::value("RK4 evaluations of f", rk4_calls, 0);
        std::cout << "  Same order of accuracy, about a quarter of the work: AB4 reuses old slopes.\n";
    });

    demo::section("Invalid input is rejected", [&] {
        auto f = [](double, double y) { return y; };
        demo::show_rejected("Negative step size", [&] { ab.ab4(f, 0, 1, 1, -0.1); });
        demo::show_rejected("t_end before t0", [&] { ab.ab2(f, 1, 1, 0, 0.1); });
        demo::show_rejected("Solution 1/(1 - t) blows up at t = 1", [&] { ab.ab4([](double, double y) { return y * y; }, 0, 1, 3, 0.01); });
    });

    return demo::finish();
}
