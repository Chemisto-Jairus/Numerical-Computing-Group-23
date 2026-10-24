/**
 * @file example_advanced_root_finder.cpp
 * @brief Demo 1 of 2 for numcomp::AdvancedRootFinder: Secant method and Fixed-Point Iteration.
 * @author Bakalu Valeria (Member 4)
 */

#include "numerical-computing/advanced_root_finder.hpp"
#include "example_helpers.hpp"
#include <cmath>

/**
 * @brief Runs the demo sections.
 * @return 0 if every section completed, 1 otherwise.
 */
int main()
{
    numcomp::AdvancedRootFinder finder;
    demo::title("Advanced Root Finder", "Secant method and Fixed-Point Iteration");

    demo::section("Secant method", [&] {
        demo::compare("x^2 - 4 from 1 and 3", finder.secant_method([](double x) { return x * x - 4.0; }, 1.0, 3.0, 1e-12, 100), 2.0);
        demo::compare("x^3 - 2x - 5 from 2 and 3", finder.secant_method([](double x) { return x * x * x - 2.0 * x - 5.0; }, 2.0, 3.0, 1e-12, 100), 2.0945514815423265);
        demo::compare("e^x - 3 from 0 and 2", finder.secant_method([](double x) { return std::exp(x) - 3.0; }, 0.0, 2.0, 1e-12, 100), std::log(3.0));
    });

    demo::section("Fixed-Point Iteration x = g(x)", [&] {
        demo::compare("g(x) = sqrt(x + 2) from 1", finder.fixed_point_iteration([](double x) { return std::sqrt(x + 2.0); }, 1.0, 1e-12, 200), 2.0);
        demo::compare("g(x) = cos(x) from 1", finder.fixed_point_iteration([](double x) { return std::cos(x); }, 1.0, 1e-12, 1000), 0.7390851332151607);
        demo::compare("g(x) = e^(-x) from 0.5", finder.fixed_point_iteration([](double x) { return std::exp(-x); }, 0.5, 1e-12, 1000), 0.5671432904097838);
    });

    demo::section("Why the choice of g matters (root of x^2 - x - 2 = 0 is 2)", [&] {
        // |g'(2)| = 1/4 < 1, so this rearrangement converges.
        demo::compare("g(x) = sqrt(x + 2)", finder.fixed_point_iteration([](double x) { return std::sqrt(x + 2.0); }, 1.0, 1e-12, 200), 2.0);
        // |g'(2)| = 4 > 1, so this rearrangement runs away from the root.
        demo::show_rejected("g(x) = x^2 - 2 (diverges)", [&] {
            finder.fixed_point_iteration([](double x) { return x * x - 2.0; }, 2.5, 1e-12, 50);
        });
    });

    demo::section("Invalid input is rejected", [&] {
        auto f = [](double x) { return x * x - 4.0; };
        demo::show_rejected("Secant, equal guesses 1 and 1", [&] { finder.secant_method(f, 1.0, 1.0, 1e-8, 100); });
        demo::show_rejected("Secant, max_iter = 0", [&] { finder.secant_method(f, 1.0, 3.0, 1e-8, 0); });
        demo::show_rejected("Fixed-point, tol = -1", [&] { finder.fixed_point_iteration([](double x) { return std::cos(x); }, 1.0, -1.0, 100); });
    });

    return demo::finish();
}
