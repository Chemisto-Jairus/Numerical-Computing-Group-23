/**
 * @file test_interpolator.cpp
 * @brief Unit tests for numcomp::Interpolator (linear interpolation, Chebyshev nodes
 *        and Chebyshev approximation).
 * @author Batte Derrick Calvin (Member 2)
 *
 * Covers normal use, edge cases and invalid input. Each check prints [PASS] or [FAIL];
 * the program returns 0 only if every check passed (see test_helpers.hpp).
 */

#include "numerical-computing/interpolator.hpp"
#include "test_helpers.hpp"
#include <algorithm>
#include <cmath>
#include <stdexcept>
#include <vector>

using test::check;
using test::close;

/// Linear interpolation on normal data and at the data end points.
void linear_normal_and_edge()
{
    numcomp::Interpolator interp;
    std::vector<double> x = {0.0, 1.0, 2.0};
    std::vector<double> y = {5.0, 7.0, 3.0};

    check("Linear: y = 2x at x = 1.5 gives 3", close(interp.linear_interpolate(1.5, {0.0, 1.0, 2.0, 3.0}, {0.0, 2.0, 4.0, 6.0}), 3.0, 1e-12));
    // Between (1,1) and (4,16): 1 + 15*(2-1)/3 = 6
    check("Linear: uneven spacing gives 6 at x = 2", close(interp.linear_interpolate(2.0, {0.0, 1.0, 4.0}, {0.0, 1.0, 16.0}), 6.0, 1e-12));
    check("Linear: negative x and y", close(interp.linear_interpolate(-1.5, {-2.0, -1.0, 0.0}, {4.0, -1.0, 0.0}), 1.5, 1e-12));
    check("Linear: x at first data point", close(interp.linear_interpolate(0.0, x, y), 5.0, 1e-12));
    check("Linear: x at last data point", close(interp.linear_interpolate(2.0, x, y), 3.0, 1e-12));
    check("Linear: x exactly on an inner point", close(interp.linear_interpolate(1.0, x, y), 7.0, 1e-12));
    check("Linear: works with only 2 points", close(interp.linear_interpolate(0.25, {0.0, 1.0}, {0.0, 4.0}), 1.0, 1e-12));
}

/// Linear interpolation with invalid data or an out-of-range x.
void linear_invalid()
{
    numcomp::Interpolator interp;
    test::expect_throw<std::invalid_argument>("Linear: different sizes throw invalid_argument", [&] { interp.linear_interpolate(0.5, {0.0, 1.0, 2.0}, {0.0, 1.0}); });
    test::expect_throw<std::invalid_argument>("Linear: one point throws invalid_argument", [&] { interp.linear_interpolate(0.0, {0.0}, {1.0}); });
    test::expect_throw<std::invalid_argument>("Linear: unsorted x throws invalid_argument", [&] { interp.linear_interpolate(1.5, {0.0, 2.0, 1.0}, {0.0, 4.0, 1.0}); });
    test::expect_throw<std::invalid_argument>("Linear: repeated x throws invalid_argument", [&] { interp.linear_interpolate(1.0, {0.0, 1.0, 1.0, 2.0}, {0.0, 1.0, 2.0, 3.0}); });
    test::expect_throw<std::out_of_range>("Linear: x below range throws out_of_range", [&] { interp.linear_interpolate(-1.0, {0.0, 1.0}, {0.0, 1.0}); });
    test::expect_throw<std::out_of_range>("Linear: x above range throws out_of_range", [&] { interp.linear_interpolate(1.5, {0.0, 1.0}, {0.0, 1.0}); });
    // NaN must be rejected by the strictly-increasing check.
    test::expect_throw<std::invalid_argument>("Linear: NaN in x values throws invalid_argument", [&] { interp.linear_interpolate(0.5, {0.0, std::nan(""), 2.0}, {0.0, 1.0, 2.0}); });
}

/// Chebyshev node values, placement and argument checks.
void chebyshev()
{
    numcomp::Interpolator interp;

    std::vector<double> two = interp.chebyshev_nodes(-1.0, 1.0, 2);
    double r = std::sqrt(2.0) / 2.0;     // cos(pi/4)
    check("Chebyshev: n = 2 on [-1,1] gives +/- 0.7071", two.size() == 2 && close(two[0], r, 1e-12) && close(two[1], -r, 1e-12));

    std::vector<double> seven = interp.chebyshev_nodes(2.0, 5.0, 7);
    bool inside = seven.size() == 7;
    for (double node : seven)
    {
        if (node <= 2.0 || node >= 5.0) inside = false;
    }
    check("Chebyshev: all 7 nodes lie strictly inside [2, 5]", inside);

    std::vector<double> one = interp.chebyshev_nodes(0.0, 4.0, 1);
    check("Chebyshev: n = 1 gives the midpoint", one.size() == 1 && close(one[0], 2.0, 1e-12));

    test::expect_throw<std::invalid_argument>("Chebyshev: n = 0 throws invalid_argument", [&] { interp.chebyshev_nodes(0.0, 1.0, 0); });
    test::expect_throw<std::invalid_argument>("Chebyshev: n < 0 throws invalid_argument", [&] { interp.chebyshev_nodes(0.0, 1.0, -3); });
    test::expect_throw<std::invalid_argument>("Chebyshev: a == b throws invalid_argument", [&] { interp.chebyshev_nodes(1.0, 1.0, 3); });
    test::expect_throw<std::invalid_argument>("Chebyshev: a > b throws invalid_argument", [&] { interp.chebyshev_nodes(2.0, 1.0, 3); });
    // The node count is capped so the allocation stays bounded.
    test::expect_throw<std::invalid_argument>("Chebyshev: n > 1,000,000 throws invalid_argument", [&] { interp.chebyshev_nodes(0.0, 1.0, 1000000001); });
    test::expect_throw<std::invalid_argument>("Chebyshev: NaN end point throws invalid_argument", [&] { interp.chebyshev_nodes(std::nan(""), 1.0, 3); });
}

