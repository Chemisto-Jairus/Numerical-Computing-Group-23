/**
 * @file test_finite_difference_solver.cpp
 * @brief Unit tests for numcomp::FiniteDifferenceSolver (linear boundary value problems).
 * @author Aogon Sharon (Member 3)
 *
 * Covers normal use, edge cases and invalid input. Each check prints [PASS] or [FAIL];
 * the program returns 0 only if every check passed (see test_helpers.hpp).
 */

#include "numerical-computing/finite_difference_solver.hpp"
#include "test_helpers.hpp"
#include <algorithm>
#include <cmath>
#include <functional>
#include <limits>
#include <stdexcept>
#include <utility>
#include <vector>

using test::check;
using test::close;

/**
 * @brief Largest difference between a computed grid and the exact solution.
 * @param grid   Grid points (x_i, y_i).
 * @param exact  Exact solution y(x).
 * @return max_i |y_i - exact(x_i)|.
 */
double max_error(const std::vector<std::pair<double, double>>& grid, const std::function<double(double)>& exact)
{
    double worst = 0.0;
    for (const std::pair<double, double>& point : grid)
    {
        worst = std::max(worst, std::abs(point.second - exact(point.first)));
    }
    return worst;
}

/// Accuracy on standard problems and the second-order convergence.
void accuracy()
{
    numcomp::FiniteDifferenceSolver fd;
    const double pi = std::acos(-1.0);
    auto load = [=](double x) { return -pi * pi * std::sin(pi * x); };
    auto exact_sin = [=](double x) { return std::sin(pi * x); };

    check("Poisson y'' = -pi^2 sin(pi x): n = 100 error below 1e-4", max_error(fd.poisson(load, 0.0, 1.0, 0.0, 0.0, 100), exact_sin) < 1e-4);

    // A quadratic solution is reproduced exactly: the central difference is exact for quadratics.
    auto quad = fd.poisson([](double) { return 2.0; }, 0.0, 1.0, 0.0, 1.0, 10);
    check("y'' = 2, y(0) = 0, y(1) = 1 gives y = x^2 exactly", max_error(quad, [](double x) { return x * x; }) < 1e-12);

    // y'' = y (p = 0, q = 1, r = 0) on [0, 1], y(0) = 0, y(1) = sinh(1): exact y = sinh(x).
    auto zero = [](double) { return 0.0; };
    auto one  = [](double) { return 1.0; };
    auto grid = fd.linear_bvp(zero, one, zero, 0.0, 1.0, 0.0, std::sinh(1.0), 50);
    check("y'' = y gives sinh(x) (error below 1e-5)", max_error(grid, [](double x) { return std::sinh(x); }) < 1e-5);

    // Burden & Faires example: y'' = -(2/x) y' + (2/x^2) y + sin(ln x)/x^2 on [1, 2], y(1) = 1, y(2) = 2.
    double c2 = (8.0 - 12.0 * std::sin(std::log(2.0)) - 4.0 * std::cos(std::log(2.0))) / 70.0;
    double c1 = 1.1 - c2;
    auto exact_bf = [=](double x) { return c1 * x + c2 / (x * x) - 0.3 * std::sin(std::log(x)) - 0.1 * std::cos(std::log(x)); };
    auto bf = fd.linear_bvp([](double x) { return -2.0 / x; }, [](double x) { return 2.0 / (x * x); },
                            [](double x) { return std::sin(std::log(x)) / (x * x); }, 1.0, 2.0, 1.0, 2.0, 10);
    check("Textbook problem with variable p, q, r: n = 10 error below 1e-4", max_error(bf, exact_bf) < 1e-4);

    double e1 = max_error(fd.poisson(load, 0.0, 1.0, 0.0, 0.0, 20), exact_sin);
    double e2 = max_error(fd.poisson(load, 0.0, 1.0, 0.0, 0.0, 40), exact_sin);
    check("Doubling n divides the error by about 4 (second order)", e1 / e2 > 3.8 && e1 / e2 < 4.2);
}

/// Grid structure, the smallest grid and non-zero boundary values.
void boundaries()
{
    numcomp::FiniteDifferenceSolver fd;
    auto grid = fd.poisson([](double) { return 0.0; }, -1.0, 3.0, 5.0, -3.0, 8);
    check("n = 8 gives 9 grid points", grid.size() == 9);
    check("First point is (a, alpha)", grid.front().first == -1.0 && grid.front().second == 5.0);
    check("Last point is exactly (b, beta)", grid.back().first == 3.0 && grid.back().second == -3.0);
    check("y'' = 0 gives the straight line between the boundary values", close(grid[4].second, 1.0, 1e-12));

    auto smallest = fd.poisson([](double) { return 2.0; }, 0.0, 2.0, 0.0, 4.0, 2);
    check("n = 2 (one inner point) gives y(1) = 1 for y = x^2", smallest.size() == 3 && close(smallest[1].second, 1.0, 1e-12));
}

/// Invalid grids and boundary values throw invalid_argument; bad functions throw runtime_error.
void errors()
{
    numcomp::FiniteDifferenceSolver fd;
    auto r = [](double) { return 1.0; };
    double nan = std::numeric_limits<double>::quiet_NaN();
    double inf = std::numeric_limits<double>::infinity();

    test::expect_throw<std::invalid_argument>("n = 0 throws invalid_argument", [&] { fd.poisson(r, 0.0, 1.0, 0.0, 0.0, 0); });
    test::expect_throw<std::invalid_argument>("n = 1 (no inner point) throws invalid_argument", [&] { fd.poisson(r, 0.0, 1.0, 0.0, 0.0, 1); });
    test::expect_throw<std::invalid_argument>("n > 1,000,000 throws invalid_argument", [&] { fd.poisson(r, 0.0, 1.0, 0.0, 0.0, 2000000); });
    test::expect_throw<std::invalid_argument>("a == b throws invalid_argument", [&] { fd.poisson(r, 1.0, 1.0, 0.0, 0.0, 10); });
    test::expect_throw<std::invalid_argument>("a > b throws invalid_argument", [&] { fd.poisson(r, 2.0, 1.0, 0.0, 0.0, 10); });
    test::expect_throw<std::invalid_argument>("a = NaN throws invalid_argument", [&] { fd.poisson(r, nan, 1.0, 0.0, 0.0, 10); });
    test::expect_throw<std::invalid_argument>("alpha = infinity throws invalid_argument", [&] { fd.poisson(r, 0.0, 1.0, inf, 0.0, 10); });
    test::expect_throw<std::invalid_argument>("beta = NaN throws invalid_argument", [&] { fd.poisson(r, 0.0, 1.0, 0.0, nan, 10); });
    test::expect_throw<std::runtime_error>("r(x) = 1/x with a grid point at x = 0 throws runtime_error",
        [&] { fd.poisson([](double x) { return 1.0 / x; }, -1.0, 1.0, 0.0, 0.0, 10); });
    // With h = 0.5, q = -8 makes the diagonal entry 2 + h^2 q zero: a singular system.
    test::expect_throw<std::runtime_error>("Singular system (zero pivot) throws runtime_error",
        [&] { fd.linear_bvp([](double) { return 0.0; }, [](double) { return -8.0; }, r, 0.0, 1.0, 0.0, 0.0, 2); });
}

/**
 * @brief Runs every test group.
 * @return 0 if all checks passed, 1 otherwise.
 */
int main()
{
    std::cout << "[*] Running Finite Difference Solver Tests..." << std::endl;
    accuracy();
    boundaries();
    errors();
    return test::finish("Finite Difference Solver");
}
