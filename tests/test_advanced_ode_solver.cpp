/**
 * @file test_advanced_ode_solver.cpp
 * @brief Unit tests for numcomp::AdvancedODESolver (RK3 and RK4).
 * @author Muwanguzi Samuel (Member 7)
 *
 * Covers normal use, edge cases and invalid input. Each check prints [PASS] or [FAIL];
 * the program returns 0 only if every check passed (see test_helpers.hpp).
 */

#include "numerical-computing/advanced_ode_solver.hpp"
#include "test_helpers.hpp"
#include <cmath>
#include <limits>
#include <stdexcept>
#include <vector>
#include <utility>

using test::check;
using test::close;

// Right-hand sides f(t, y) of the test problems dy/dt = f(t, y).
/// @brief Test function: returns 4.0 * t * t * t (exact solution y = t^4).
/// @param t  Time.
/// @return The function value.
double cubic_rhs(double t, double) { return 4.0 * t * t * t; }
/// @brief Test function: returns -std::sin(t) (exact solution y = cos(t)).
/// @param t  Time.
/// @return The function value.
double sin_rhs(double t, double) { return -std::sin(t); }
/// @brief Test function: returns -2.0 * y (exact solution y = e^(-2t)).
/// @param y  Current value.
/// @return The function value.
double decay_rhs(double, double y) { return -2.0 * y; }

/// Accuracy on polynomial, trigonometric and exponential problems, plus the observed order of convergence.
void accuracy()
{
    numcomp::AdvancedODESolver solver;
    const double pi = std::acos(-1.0);
    double exact = std::exp(-2.0);

    // Polynomial: when f depends only on t, RK3 and RK4 act like Simpson's rule (exact for cubics)
    check("RK4: dy/dt = 4t^3 gives y(2) = 16 exactly", close(solver.rk4(cubic_rhs, 0.0, 0.0, 2.0, 0.1).back().second, 16.0, 1e-10));
    check("RK3: dy/dt = 4t^3 gives y(2) = 16 exactly", close(solver.rk3(cubic_rhs, 0.0, 0.0, 2.0, 0.1).back().second, 16.0, 1e-10));

    // Trig
    check("RK4: dy/dt = -sin(t) gives cos(pi) = -1", close(solver.rk4(sin_rhs, 0.0, 1.0, pi, 0.01).back().second, -1.0, 1e-9));
    check("RK3: dy/dt = -sin(t) gives cos(pi) = -1", close(solver.rk3(sin_rhs, 0.0, 1.0, pi, 0.01).back().second, -1.0, 1e-8));

    // Decay
    check("RK4: decay dy/dt = -2y, h = 0.1, close to e^-2", close(solver.rk4(decay_rhs, 0.0, 1.0, 1.0, 0.1).back().second, exact, 1e-5));
    check("RK3: decay dy/dt = -2y, h = 0.1, close to e^-2", close(solver.rk3(decay_rhs, 0.0, 1.0, 1.0, 0.1).back().second, exact, 1e-3));

    // Order of accuracy: halving h divides RK3's error by about 8 and RK4's by about 16
    double rk3_ratio = std::abs(solver.rk3(decay_rhs, 0.0, 1.0, 1.0, 0.1).back().second - exact)
                     / std::abs(solver.rk3(decay_rhs, 0.0, 1.0, 1.0, 0.05).back().second - exact);
    double rk4_ratio = std::abs(solver.rk4(decay_rhs, 0.0, 1.0, 1.0, 0.1).back().second - exact)
                     / std::abs(solver.rk4(decay_rhs, 0.0, 1.0, 1.0, 0.05).back().second - exact);
    check("RK3: halving h divides the error by about 8", rk3_ratio > 7.0 && rk3_ratio < 9.0);
    check("RK4: halving h divides the error by about 16", rk4_ratio > 14.0 && rk4_ratio < 18.0);
}

/// Structure of the returned curve: first point, number of points, exact final time.
void boundaries()
{
    numcomp::AdvancedODESolver solver;
    std::vector<std::pair<double, double>> curve = solver.rk4(decay_rhs, 0.0, 1.0, 1.0, 0.1);
    check("RK4: first point is (t0, y0)", curve.front().first == 0.0 && curve.front().second == 1.0);
    check("RK4: h = 0.1 on [0, 1] gives 11 points", curve.size() == 11);
    check("RK4: last t is exactly t_end", curve.back().first == 1.0);

    std::vector<std::pair<double, double>> uneven = solver.rk3(decay_rhs, 0.0, 1.0, 1.0, 0.3);
    check("RK3: h = 0.3 gives 5 points ending exactly at 1", uneven.size() == 5 && uneven.back().first == 1.0);

    std::vector<std::pair<double, double>> one_step = solver.rk4(decay_rhs, 0.0, 1.0, 1.0, 5.0);
    check("RK4: h bigger than the interval gives one step to t_end", one_step.size() == 2 && one_step.back().first == 1.0);
}

/// Every invalid time interval or step size must throw std::invalid_argument.
void errors()
{
    numcomp::AdvancedODESolver solver;
    double nan = std::numeric_limits<double>::quiet_NaN();

    // The same six invalid inputs are applied to both methods.
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
        test::expect_throw<std::invalid_argument>(std::string("RK4: ") + b.label + " throws invalid_argument", [&] { solver.rk4(decay_rhs, b.t0, 1.0, b.t_end, b.h); });
    }
    for (const BadInput& b : bad)
    {
        test::expect_throw<std::invalid_argument>(std::string("RK3: ") + b.label + " throws invalid_argument", [&] { solver.rk3(decay_rhs, b.t0, 1.0, b.t_end, b.h); });
    }
}

/**
 * @brief Runs every test group.
 * @return 0 if all checks passed, 1 otherwise.
 */
int main()
{
    std::cout << "[*] Running Advanced ODE Solver Tests..." << std::endl;
    accuracy();
    boundaries();
    errors();
    return test::finish("Advanced ODE Solver");
}
