/**
 * @file example_finite_difference_solver.cpp
 * @brief Demo 1 of 2 for numcomp::FiniteDifferenceSolver: the finite difference method.
 * @author Aogon Sharon (Member 3)
 */

#include "numerical-computing/finite_difference_solver.hpp"
#include "example_helpers.hpp"
#include <algorithm>
#include <cmath>
#include <utility>
#include <vector>

/**
 * @brief Runs the demo sections.
 * @return 0 if every section completed, 1 otherwise.
 */
int main()
{
    numcomp::FiniteDifferenceSolver fd;
    const double pi = std::acos(-1.0);
    demo::title("Finite Difference Solver", "Linear two-point boundary value problems");

    demo::section("y'' = -pi^2 sin(pi x), y(0) = y(1) = 0, n = 10 (exact y = sin(pi x))", [&] {
        auto grid = fd.poisson([=](double x) { return -pi * pi * std::sin(pi * x); }, 0, 1, 0, 0, 10);
        demo::table_header({"x", "y (grid)", "exact", "error"});
        for (std::size_t i = 0; i < grid.size(); i += 2)
        {
            double e = std::sin(pi * grid[i].first);
            demo::table_row({grid[i].first, grid[i].second, e, std::abs(grid[i].second - e)}, 6);
        }
    });

    demo::section("Second order: doubling n divides the largest error by about 4", [&] {
        demo::table_header({"n", "max error"});
        for (int n : {10, 20, 40, 80})
        {
            auto grid = fd.poisson([=](double x) { return -pi * pi * std::sin(pi * x); }, 0, 1, 0, 0, n);
            double worst = 0.0;
            for (const std::pair<double, double>& p : grid) worst = std::max(worst, std::abs(p.second - std::sin(pi * p.first)));
            demo::table_row({static_cast<double>(n), worst}, 4);
        }
    });

    demo::section("Variable coefficients: y'' = -(2/x) y' + (2/x^2) y + sin(ln x)/x^2 on [1, 2]", [&] {
        double c2 = (8.0 - 12.0 * std::sin(std::log(2.0)) - 4.0 * std::cos(std::log(2.0))) / 70.0;
        double c1 = 1.1 - c2;
        auto exact = [=](double x) { return c1 * x + c2 / (x * x) - 0.3 * std::sin(std::log(x)) - 0.1 * std::cos(std::log(x)); };
        auto grid = fd.linear_bvp([](double x) { return -2.0 / x; }, [](double x) { return 2.0 / (x * x); },
                                  [](double x) { return std::sin(std::log(x)) / (x * x); }, 1, 2, 1, 2, 10);
        for (std::size_t i = 2; i < grid.size(); i += 3)
        {
            demo::compare("y(" + std::to_string(grid[i].first).substr(0, 3) + ")", grid[i].second, exact(grid[i].first));
        }
        std::cout << "  A textbook test problem (Burden & Faires) with y(1) = 1 and y(2) = 2.\n";
    });

    demo::section("Invalid input is rejected", [&] {
        auto r = [](double) { return 1.0; };
        demo::show_rejected("Only 1 interval (no inner point)", [&] { fd.poisson(r, 0, 1, 0, 0, 1); });
        demo::show_rejected("a greater than b", [&] { fd.poisson(r, 1, 0, 0, 0, 10); });
        demo::show_rejected("r(x) = 1/x with a grid point at x = 0", [&] { fd.poisson([](double x) { return 1.0 / x; }, -1, 1, 0, 0, 10); });
    });

    return demo::finish();
}
