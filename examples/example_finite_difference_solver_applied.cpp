/**
 * @file example_finite_difference_solver_applied.cpp
 * @brief Demo 2 of 2 for numcomp::FiniteDifferenceSolver: applied boundary value problems.
 * @author Aogon Sharon (Member 3)
 *
 * - Steady temperature in a cooling fin that loses heat to the air.
 * - Sag of a cable under its own load between two supports.
 */

#include "numerical-computing/finite_difference_solver.hpp"
#include "example_helpers.hpp"
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
    demo::title("Finite Difference Solver (applied)", "Cooling fin and a hanging cable");

    demo::section("Fin 0.1 m long: T'' = 400 (T - 25), T(0) = 100 deg C, T(0.1) = 40 deg C", [&] {
        const double m2 = 400.0, air = 25.0, L = 0.1;
        // In the form y'' = p y' + q y + r: p = 0, q = 400, r = -400 * 25.
        auto grid = fd.linear_bvp([](double) { return 0.0; }, [=](double) { return m2; }, [=](double) { return -m2 * air; },
                                  0.0, L, 100.0, 40.0, 50);
        // Exact: T = 25 + A cosh(20 x) + B sinh(20 x), with A and B from the two end temperatures.
        double m = std::sqrt(m2);
        double A = 75.0;
        double B = (15.0 - A * std::cosh(m * L)) / std::sinh(m * L);
        demo::table_header({"x (m)", "T (grid)", "exact T"});
        for (std::size_t i = 0; i < grid.size(); i += 10)
        {
            double x = grid[i].first;
            demo::table_row({x, grid[i].second, air + A * std::cosh(m * x) + B * std::sinh(m * x)}, 8);
        }
    });

    demo::section("Cable over 40 m, tension 5000 N, load 50 N/m: y'' = 50 / 5000, ends at 10 m height", [&] {
        auto grid = fd.poisson([](double) { return 0.01; }, 0.0, 40.0, 10.0, 10.0, 40);
        double lowest = grid[0].second, where = 0.0;
        for (const std::pair<double, double>& p : grid)
        {
            if (p.second < lowest) { lowest = p.second; where = p.first; }
        }
        demo::compare("Lowest point height (m)", lowest, 10.0 - 0.01 * 20.0 * 20.0 / 2.0);
        demo::value("Lowest point at x (m)", where, 2);
        std::cout << "  The exact sag is a parabola, which the central difference reproduces exactly.\n";
    });

    return demo::finish();
}
