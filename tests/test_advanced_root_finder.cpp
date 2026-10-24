// Tests for numcomp::AdvancedRootFinder (Secant + Fixed-Point Iteration)
// Owner: Bakalu Valeria (Member 4)
// Uses the shared helpers in test_helpers.hpp.

#include "numerical-computing/advanced_root_finder.hpp"
#include "test_helpers.hpp"
#include <cmath>
#include <limits>
#include <stdexcept>

using test::check;
using test::close;

// Test functions
double f_square(double x) { return x * x - 4.0; }          // root at 2
double g_sqrt(double x) { return std::sqrt(x + 2.0); }     // fixed point at 2 (x^2 - x - 2 = 0)
double g_cos(double x) { return std::cos(x); }             // fixed point near 0.7390851332

void normal_cases()
{
    numcomp::AdvancedRootFinder finder;
    check("Secant: x^2 - 4 from 1 and 3 gives 2", close(finder.secant_method(f_square, 1.0, 3.0, 1e-6, 100), 2.0, 1e-5));
    check("Fixed-point: g(x) = sqrt(x + 2) from 1 gives 2", close(finder.fixed_point_iteration(g_sqrt, 1.0, 1e-6, 100), 2.0, 1e-5));
    check("Secant: cos(x) - x from 0 and 1", close(finder.secant_method([](double x) { return std::cos(x) - x; }, 0.0, 1.0, 1e-12, 100), 0.7390851332, 1e-9));
    check("Fixed-point: g(x) = cos(x) from 1", close(finder.fixed_point_iteration(g_cos, 1.0, 1e-12, 1000), 0.7390851332, 1e-9));
}

void edge_cases()
{
    numcomp::AdvancedRootFinder finder;
    check("Secant: guesses in either order (3 then 1)", close(finder.secant_method(f_square, 3.0, 1.0, 1e-10, 100), 2.0, 1e-8));
    check("Fixed-point: starting on the answer returns it", close(finder.fixed_point_iteration(g_sqrt, 2.0, 1e-10, 10), 2.0, 1e-12));
}

void invalid_input()
{
    numcomp::AdvancedRootFinder finder;
    double nan = std::numeric_limits<double>::quiet_NaN();

    // max_iter = 2 is far too few steps for tol = 1e-12
    test::expect_throw<std::runtime_error>("Secant: max_iter = 2 throws runtime_error", [&] { finder.secant_method(f_square, 1.0, 3.0, 1e-12, 2); });
    test::expect_throw<std::runtime_error>("Fixed-point: max_iter = 2 throws runtime_error", [&] { finder.fixed_point_iteration(g_sqrt, 1.0, 1e-12, 2); });

    test::expect_throw<std::invalid_argument>("Secant: tol = 0 throws invalid_argument", [&] { finder.secant_method(f_square, 1.0, 3.0, 0.0, 100); });
    test::expect_throw<std::invalid_argument>("Fixed-point: tol = 0 throws invalid_argument", [&] { finder.fixed_point_iteration(g_sqrt, 1.0, 0.0, 100); });
    test::expect_throw<std::invalid_argument>("Secant: max_iter = 0 throws invalid_argument", [&] { finder.secant_method(f_square, 1.0, 3.0, 1e-6, 0); });
    test::expect_throw<std::invalid_argument>("Fixed-point: max_iter = 0 throws invalid_argument", [&] { finder.fixed_point_iteration(g_sqrt, 1.0, 1e-6, 0); });
    test::expect_throw<std::invalid_argument>("Secant: tol = NaN throws invalid_argument", [&] { finder.secant_method(f_square, 1.0, 3.0, nan, 100); });

    // x0 == x1 means f(x1) - f(x0) = 0, so the method would divide by zero
    test::expect_throw<std::runtime_error>("Secant: equal guesses throw runtime_error", [&] { finder.secant_method(f_square, 1.0, 1.0, 1e-6, 100); });
    // g(x) = 2x + 1 runs away from its fixed point (-1), so it never converges
    test::expect_throw<std::runtime_error>("Fixed-point: diverging g throws runtime_error", [&] { finder.fixed_point_iteration([](double x) { return 2.0 * x + 1.0; }, 1.0, 1e-8, 100); });
}

int main()
{
    std::cout << "[*] Running Advanced Root Finder Tests..." << std::endl;
    normal_cases();
    edge_cases();
    invalid_input();
    return test::finish("Advanced Root Finder");
}
