/**
 * @file test_numerical_differentiator.cpp
 * @brief Unit tests for numcomp::NumericalDifferentiator (finite difference formulas).
 * @author Chemisto Jairus (Member 1)
 *
 * Covers normal use, edge cases and invalid input. Each check prints [PASS] or [FAIL];
 * the program returns 0 only if every check passed (see test_helpers.hpp).
 */

#include "numerical-computing/numerical_differentiator.hpp"
#include "test_helpers.hpp"
#include <cmath>
#include <limits>
#include <stdexcept>

using test::check;
using test::close;

/// Accuracy on standard functions, and the order of accuracy of every formula.
void accuracy()
{
    numcomp::NumericalDifferentiator d;
    auto sine = [](double x) { return std::sin(x); };
    double exact = std::cos(1.0);

    check("Forward: d/dx sin at 1, h = 1e-6, close to cos(1)", close(d.forward_difference(sine, 1.0, 1e-6), exact, 1e-5));
    check("Backward: d/dx sin at 1, h = 1e-6, close to cos(1)", close(d.backward_difference(sine, 1.0, 1e-6), exact, 1e-5));
    check("Central: d/dx sin at 1, h = 1e-5, close to cos(1)", close(d.central_difference(sine, 1.0, 1e-5), exact, 1e-9));
    check("Second: d2/dx2 sin at 1, h = 1e-3, close to -sin(1)", close(d.second_derivative(sine, 1.0, 1e-3), -std::sin(1.0), 1e-6));
    check("Richardson: d/dx sin at 1, h = 0.01, close to cos(1)", close(d.richardson_extrapolation(sine, 1.0, 0.01), exact, 1e-10));
    check("Central: d/dx e^x at 0 is 1", close(d.central_difference([](double x) { return std::exp(x); }, 0.0, 1e-5), 1.0, 1e-9));

    // Polynomials: the central formula is exact for quadratics, the second-derivative formula for cubics.
    check("Central: exact for x^2 at x = 3 (gives 6)", close(d.central_difference([](double x) { return x * x; }, 3.0, 0.5), 6.0, 1e-12));
    check("Second: exact for x^3 at x = 2 (gives 12)", close(d.second_derivative([](double x) { return x * x * x; }, 2.0, 0.1), 12.0, 1e-9));
    check("Forward: exact for a straight line 3x + 1", close(d.forward_difference([](double x) { return 3.0 * x + 1.0; }, 5.0, 0.25), 3.0, 1e-12));

    // Order of accuracy: halving h divides the error by about 2, 4 and 16.
    double r_fwd = std::abs(d.forward_difference(sine, 1.0, 0.1) - exact) / std::abs(d.forward_difference(sine, 1.0, 0.05) - exact);
    double r_cen = std::abs(d.central_difference(sine, 1.0, 0.1) - exact) / std::abs(d.central_difference(sine, 1.0, 0.05) - exact);
    double r_sec = std::abs(d.second_derivative(sine, 1.0, 0.1) + std::sin(1.0)) / std::abs(d.second_derivative(sine, 1.0, 0.05) + std::sin(1.0));
    double r_ric = std::abs(d.richardson_extrapolation(sine, 1.0, 0.1) - exact) / std::abs(d.richardson_extrapolation(sine, 1.0, 0.05) - exact);
    check("Forward: halving h halves the error (first order)", r_fwd > 1.8 && r_fwd < 2.2);
    check("Central: halving h divides the error by about 4", r_cen > 3.8 && r_cen < 4.2);
    check("Second: halving h divides the error by about 4", r_sec > 3.8 && r_sec < 4.2);
    check("Richardson: halving h divides the error by about 16", r_ric > 14.0 && r_ric < 18.0);
}

/// Edge cases: negative x, x = 0, and a function with a corner.
void edge_cases()
{
    numcomp::NumericalDifferentiator d;
    check("Central: works at a negative x (d/dx x^2 at -2 is -4)", close(d.central_difference([](double x) { return x * x; }, -2.0, 1e-3), -4.0, 1e-9));
    check("Second: constant function gives 0", close(d.second_derivative([](double) { return 7.0; }, 0.0, 0.1), 0.0, 1e-12));
    // |x| has no derivative at 0: the one-sided formulas see the two different slopes.
    auto abs_f = [](double x) { return std::abs(x); };
    check("Forward at the corner of |x| gives the right slope +1", close(d.forward_difference(abs_f, 0.0, 0.1), 1.0, 1e-12));
    check("Backward at the corner of |x| gives the left slope -1", close(d.backward_difference(abs_f, 0.0, 0.1), -1.0, 1e-12));
    check("Central at the corner of |x| gives the average 0", close(d.central_difference(abs_f, 0.0, 0.1), 0.0, 1e-12));
}

/// Invalid x or h throws invalid_argument; a non-finite result throws runtime_error.
void errors()
{
    numcomp::NumericalDifferentiator d;
    auto sine = [](double x) { return std::sin(x); };
    double nan = std::numeric_limits<double>::quiet_NaN();
    double inf = std::numeric_limits<double>::infinity();

    test::expect_throw<std::invalid_argument>("Forward: h = 0 throws invalid_argument", [&] { d.forward_difference(sine, 1.0, 0.0); });
    test::expect_throw<std::invalid_argument>("Backward: h < 0 throws invalid_argument", [&] { d.backward_difference(sine, 1.0, -0.1); });
    test::expect_throw<std::invalid_argument>("Central: h = NaN throws invalid_argument", [&] { d.central_difference(sine, 1.0, nan); });
    test::expect_throw<std::invalid_argument>("Second: h = infinity throws invalid_argument", [&] { d.second_derivative(sine, 1.0, inf); });
    test::expect_throw<std::invalid_argument>("Richardson: x = NaN throws invalid_argument", [&] { d.richardson_extrapolation(sine, nan, 0.1); });
    test::expect_throw<std::invalid_argument>("Central: x = infinity throws invalid_argument", [&] { d.central_difference(sine, inf, 0.1); });
    test::expect_throw<std::runtime_error>("Central: log(x) at x - h < 0 gives NaN (runtime_error)", [&] { d.central_difference([](double x) { return std::log(x); }, 0.05, 0.1); });
    test::expect_throw<std::runtime_error>("Forward: f overflowing throws runtime_error", [&] { d.forward_difference([](double x) { return std::exp(x); }, 800.0, 1.0); });
}

/**
 * @brief Runs every test group.
 * @return 0 if all checks passed, 1 otherwise.
 */
int main()
{
    std::cout << "[*] Running Numerical Differentiator Tests..." << std::endl;
    accuracy();
    edge_cases();
    errors();
    return test::finish("Numerical Differentiator");
}
