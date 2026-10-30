/**
 * @file example_runge_kutta_fehlberg_solver.cpp
 * @brief Demo 1 of 2 for numcomp::RungeKuttaFehlbergSolver: adaptive RKF45.
 * @author Wasswa George Mpanga (Member 5)
 */

#include "numerical-computing/runge_kutta_fehlberg_solver.hpp"
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
    numcomp::RungeKuttaFehlbergSolver rkf;
    demo::title("Runge-Kutta-Fehlberg Solver", "Adaptive step size with an embedded error estimate");

    demo::section("Tolerance controls the error: dy/dt = -2y, y(0) = 1, t = 1", [&] {
        auto f = [](double, double y) { return -2.0 * y; };
        demo::table_header({"tol", "error", "steps"});
        for (double tol : {1e-3, 1e-5, 1e-7, 1e-9})
        {
            std::vector<std::pair<double, double>> curve = rkf.rkf45(f, 0, 1, 1, 0.1, tol);
            demo::table_row({tol, std::abs(curve.back().second - std::exp(-2.0)), static_cast<double>(curve.size() - 1)}, 3);
        }
        std::cout << "  The user chooses the accuracy; the method chooses the steps.\n";
    });

    demo::section("Steps adapt: dy/dt = -50 (y - cos t), y(0) = 0, tol = 1e-6", [&] {
        auto f = [](double t, double y) { return -50.0 * (y - std::cos(t)); };
        std::vector<std::pair<double, double>> curve = rkf.rkf45(f, 0, 0, 1, 0.01, 1e-6);
        demo::table_header({"t", "y", "step used"});
        for (std::size_t i = 1; i < curve.size(); i += curve.size() / 8 + 1)
        {
            demo::table_row({curve[i].first, curve[i].second, curve[i].first - curve[i-1].first}, 6);
        }
        demo::value("Accepted steps", static_cast<double>(curve.size() - 1), 0);
        std::cout << "  Small steps while y jumps up to follow cos t, larger steps once it is smooth.\n";
    });

    demo::section("rkf45_value: just y(t_end), tol = 1e-9", [&] {
        demo::compare("y' = y - t, y(0) = 2: y(1)", rkf.rkf45_value([](double t, double y) { return y - t; }, 0, 2, 1, 1e-9), std::exp(1.0) + 2.0);
        demo::compare("y' = t y, y(0) = 1: y(2)", rkf.rkf45_value([](double t, double y) { return t * y; }, 0, 1, 2, 1e-9), std::exp(2.0));
    });

    demo::section("Invalid input is rejected", [&] {
        auto f = [](double, double y) { return y; };
        demo::show_rejected("Tolerance 0", [&] { rkf.rkf45(f, 0, 1, 1, 0.1, 0.0); });
        demo::show_rejected("Negative first step", [&] { rkf.rkf45(f, 0, 1, 1, -0.1, 1e-6); });
        demo::show_rejected("Solution 1/(1 - t) blows up at t = 1", [&] { rkf.rkf45([](double, double y) { return y * y; }, 0, 1, 2, 0.1, 1e-6); });
    });

    return demo::finish();
}
