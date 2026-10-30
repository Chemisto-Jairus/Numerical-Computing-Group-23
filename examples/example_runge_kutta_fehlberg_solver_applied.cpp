/**
 * @file example_runge_kutta_fehlberg_solver_applied.cpp
 * @brief Demo 2 of 2 for numcomp::RungeKuttaFehlbergSolver: applied models.
 * @author Wasswa George Mpanga (Member 5)
 *
 * - A skydiver whose parachute opens at t = 10 s (a rapid change in the model).
 * - A drug dose eliminated by an enzyme (Michaelis-Menten kinetics).
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
    demo::title("Runge-Kutta-Fehlberg Solver (applied)", "Parachute opening and drug elimination");

    demo::section("Skydiver: the parachute opens at 10 s, drag coefficient 0.004 -> 0.4 within 1 s", [&] {
        // The drag coefficient rises smoothly (an S-shaped curve centred on t = 10 s).
        auto drag = [](double t) { return 0.004 + 0.396 / (1.0 + std::exp(-10.0 * (t - 10.0))); };
        auto f = [=](double t, double v) { return 9.81 - drag(t) * v * v; };
        std::vector<std::pair<double, double>> curve = rkf.rkf45(f, 0, 0, 20, 0.5, 1e-6);
        double smallest = 1e9, at = 0.0;
        for (std::size_t i = 1; i < curve.size(); ++i)
        {
            double step = curve[i].first - curve[i-1].first;
            if (step < smallest) { smallest = step; at = curve[i].first; }
        }
        demo::value("Speed at 10 s, as it opens (m/s)", rkf.rkf45_value(f, 0, 0, 10, 1e-6), 4);
        demo::value("Speed at 20 s (m/s)", curve.back().second, 4);
        demo::value("Final speed sqrt(9.81/0.4) (m/s)", std::sqrt(9.81 / 0.4), 4);
        demo::value("Accepted steps", static_cast<double>(curve.size() - 1), 0);
        demo::value("Smallest step (s)", smallest, 6);
        demo::value("...taken near t (s)", at, 3);
        std::cout << "  The method shrinks its steps on its own while the parachute opens.\n";
    });

    demo::section("Drug elimination: dC/dt = -10 C / (2 + C) (mg/L per h), C(0) = 20", [&] {
        auto f = [](double, double C) { return -10.0 * C / (2.0 + C); };
        demo::table_header({"time (h)", "C (mg/L)"});
        for (double t : {0.5, 1.0, 1.5, 2.0, 3.0})
        {
            demo::table_row({t, rkf.rkf45_value(f, 0, 20, t, 1e-9)}, 8);
        }
        std::cout << "  Near-constant removal at high C, exponential decay once C is small.\n";
    });

    return demo::finish();
}
