/**
 * @file test_basic_root_finder.cpp
 * @brief Unit tests for numcomp::BasicRootFinder (Bisection and Newton-Raphson).
 * @author Chemisto Jairus (Member 1)
 *
 * Covers normal use, edge cases and invalid input. Each check prints [PASS] or [FAIL];
 * the program returns 0 only if every check passed (see test_helpers.hpp).
 */

#include "numerical-computing/basic_root_finder.hpp"
#include "test_helpers.hpp"
#include <cmath>
#include <limits>
#include <stdexcept>

using test::check;
using test::close;

// Functions with known roots, integrals or fixed points.
/// @brief Test function: returns x * x - 4.0 (roots at -2 and 2).
/// @param x  Point of evaluation.
/// @return The function value.
double f_square(double x) { return x * x - 4.0; }
/// @brief Test function: returns 2.0 * x.
/// @param x  Point of evaluation.
/// @return The function value.
double df_square(double x) { return 2.0 * x; }
/// @brief Test function: returns x * x * x - x - 2.0 (one root near 1.5213797068).
/// @param x  Point of evaluation.
/// @return The function value.
double f_cubic(double x) { return x * x * x - x - 2.0; }
/// @brief Test function: returns 3.0 * x * x - 1.0.
/// @param x  Point of evaluation.
/// @return The function value.
double df_cubic(double x) { return 3.0 * x * x - 1.0; }
/// @brief Test function: returns std::cos(x) - x (root near 0.7390851332).
/// @param x  Point of evaluation.
/// @return The function value.
double f_cos(double x) { return std::cos(x) - x; }
/// @brief Test function: returns -std::sin(x) - 1.0.
/// @param x  Point of evaluation.
/// @return The function value.
double df_cos(double x) { return -std::sin(x) - 1.0; }

/// Typical inputs compared with known exact answers.
void normal_cases()
{
    numcomp::BasicRootFinder finder;
    check("Bisection: x^2 - 4 on [0, 3] gives 2", close(finder.bisection(f_square, 0.0, 3.0, 1e-8, 100), 2.0, 1e-6));
    check("Bisection: x^3 - x - 2 on [1, 2]", close(finder.bisection(f_cubic, 1.0, 2.0, 1e-10, 200), 1.5213797068, 1e-7));
    check("Newton: x^2 - 4 from x0 = 3 gives 2", close(finder.newton_raphson(f_square, df_square, 3.0, 1e-10, 50), 2.0, 1e-9));
    check("Newton: x^3 - x - 2 from x0 = 1.5", close(finder.newton_raphson(f_cubic, df_cubic, 1.5, 1e-12, 50), 1.5213797068, 1e-9));
    check("Newton: cos(x) - x from x0 = 1", close(finder.newton_raphson(f_cos, df_cos, 1.0, 1e-12, 50), 0.7390851332, 1e-9));
}

/// Boundary situations: end points, reversed intervals, minimum sizes.
void edge_cases()
{
    numcomp::BasicRootFinder finder;
    // A reversed interval must give the same root as the ordered one.
    check("Bisection: reversed interval [3, 0] still gives 2", close(finder.bisection(f_square, 3.0, 0.0, 1e-8, 100), 2.0, 1e-6));
    check("Bisection: [-3, 0] finds the negative root -2", close(finder.bisection(f_square, -3.0, 0.0, 1e-8, 100), -2.0, 1e-6));
    check("Bisection: root exactly at a = 2 is returned", finder.bisection(f_square, 2.0, 5.0, 1e-8, 100) == 2.0);
    check("Bisection: root exactly at b = 2 is returned", finder.bisection(f_square, 0.0, 2.0, 1e-8, 100) == 2.0);
    check("Newton: starting on the root stays there", close(finder.newton_raphson(f_square, df_square, 2.0, 1e-10, 5), 2.0, 1e-12));
}

/// Invalid arguments must throw the exception documented in the header.
void invalid_input()
{
    numcomp::BasicRootFinder finder;
    double nan = std::numeric_limits<double>::quiet_NaN();

    test::expect_throw<std::invalid_argument>("Bisection: tol = 0 throws invalid_argument", [&] { finder.bisection(f_square, 0.0, 3.0, 0.0, 100); });
    test::expect_throw<std::invalid_argument>("Bisection: max_iter = 0 throws invalid_argument", [&] { finder.bisection(f_square, 0.0, 3.0, 1e-8, 0); });
    test::expect_throw<std::invalid_argument>("Bisection: tol = NaN throws invalid_argument", [&] { finder.bisection(f_square, 0.0, 3.0, nan, 100); });
    test::expect_throw<std::invalid_argument>("Bisection: same sign at both ends throws invalid_argument", [&] { finder.bisection(f_square, 3.0, 5.0, 1e-8, 100); });
    test::expect_throw<std::invalid_argument>("Bisection: f giving NaN throws invalid_argument", [&] { finder.bisection([&](double) { return nan; }, 0.0, 1.0, 1e-8, 100); });
    test::expect_throw<std::runtime_error>("Bisection: max_iter = 2 throws runtime_error", [&] { finder.bisection(f_square, 0.0, 3.0, 1e-12, 2); });

    test::expect_throw<std::invalid_argument>("Newton: tol = 0 throws invalid_argument", [&] { finder.newton_raphson(f_square, df_square, 3.0, 0.0, 50); });
    test::expect_throw<std::invalid_argument>("Newton: max_iter = 0 throws invalid_argument", [&] { finder.newton_raphson(f_square, df_square, 3.0, 1e-8, 0); });
    test::expect_throw<std::runtime_error>("Newton: zero derivative throws runtime_error", [&] { finder.newton_raphson(f_square, df_square, 0.0, 1e-8, 50); });
    test::expect_throw<std::runtime_error>("Newton: max_iter = 2 throws runtime_error", [&] { finder.newton_raphson(f_square, df_square, 100.0, 1e-12, 2); });
}

/**
 * @brief Runs every test group.
 * @return 0 if all checks passed, 1 otherwise.
 */
int main()
{
    std::cout << "[*] Running Basic Root Finder Tests..." << std::endl;
    normal_cases();
    edge_cases();
    invalid_input();
    return test::finish("Basic Root Finder");
}
