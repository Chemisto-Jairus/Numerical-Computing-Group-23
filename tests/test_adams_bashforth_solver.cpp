/**
 * @file test_adams_bashforth_solver.cpp
 * @brief Unit tests for numcomp::AdamsBashforthSolver (AB2 and AB4).
 * @author Mwanje Ian (Member 6)
 *
 * Covers normal use, edge cases and invalid input. Each check prints [PASS] or [FAIL];
 * the program returns 0 only if every check passed (see test_helpers.hpp).
 */

#include "numerical-computing/adams_bashforth_solver.hpp"
#include "test_helpers.hpp"
#include <cmath>
#include <limits>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

using test::check;
using test::close;

/// @brief Test function: returns -2.0 * y (exact solution y = e^(-2t)).
/// @param y  Current value.
/// @return The function value.
double decay_rhs(double, double y) { return -2.0 * y; }
/// @brief Test function: returns 3.0 * t * t (exact solution y = t^3).
/// @param t  Time.
/// @return The function value.
double square_rhs(double t, double) { return 3.0 * t * t; }
/// @brief Test function: returns 2.0 * t (exact solution y = t^2).
/// @param t  Time.
/// @return The function value.
double linear_rhs(double t, double) { return 2.0 * t; }
/// @brief Test function: returns -std::sin(t) (exact solution y = cos(t)).
/// @param t  Time.
/// @return The function value.
double sin_rhs(double t, double) { return -std::sin(t); }

/// Accuracy on exponential, polynomial and trigonometric problems, and the observed order.
void accuracy()
{
    numcomp::AdamsBashforthSolver ab;
    const double pi = std::acos(-1.0);
    double exact = std::exp(-2.0);

    check("AB4: decay dy/dt = -2y, h = 0.05, close to e^-2", close(ab.ab4(decay_rhs, 0.0, 1.0, 1.0, 0.05).back().second, exact, 1e-5));
    check("AB2: decay dy/dt = -2y, h = 0.05, close to e^-2", close(ab.ab2(decay_rhs, 0.0, 1.0, 1.0, 0.05).back().second, exact, 2e-3));
    // When f depends only on t, ABk is exact for slopes that are polynomials of degree k - 1.
    check("AB2: dy/dt = 2t gives y(2) = 4 exactly", close(ab.ab2(linear_rhs, 0.0, 0.0, 2.0, 0.1).back().second, 4.0, 1e-12));
    check("AB4: dy/dt = 3t^2 gives y(2) = 8 exactly", close(ab.ab4(square_rhs, 0.0, 0.0, 2.0, 0.1).back().second, 8.0, 1e-10));
    check("AB4: dy/dt = -sin t gives cos(pi) = -1", close(ab.ab4(sin_rhs, 0.0, 1.0, pi, 0.01).back().second, -1.0, 1e-8));

    double r2 = std::abs(ab.ab2(decay_rhs, 0.0, 1.0, 1.0, 0.05).back().second - exact)
              / std::abs(ab.ab2(decay_rhs, 0.0, 1.0, 1.0, 0.025).back().second - exact);
    double r4 = std::abs(ab.ab4(decay_rhs, 0.0, 1.0, 1.0, 0.05).back().second - exact)
              / std::abs(ab.ab4(decay_rhs, 0.0, 1.0, 1.0, 0.025).back().second - exact);
    check("AB2: halving h divides the error by about 4", r2 > 3.5 && r2 < 4.5);
    check("AB4: halving h divides the error by about 16", r4 > 13.0 && r4 < 19.0);
}

/// Shape of the returned curve, very short intervals and the shortened final step.
void boundaries()
{
    numcomp::AdamsBashforthSolver ab;
    std::vector<std::pair<double, double>> curve = ab.ab4(decay_rhs, 0.0, 1.0, 1.0, 0.1);
    check("AB4: first point is (t0, y0)", curve.front().first == 0.0 && curve.front().second == 1.0);
    check("AB4: h = 0.1 on [0, 1] gives 11 points", curve.size() == 11);
    check("AB4: last t is exactly t_end", curve.back().first == 1.0);

    // Fewer steps than the method needs: only the RK4 starter runs, and the answer is still accurate.
    std::vector<std::pair<double, double>> short_run = ab.ab4(decay_rhs, 0.0, 1.0, 0.2, 0.1);
    check("AB4: only 2 steps (all from the RK4 starter) still accurate", short_run.size() == 3 && close(short_run.back().second, std::exp(-0.4), 1e-5));

    std::vector<std::pair<double, double>> uneven = ab.ab2(decay_rhs, 0.0, 1.0, 1.0, 0.3);
    check("AB2: h = 0.3 gives 5 points ending exactly at 1", uneven.size() == 5 && uneven.back().first == 1.0);
    check("AB2: works on an interval that does not start at 0", close(ab.ab2(linear_rhs, 1.0, 1.0, 2.0, 0.01).back().second, 4.0, 1e-10));
}

/// Invalid time intervals and step sizes throw invalid_argument; blow-up throws runtime_error.
void errors()
{
    numcomp::AdamsBashforthSolver ab;
    double nan = std::numeric_limits<double>::quiet_NaN();

    struct BadInput { const char* label; double t0; double t_end; double h; };
    std::vector<BadInput> bad = {
        {"h = 0", 0.0, 1.0, 0.0},
        {"h < 0", 0.0, 1.0, -0.1},
        {"t_end == t0", 1.0, 1.0, 0.1},
        {"t_end < t0", 1.0, 0.0, 0.1},
        {"too many steps (h = 1e-7)", 0.0, 1.0, 1e-7},
        {"h = NaN", 0.0, 1.0, nan},
    };
    for (const BadInput& b : bad)
    {
        test::expect_throw<std::invalid_argument>(std::string("AB2: ") + b.label + " throws invalid_argument", [&] { ab.ab2(decay_rhs, b.t0, 1.0, b.t_end, b.h); });
        test::expect_throw<std::invalid_argument>(std::string("AB4: ") + b.label + " throws invalid_argument", [&] { ab.ab4(decay_rhs, b.t0, 1.0, b.t_end, b.h); });
    }

    // dy/dt = y^2, y(0) = 1 has the solution 1 / (1 - t), which becomes infinite at t = 1.
    test::expect_throw<std::runtime_error>("AB4: solution that blows up throws runtime_error",
        [&] { ab.ab4([](double, double y) { return y * y; }, 0.0, 1.0, 3.0, 0.01); });
}

/**
 * @brief Runs every test group.
 * @return 0 if all checks passed, 1 otherwise.
 */
int main()
{
    std::cout << "[*] Running Adams-Bashforth Solver Tests..." << std::endl;
    accuracy();
    boundaries();
    errors();
    return test::finish("Adams-Bashforth Solver");
}
