/**
 * @file test_basic_integrator.cpp
 * @brief Unit tests for numcomp::BasicIntegrator (Trapezoidal and Midpoint rules).
 * @author Aogon Sharon (Member 3)
 *
 * Covers normal use, edge cases and invalid input. Each check prints [PASS] or [FAIL];
 * the program returns 0 only if every check passed (see test_helpers.hpp).
 */

#include "numerical-computing/basic_integrator.hpp"
#include "test_helpers.hpp"
#include <cmath>
#include <limits>
#include <stdexcept>

using test::check;
using test::close;

// Functions with known roots, integrals or fixed points.
/// @brief Test function: returns x * x (integral 0..1 = 1/3).
/// @param x  Point of evaluation.
/// @return The function value.
double square(double x) { return x * x; }
/// @brief Test function: returns 2.0 * x + 1.0 (integral 0..3 = 12).
/// @param x  Point of evaluation.
/// @return The function value.
double line(double x) { return 2.0 * x + 1.0; }

/// Typical inputs compared with known exact answers.
void normal_cases()
{
    numcomp::BasicIntegrator integrator;
    const double pi = std::acos(-1.0);
    auto s = [](double x) { return std::sin(x); };

    check("Trapezoidal: x^2 on [0, 1] with n = 100 is close to 1/3", close(integrator.trapezoidal_rule(square, 0.0, 1.0, 100), 1.0 / 3.0, 1e-4));
    check("Midpoint: x^2 on [0, 1] with n = 100 is close to 1/3", close(integrator.midpoint_rule(square, 0.0, 1.0, 100), 1.0 / 3.0, 1e-4));
    check("Trapezoidal: sin on [0, pi] is close to 2", close(integrator.trapezoidal_rule(s, 0.0, pi, 1000), 2.0, 1e-5));
    check("Midpoint: sin on [0, pi] is close to 2", close(integrator.midpoint_rule(s, 0.0, pi, 1000), 2.0, 1e-5));
}

/// Cases where the method is exact, so the tolerance can be very tight.
void exact_results()
{
    numcomp::BasicIntegrator integrator;
    // Both rules are exact for a straight line, even with few strips
    check("Trapezoidal: 2x + 1 on [0, 3] is exactly 12 (n = 1)", close(integrator.trapezoidal_rule(line, 0.0, 3.0, 1), 12.0, 1e-12));
    check("Trapezoidal: 2x + 1 on [0, 3] is exactly 12 (n = 7)", close(integrator.trapezoidal_rule(line, 0.0, 3.0, 7), 12.0, 1e-12));
    check("Midpoint: 2x + 1 on [0, 3] is exactly 12 (n = 1)", close(integrator.midpoint_rule(line, 0.0, 3.0, 1), 12.0, 1e-12));
    check("Midpoint: 2x + 1 on [0, 3] is exactly 12 (n = 7)", close(integrator.midpoint_rule(line, 0.0, 3.0, 7), 12.0, 1e-12));
    // Known errors for x^2 on [0, 1]: trapezoidal = 1/3 + 1/(6n^2), midpoint = 1/3 - 1/(12n^2)
    check("Trapezoidal: x^2 with n = 10 is exactly 1/3 + 1/600", close(integrator.trapezoidal_rule(square, 0.0, 1.0, 10), 1.0 / 3.0 + 1.0 / 600.0, 1e-12));
    check("Midpoint: x^2 with n = 10 is exactly 1/3 - 1/1200", close(integrator.midpoint_rule(square, 0.0, 1.0, 10), 1.0 / 3.0 - 1.0 / 1200.0, 1e-12));
}

/// Boundary situations: end points, reversed intervals, minimum sizes.
void edge_cases()
{
    numcomp::BasicIntegrator integrator;
    // Swapping a and b flips the sign of the integral
    check("Trapezoidal: reversed limits [1, 0] give -1/3", close(integrator.trapezoidal_rule(square, 1.0, 0.0, 1000), -1.0 / 3.0, 1e-6));
    check("Midpoint: reversed limits [1, 0] give -1/3", close(integrator.midpoint_rule(square, 1.0, 0.0, 1000), -1.0 / 3.0, 1e-6));
    check("Trapezoidal: a == b gives 0", integrator.trapezoidal_rule(square, 2.0, 2.0, 10) == 0.0);
    check("Midpoint: a == b gives 0", integrator.midpoint_rule(square, 2.0, 2.0, 10) == 0.0);
    check("Trapezoidal: 2x + 1 on [-2, -1] is exactly -2", close(integrator.trapezoidal_rule(line, -2.0, -1.0, 4), -2.0, 1e-12));
}

/// Invalid arguments must throw the exception documented in the header.
void invalid_input()
{
    numcomp::BasicIntegrator integrator;
    double nan = std::numeric_limits<double>::quiet_NaN();
    double inf = std::numeric_limits<double>::infinity();

    test::expect_throw<std::invalid_argument>("Trapezoidal: n = 0 throws invalid_argument", [&] { integrator.trapezoidal_rule(square, 0.0, 1.0, 0); });
    test::expect_throw<std::invalid_argument>("Midpoint: n = 0 throws invalid_argument", [&] { integrator.midpoint_rule(square, 0.0, 1.0, 0); });
    test::expect_throw<std::invalid_argument>("Trapezoidal: n < 0 throws invalid_argument", [&] { integrator.trapezoidal_rule(square, 0.0, 1.0, -5); });
    test::expect_throw<std::invalid_argument>("Trapezoidal: a = NaN throws invalid_argument", [&] { integrator.trapezoidal_rule(square, nan, 1.0, 10); });
    test::expect_throw<std::invalid_argument>("Midpoint: b = infinity throws invalid_argument", [&] { integrator.midpoint_rule(square, 0.0, inf, 10); });
}

/**
 * @brief Runs every test group.
 * @return 0 if all checks passed, 1 otherwise.
 */
int main()
{
    std::cout << "[*] Running Basic Integration Tests..." << std::endl;
    normal_cases();
    exact_results();
    edge_cases();
    invalid_input();
    return test::finish("Basic Integration");
}
