/**
 * @file test_basic_ode_solver.cpp
 * @brief Unit tests for numcomp::BasicODESolver (Euler and Heun).
 * @author Mwanje Ian (Member 6)
 *
 * Covers normal use, edge cases and invalid input. Each check prints [PASS] or [FAIL];
 * the program returns 0 only if every check passed (see test_helpers.hpp).
 */

#include "numerical-computing/basic_ode_solver.hpp"
#include "test_helpers.hpp"
#include <cmath>
#include <limits>
#include <stdexcept>
#include <vector>
#include <utility>

using test::check;
using test::close;

// Right-hand sides f(t, y) of the test problems dy/dt = f(t, y).
/// @brief Test function: returns 1.0 (exact solution y = t).
/// @return The function value.
double constant_rhs(double, double) { return 1.0; }
/// @brief Test function: returns 2.0 * t (exact solution y = t^2).
/// @param t  Time.
/// @return The function value.
double linear_rhs(double t, double) { return 2.0 * t; }
/// @brief Test function: returns std::cos(t) (exact solution y = sin(t)).
/// @param t  Time.
/// @return The function value.
double cos_rhs(double t, double) { return std::cos(t); }
/// @brief Test function: returns y (exact solution y = e^t).
/// @param y  Current value.
/// @return The function value.
double growth_rhs(double, double y) { return y; }

/// Accuracy on polynomial, trigonometric and exponential problems, plus the observed order of convergence.
void accuracy()
{
    numcomp::BasicODESolver solver;
    double e = std::exp(1.0);

    // Polynomial
    check("Euler: dy/dt = 1 gives y(1) = 1 exactly", close(solver.euler(constant_rhs, 0.0, 0.0, 1.0, 0.1).back().second, 1.0, 1e-12));
    // Euler with h = 0.1 gives exactly h^2 * n(n-1) = 0.9 (a known first-order error)
    check("Euler: dy/dt = 2t with h = 0.1 gives 0.9 at t = 1", close(solver.euler(linear_rhs, 0.0, 0.0, 1.0, 0.1).back().second, 0.9, 1e-12));
    // Heun averages the slopes, so it is exact when f is linear in t
    check("Heun: dy/dt = 2t gives y(1) = 1 exactly", close(solver.heun(linear_rhs, 0.0, 0.0, 1.0, 0.1).back().second, 1.0, 1e-12));

    // Trig
    check("Euler: dy/dt = cos(t), h = 0.001, close to sin(2)", close(solver.euler(cos_rhs, 0.0, 0.0, 2.0, 0.001).back().second, std::sin(2.0), 1e-3));
    check("Heun: dy/dt = cos(t), h = 0.01, close to sin(2)", close(solver.heun(cos_rhs, 0.0, 0.0, 2.0, 0.01).back().second, std::sin(2.0), 1e-4));

    // Exponential
    check("Euler: dy/dt = y, h = 0.001, close to e", close(solver.euler(growth_rhs, 0.0, 1.0, 1.0, 0.001).back().second, e, 2e-3));
    check("Heun: dy/dt = y, h = 0.01, close to e", close(solver.heun(growth_rhs, 0.0, 1.0, 1.0, 0.01).back().second, e, 1e-4));

    // Order of accuracy: halving h halves Euler's error and quarters Heun's
    double euler_ratio = std::abs(solver.euler(growth_rhs, 0.0, 1.0, 1.0, 0.01).back().second - e)
                       / std::abs(solver.euler(growth_rhs, 0.0, 1.0, 1.0, 0.005).back().second - e);
    double heun_ratio = std::abs(solver.heun(growth_rhs, 0.0, 1.0, 1.0, 0.01).back().second - e)
                      / std::abs(solver.heun(growth_rhs, 0.0, 1.0, 1.0, 0.005).back().second - e);
    check("Euler: halving h halves the error (ratio close to 2)", euler_ratio > 1.8 && euler_ratio < 2.2);
    check("Heun: halving h quarters the error (ratio close to 4)", heun_ratio > 3.6 && heun_ratio < 4.4);
}

/// Structure of the returned curve: first point, number of points, exact final time.
void curve_shape()
{
    numcomp::BasicODESolver solver;
    std::vector<std::pair<double, double>> curve = solver.euler(growth_rhs, 0.0, 1.0, 1.0, 0.1);
    check("Euler: first point is (t0, y0)", curve.front().first == 0.0 && curve.front().second == 1.0);
    check("Euler: h = 0.1 on [0, 1] gives 11 points", curve.size() == 11);
    check("Euler: last t is exactly t_end", curve.back().first == 1.0);

    // h = 0.3 does not divide [0, 1] evenly: t = 0, 0.3, 0.6, 0.9, 1.0
    std::vector<std::pair<double, double>> uneven = solver.heun(growth_rhs, 0.0, 1.0, 1.0, 0.3);
    check("Heun: h = 0.3 gives 5 points ending exactly at 1", uneven.size() == 5 && uneven.back().first == 1.0);

    std::vector<std::pair<double, double>> one_step = solver.euler(constant_rhs, 0.0, 0.0, 1.0, 5.0);
    check("Euler: h bigger than the interval gives one step to t_end", one_step.size() == 2 && one_step.back().first == 1.0);

    check("Heun: works from t0 = -1 to t_end = 1 (dy/dt = 2t, y(-1) = 1)", close(solver.heun(linear_rhs, -1.0, 1.0, 1.0, 0.1).back().second, 1.0, 1e-12));
}

/// Every invalid time interval or step size must throw std::invalid_argument.
void errors()
{
    numcomp::BasicODESolver solver;
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
        test::expect_throw<std::invalid_argument>(std::string("Euler: ") + b.label + " throws invalid_argument", [&] { solver.euler(growth_rhs, b.t0, 1.0, b.t_end, b.h); });
    }
    for (const BadInput& b : bad)
    {
        test::expect_throw<std::invalid_argument>(std::string("Heun: ") + b.label + " throws invalid_argument", [&] { solver.heun(growth_rhs, b.t0, 1.0, b.t_end, b.h); });
    }
}

/**
 * @brief Runs every test group.
 * @return 0 if all checks passed, 1 otherwise.
 */
int main()
{
    std::cout << "[*] Running Basic ODE Solver Tests..." << std::endl;
    accuracy();
    curve_shape();
    errors();
    return test::finish("Basic ODE Solver");
}