/// Chebyshev approximation: accuracy for smooth functions, exactness for polynomials, invalid input.
void chebyshev_approximation()
{
    numcomp::Interpolator interp;
    auto exp_f = [](double x) { return std::exp(x); };

    // e^x is smooth, so 12 coefficients reach almost full double precision.
    std::vector<double> c = interp.chebyshev_coefficients(exp_f, 0.0, 1.0, 12);
    double worst = 0.0;
    for (int k = 0; k <= 20; ++k)
    {
        double x = k / 20.0;
        worst = std::max(worst, std::abs(interp.chebyshev_approximate(c, 0.0, 1.0, x) - std::exp(x)));
    }
    check("Chebyshev approx: e^x on [0, 1] with 12 terms, error < 1e-13", worst < 1e-13);
    check("Chebyshev approx: coefficients of e^x shrink quickly", std::abs(c[11]) < 1e-12 && std::abs(c[1]) > 0.1);

    // A polynomial of degree 3 is reproduced exactly with 4 terms.
    auto cubic = [](double x) { return 2.0 * x * x * x - x + 1.0; };
    std::vector<double> cc = interp.chebyshev_coefficients(cubic, -2.0, 3.0, 4);
    check("Chebyshev approx: cubic reproduced exactly at x = 0.7", close(interp.chebyshev_approximate(cc, -2.0, 3.0, 0.7), cubic(0.7), 1e-11));

    // Edge cases: interval end points, and a single coefficient (a constant).
    check("Chebyshev approx: exact at the left end a", close(interp.chebyshev_approximate(c, 0.0, 1.0, 0.0), 1.0, 1e-13));
    check("Chebyshev approx: exact at the right end b", close(interp.chebyshev_approximate(c, 0.0, 1.0, 1.0), std::exp(1.0), 1e-13));
    std::vector<double> one = interp.chebyshev_coefficients([](double) { return 5.0; }, 0.0, 1.0, 1);
    check("Chebyshev approx: n = 1 gives the constant 5", close(interp.chebyshev_approximate(one, 0.0, 1.0, 0.3), 5.0, 1e-13));

    // More terms give a smaller error for the Runge function (no oscillation at Chebyshev nodes).
    auto runge = [](double x) { return 1.0 / (1.0 + 25.0 * x * x); };
    double err10 = 0.0, err40 = 0.0;
    std::vector<double> r10 = interp.chebyshev_coefficients(runge, -1.0, 1.0, 10);
    std::vector<double> r40 = interp.chebyshev_coefficients(runge, -1.0, 1.0, 40);
    for (int k = 0; k <= 100; ++k)
    {
        double x = -1.0 + k * 0.02;
        err10 = std::max(err10, std::abs(interp.chebyshev_approximate(r10, -1.0, 1.0, x) - runge(x)));
        err40 = std::max(err40, std::abs(interp.chebyshev_approximate(r40, -1.0, 1.0, x) - runge(x)));
    }
    check("Chebyshev approx: Runge function error falls from 10 to 40 terms", err40 < err10 / 10.0);

    // Invalid input
    test::expect_throw<std::invalid_argument>("Chebyshev approx: n = 0 throws invalid_argument", [&] { interp.chebyshev_coefficients(exp_f, 0.0, 1.0, 0); });
    test::expect_throw<std::invalid_argument>("Chebyshev approx: n > 1000 throws invalid_argument", [&] { interp.chebyshev_coefficients(exp_f, 0.0, 1.0, 1001); });
    test::expect_throw<std::invalid_argument>("Chebyshev approx: a > b throws invalid_argument", [&] { interp.chebyshev_coefficients(exp_f, 1.0, 0.0, 5); });
    test::expect_throw<std::runtime_error>("Chebyshev approx: f returning NaN throws runtime_error", [&] { interp.chebyshev_coefficients([](double x) { return std::log(x); }, -1.0, 1.0, 4); });
    test::expect_throw<std::invalid_argument>("Chebyshev approx: empty coefficients throw invalid_argument", [&] { interp.chebyshev_approximate({}, 0.0, 1.0, 0.5); });
    test::expect_throw<std::out_of_range>("Chebyshev approx: x outside [a, b] throws out_of_range", [&] { interp.chebyshev_approximate(c, 0.0, 1.0, 1.5); });
    test::expect_throw<std::out_of_range>("Chebyshev approx: NaN x throws out_of_range", [&] { interp.chebyshev_approximate(c, 0.0, 1.0, std::nan("")); });
}

/**
 * @brief Runs every test group.
 * @return 0 if all checks passed, 1 otherwise.
 */
int main()
{
    std::cout << "[*] Running Interpolator Tests..." << std::endl;
    linear_normal_and_edge();
    linear_invalid();
    chebyshev();
    chebyshev_approximation();
    return test::finish("Interpolator");
}
