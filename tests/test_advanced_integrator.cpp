/**
 * @file test_advanced_integrator.cpp
 * @brief Unit tests for numcomp::AdvancedIntegrator (Simpson's rule and Romberg integration).
 * @author Wasswa George Mpanga (Member 5)
 *
 * Covers normal use, edge cases and invalid input. Each check prints [PASS] or [FAIL];
 * the program returns 0 only if every check passed (see test_helpers.hpp).
 */

#include "numerical-computing/advanced_integrator.hpp"
#include "test_helpers.hpp"
#include <cmath>
#include <limits>
#include <stdexcept>

using test::check;
using test::close;

// Functions with known roots, integrals or fixed points.
/// @brief Test function: returns x * x * x (integral 0..2 = 4).
/// @param x  Point of evaluation.
/// @return The function value.
double cube(double x) { return x * x * x; }
/// @brief Test function: returns x * x * x * x (integral 0..1 = 1/5).
/// @param x  Point of evaluation.
/// @return The function value.
double quartic(double x) { return x * x * x * x; }

/// Typical inputs compared with known exact answers.
void normal_cases()
{
    numcomp::AdvancedIntegrator integrator;
    const double pi = std::acos(-1.0);
    auto s = [](double x) { return std::sin(x); };

    check("Simpson: x^3 on [0, 2] with n = 100 gives 4", close(integrator.simpsons_rule(cube, 0.0, 2.0, 100), 4.0, 1e-6));
    check("Romberg: x^3 on [0, 2] with 5 rows gives 4", close(integrator.romberg_integration(cube, 0.0, 2.0, 5), 4.0, 1e-6));
    check("Simpson: sin on [0, pi] with n = 100 is close to 2", close(integrator.simpsons_rule(s, 0.0, pi, 100), 2.0, 1e-7));
    check("Romberg: sin on [0, pi] with 8 rows is close to 2", close(integrator.romberg_integration(s, 0.0, pi, 8), 2.0, 1e-10));
}

/// Cases where the method is exact, so the tolerance can be very tight.
void exact_results()
{
    numcomp::AdvancedIntegrator integrator;
    // Simpson is exact for cubics, even with only 2 strips
    check("Simpson: x^3 on [0, 2] with n = 2 is exactly 4", close(integrator.simpsons_rule(cube, 0.0, 2.0, 2), 4.0, 1e-12));
    check("Romberg: x^3 on [0, 2] with 2 rows is exactly 4", close(integrator.romberg_integration(cube, 0.0, 2.0, 2), 4.0, 1e-12));
    // Romberg with 3 rows is exact up to degree 5
    check("Romberg: x^4 on [0, 1] with 3 rows is exactly 1/5", close(integrator.romberg_integration(quartic, 0.0, 1.0, 3), 0.2, 1e-12));
}

/// Boundary situations: end points, reversed intervals, minimum sizes.
void edge_cases()
{
    numcomp::AdvancedIntegrator integrator;
    check("Simpson: reversed limits [2, 0] give -4", close(integrator.simpsons_rule(cube, 2.0, 0.0, 10), -4.0, 1e-12));
    check("Romberg: reversed limits [2, 0] give -4", close(integrator.romberg_integration(cube, 2.0, 0.0, 4), -4.0, 1e-12));
    // With 1 row Romberg is just one trapezoid: (f(0) + f(2)) / 2 * 2 = 8
    check("Romberg: 1 row equals one trapezoid (8)", close(integrator.romberg_integration(cube, 0.0, 2.0, 1), 8.0, 1e-12));
    test::expect_no_throw("Romberg: 20 rows runs without error", [&] { integrator.romberg_integration(cube, 0.0, 2.0, 20); });
}

/// Invalid arguments must throw the exception documented in the header.
void invalid_input()
{
    numcomp::AdvancedIntegrator integrator;
    double nan = std::numeric_limits<double>::quiet_NaN();
    double inf = std::numeric_limits<double>::infinity();

    test::expect_throw<std::invalid_argument>("Simpson: odd n = 3 throws invalid_argument", [&] { integrator.simpsons_rule(cube, 0.0, 2.0, 3); });
    test::expect_throw<std::invalid_argument>("Simpson: n = 0 throws invalid_argument", [&] { integrator.simpsons_rule(cube, 0.0, 2.0, 0); });
    test::expect_throw<std::invalid_argument>("Romberg: max_iter = 26 (above 25) throws invalid_argument", [&] { integrator.romberg_integration(cube, 0.0, 2.0, 26); });
    test::expect_throw<std::invalid_argument>("Romberg: max_iter = 0 throws invalid_argument", [&] { integrator.romberg_integration(cube, 0.0, 2.0, 0); });
    test::expect_throw<std::invalid_argument>("Simpson: a = NaN throws invalid_argument", [&] { integrator.simpsons_rule(cube, nan, 2.0, 10); });
    test::expect_throw<std::invalid_argument>("Romberg: b = infinity throws invalid_argument", [&] { integrator.romberg_integration(cube, 0.0, inf, 5); });
}

/**
 * @brief Runs every test group.
 * @return 0 if all checks passed, 1 otherwise.
 */
int main()
{
    std::cout << "[*] Running Advanced Integration Tests..." << std::endl;
    normal_cases();
    exact_results();
    edge_cases();
    invalid_input();
    return test::finish("Advanced Integration");
}
