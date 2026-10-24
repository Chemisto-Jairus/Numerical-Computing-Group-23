/**
 * @file example_basic_root_finder.cpp
 * @brief Demo 1 of 2 for numcomp::BasicRootFinder: Bisection and Newton-Raphson on standard equations.
 * @author Chemisto Jairus (Member 1)
 *
 * Shows both methods on the same equations, how the tolerance affects the
 * result, and how invalid input is rejected.
 */

#include "numerical-computing/basic_root_finder.hpp"
#include "example_helpers.hpp"
#include <cmath>

/**
 * @brief Runs the demo sections.
 * @return 0 if every section completed, 1 otherwise.
 */
int main()
{
    numcomp::BasicRootFinder finder;
    demo::title("Basic Root Finder", "Bisection and Newton-Raphson on standard equations");

    demo::section("Square root of 2: f(x) = x^2 - 2", [&] {
        auto f  = [](double x) { return x * x - 2.0; };
        auto df = [](double x) { return 2.0 * x; };
        demo::compare("Bisection on [0, 2]", finder.bisection(f, 0.0, 2.0, 1e-12, 200), std::sqrt(2.0));
        demo::compare("Newton from x0 = 1", finder.newton_raphson(f, df, 1.0, 1e-12, 50), std::sqrt(2.0));
    });

    demo::section("Cubic: f(x) = x^3 - x - 2", [&] {
        auto f  = [](double x) { return x * x * x - x - 2.0; };
        auto df = [](double x) { return 3.0 * x * x - 1.0; };
        double reference = 1.5213797068045676;
        demo::compare("Bisection on [1, 2]", finder.bisection(f, 1.0, 2.0, 1e-12, 200), reference);
        demo::compare("Newton from x0 = 1.5", finder.newton_raphson(f, df, 1.5, 1e-12, 50), reference);
    });

    demo::section("Transcendental: f(x) = cos(x) - x", [&] {
        auto f  = [](double x) { return std::cos(x) - x; };
        auto df = [](double x) { return -std::sin(x) - 1.0; };
        double reference = 0.7390851332151607;
        demo::compare("Bisection on [0, 1]", finder.bisection(f, 0.0, 1.0, 1e-12, 200), reference);
        demo::compare("Newton from x0 = 1", finder.newton_raphson(f, df, 1.0, 1e-12, 50), reference);
    });

    demo::section("Effect of the tolerance (bisection, x^2 - 2 on [0, 2])", [&] {
        auto f = [](double x) { return x * x - 2.0; };
        demo::table_header({"tol", "root", "error"});
        for (double tol : {1e-2, 1e-4, 1e-6, 1e-8, 1e-10})
        {
            double r = finder.bisection(f, 0.0, 2.0, tol, 200);
            demo::table_row({tol, r, std::abs(r - std::sqrt(2.0))});
        }
    });

    demo::section("Invalid input is rejected", [&] {
        auto f  = [](double x) { return x * x - 2.0; };
        auto df = [](double x) { return 2.0 * x; };
        demo::show_rejected("Bisection, no sign change on [2, 3]", [&] { finder.bisection(f, 2.0, 3.0, 1e-8, 100); });
        demo::show_rejected("Bisection, tol = 0", [&] { finder.bisection(f, 0.0, 2.0, 0.0, 100); });
        demo::show_rejected("Newton, f'(x0) = 0 at x0 = 0", [&] { finder.newton_raphson(f, df, 0.0, 1e-8, 50); });
    });

    return demo::finish();
}
