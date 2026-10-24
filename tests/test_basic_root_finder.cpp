// Tests for numcomp::BasicRootFinder (Bisection + Newton-Raphson)
// Owner: Chemisto Jairus (Member 1)
// Uses the shared helpers in test_helpers.hpp (check, close, expect_throw, finish).

#include "numerical-computing/basic_root_finder.hpp"
#include "test_helpers.hpp"
#include <cmath>
#include <limits>
#include <stdexcept>

using test::check;
using test::close;

// Test functions
double f_square(double x) { return x * x - 4.0; }        // roots at -2 and 2
double df_square(double x) { return 2.0 * x; }
double f_cubic(double x) { return x * x * x - x - 2.0; }  // one root near 1.5213797068
double df_cubic(double x) { return 3.0 * x * x - 1.0; }
double f_cos(double x) { return std::cos(x) - x; }        // root near 0.7390851332
double df_cos(double x) { return -std::sin(x) - 1.0; }

void normal_cases()
{
    numcomp::BasicRootFinder finder;
    check("Bisection: x^2 - 4 on [0, 3] gives 2", close(finder.bisection(f_square, 0.0, 3.0, 1e-8, 100), 2.0, 1e-6));
    check("Bisection: x^3 - x - 2 on [1, 2]", close(finder.bisection(f_cubic, 1.0, 2.0, 1e-10, 200), 1.5213797068, 1e-7));
    check("Newton: x^2 - 4 from x0 = 3 gives 2", close(finder.newton_raphson(f_square, df_square, 3.0, 1e-10, 50), 2.0, 1e-9));
    check("Newton: x^3 - x - 2 from x0 = 1.5", close(finder.newton_raphson(f_cubic, df_cubic, 1.5, 1e-12, 50), 1.5213797068, 1e-9));
    check("Newton: cos(x) - x from x0 = 1", close(finder.newton_raphson(f_cos, df_cos, 1.0, 1e-12, 50), 0.7390851332, 1e-9));
}

void edge_cases()
{
    numcomp::BasicRootFinder finder;
    // Regression test for the Week 3 bug: [3, 0] used to return 1.5
    check("Bisection: reversed interval [3, 0] still gives 2", close(finder.bisection(f_square, 3.0, 0.0, 1e-8, 100), 2.0, 1e-6));
    check("Bisection: [-3, 0] finds the negative root -2", close(finder.bisection(f_square, -3.0, 0.0, 1e-8, 100), -2.0, 1e-6));
    check("Bisection: root exactly at a = 2 is returned", finder.bisection(f_square, 2.0, 5.0, 1e-8, 100) == 2.0);
    check("Bisection: root exactly at b = 2 is returned", finder.bisection(f_square, 0.0, 2.0, 1e-8, 100) == 2.0);
    check("Newton: starting on the root stays there", close(finder.newton_raphson(f_square, df_square, 2.0, 1e-10, 5), 2.0, 1e-12));
}

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

int main()
{
    std::cout << "[*] Running Basic Root Finder Tests..." << std::endl;
    normal_cases();
    edge_cases();
    invalid_input();
    return test::finish("Basic Root Finder");
}
