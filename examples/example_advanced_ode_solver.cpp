/**
 * @file example_advanced_ode_solver.cpp
 * @brief Demo 1 of 2 for numcomp::AdvancedODESolver: Runge-Kutta 3 and 4.
 * @author Muwanguzi Samuel (Member 7)
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
    demo::title("Advanced ODE Solver", "Runge-Kutta 3 and Runge-Kutta 4");

    demo::section("Decay dy/dt = -2y, y(0) = 1, h = 0.1", [&] {
        auto f = [](double, double y) { return -2.0 * y; };
        std::vector<std::pair<double, double>> a = solver.rk3(f, 0, 1, 1, 0.1);
        std::vector<std::pair<double, double>> b = solver.rk4(f, 0, 1, 1, 0.1);
        demo::table_header({"t", "RK3", "RK4", "exact"});
        for (std::size_t i = 0; i < a.size(); i += 2)
        {
            demo::table_row({a[i].first, a[i].second, b[i].second, std::exp(-2.0 * a[i].first)}, 9);
        }
    });

    demo::section("Order of accuracy at t = 1 (dy/dt = -2y)", [&] {
        auto f = [](double, double y) { return -2.0 * y; };
        demo::table_header({"h", "RK3 error", "RK4 error"});
        for (double h : {0.2, 0.1, 0.05, 0.025})
        {
            double e3 = std::abs(solver.rk3(f, 0, 1, 1, h).back().second - std::exp(-2.0));
            double e4 = std::abs(solver.rk4(f, 0, 1, 1, h).back().second - std::exp(-2.0));
            demo::table_row({h, e3, e4}, 5);
        }
        std::cout << "  Halving h divides RK3's error by about 8 and RK4's by about 16.\n";
    });

    demo::section("Other right-hand sides, h = 0.05", [&] {
        const double pi = std::acos(-1.0);
        demo::compare("RK4: dy/dt = -sin t, y(0) = 1, t = pi", solver.rk4([](double t, double) { return -std::sin(t); }, 0, 1, pi, 0.05).back().second, -1.0);
        demo::compare("RK4: dy/dt = 4t^3, y(0) = 0, t = 2", solver.rk4([](double t, double) { return 4 * t * t * t; }, 0, 0, 2, 0.05).back().second, 16.0);
        demo::compare("RK3: dy/dt = y - t, y(0) = 2, t = 1", solver.rk3([](double t, double y) { return y - t; }, 0, 2, 1, 0.05).back().second, std::exp(1.0) + 2.0);
    });

    demo::section("Invalid input is rejected", [&] {
        auto f = [](double, double y) { return y; };
        demo::show_rejected("Negative step size", [&] { solver.rk4(f, 0, 1, 1, -0.1); });
        demo::show_rejected("t_end equal to t0", [&] { solver.rk3(f, 1, 1, 1, 0.1); });
    });

    return demo::finish();
}
