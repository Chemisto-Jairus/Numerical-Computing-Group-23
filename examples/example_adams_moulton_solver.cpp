/**
 * @file example_adams_moulton_solver.cpp
 * @brief Demo 1 of 2 for numcomp::AdamsMoultonSolver: AM2 and AM4.
 * @author Muwanguzi Samuel (Member 7)
 */

#include "numerical-computing/adams_moulton_solver.hpp"
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
    numcomp::AdamsMoultonSolver am;
    demo::title("Adams-Moulton Solver", "Implicit multistep methods AM2 (trapezoidal) and AM4");

    demo::section("Decay dy/dt = -2y, y(0) = 1, h = 0.1", [&] {
        auto f = [](double, double y) { return -2.0 * y; };
        std::vector<std::pair<double, double>> a = am.am2(f, 0, 1, 1, 0.1);
        std::vector<std::pair<double, double>> b = am.am4(f, 0, 1, 1, 0.1);
        demo::table_header({"t", "AM2", "AM4", "exact"});
        for (std::size_t i = 0; i < a.size(); i += 2)
        {
            demo::table_row({a[i].first, a[i].second, b[i].second, std::exp(-2.0 * a[i].first)}, 9);
        }
    });

    demo::section("Implicit versus explicit: errors at t = 1 for dy/dt = -2y", [&] {
        auto f = [](double, double y) { return -2.0 * y; };
        numcomp::AdamsBashforthSolver ab;
        double exact = std::exp(-2.0);
        demo::table_header({"h", "AB2", "AM2", "AB4", "AM4"});
        for (double h : {0.1, 0.05, 0.025})
        {
            demo::table_row({h, std::abs(ab.ab2(f, 0, 1, 1, h).back().second - exact), std::abs(am.am2(f, 0, 1, 1, h).back().second - exact),
                                std::abs(ab.ab4(f, 0, 1, 1, h).back().second - exact), std::abs(am.am4(f, 0, 1, 1, h).back().second - exact)}, 3);
        }
        std::cout << "  Same orders (2 and 4), but the implicit formulas have much smaller error constants.\n";
    });

    demo::section("A fast-decaying problem dy/dt = -50 (y - 1), y(0) = 0, h = 0.03", [&] {
        auto f = [](double, double y) { return -50.0 * (y - 1.0); };
        numcomp::AdamsBashforthSolver ab;
        demo::value("AB2 y(1) (explicit)", ab.ab2(f, 0, 0, 1, 0.03).back().second, 6);
        demo::value("AM2 y(1) (implicit)", am.am2(f, 0, 0, 1, 0.03).back().second, 6);
        demo::value("Exact y(1)", 1.0 - std::exp(-50.0), 6);
        std::cout << "  With this step AB2 is unstable and its error explodes; AM2 stays close to 1.\n";
    });

    demo::section("Invalid input is rejected", [&] {
        auto f = [](double, double y) { return y; };
        demo::show_rejected("Step size 0", [&] { am.am4(f, 0, 1, 1, 0.0); });
        demo::show_rejected("Corrector diverges: dy/dt = -50y with h = 0.1", [&] { am.am2([](double, double y) { return -50.0 * y; }, 0, 1, 1, 0.1); });
    });

    return demo::finish();
}
