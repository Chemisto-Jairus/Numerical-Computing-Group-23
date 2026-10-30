/**
 * @file test_runge_kutta_fehlberg_solver.cpp
 * @brief Unit tests for numcomp::RungeKuttaFehlbergSolver (adaptive RKF45).
 * @author Wasswa George Mpanga (Member 5)
 *
 * Covers normal use, edge cases and invalid input. Each check prints [PASS] or [FAIL];
 * the program returns 0 only if every check passed (see test_helpers.hpp).
 */

#include "numerical-computing/runge_kutta_fehlberg_solver.hpp"
#include "test_helpers.hpp"
#include <algorithm>
#include <cmath>
#include <limits>
#include <stdexcept>
#include <utility>
#include <vector>

using test::check;
using test::close;

/// @brief Test function: returns -2.0 * y (exact solution y = e^(-2t)).
/// @param y  Current value.
/// @return The function value.
double decay_rhs(double, double y) { return -2.0 * y; }

/// Accuracy follows the tolerance; the step size adapts to the solution.
void accuracy()
{
    numcomp::RungeKuttaFehlbergSolver rkf;
    double exact = std::exp(-2.0);

    double err_loose = std::abs(rkf.rkf45(decay_rhs, 0.0, 1.0, 1.0, 0.1, 1e-4).back().second - exact);
    double err_tight = std::abs(rkf.rkf45(decay_rhs, 0.0, 1.0, 1.0, 0.1, 1e-8).back().second - exact);
    check("tol = 1e-4: error below 1e-4", err_loose < 1e-4);
    check("tol = 1e-8: error below 1e-8", err_tight < 1e-8);
    check("A tighter tolerance needs more steps",
          rkf.rkf45(decay_rhs, 0.0, 1.0, 1.0, 0.1, 1e-8).size() > rkf.rkf45(decay_rhs, 0.0, 1.0, 1.0, 0.1, 1e-4).size());
    check("rkf45_value: dy/dt = y - t, y(0) = 2, gives e + 2 at t = 1",
          close(rkf.rkf45_value([](double t, double y) { return y - t; }, 0.0, 2.0, 1.0, 1e-9), std::exp(1.0) + 2.0, 1e-8));

    // dy/dt = 4t^3 has a fourth-order polynomial solution: one step of the 4th-order formula is exact.
    std::vector<std::pair<double, double>> poly = rkf.rkf45([](double t, double) { return 4.0 * t * t * t; }, 0.0, 0.0, 2.0, 2.0, 1e-6);
    check("Polynomial solution t^4: exact, and in very few steps", close(poly.back().second, 16.0, 1e-10) && poly.size() <= 3);

    // The solution of dy/dt = -50 (y - cos t) changes fast near t = 0 and slowly afterwards,
    // so the accepted steps must be much larger at the end than at the start.
    std::vector<std::pair<double, double>> curve = rkf.rkf45([](double t, double y) { return -50.0 * (y - std::cos(t)); }, 0.0, 0.0, 1.0, 0.01, 1e-6);
    double first_step = curve[1].first - curve[0].first;
    double largest_step = 0.0;
    for (std::size_t i = 1; i < curve.size(); ++i)
    {
        largest_step = std::max(largest_step, curve[i].first - curve[i-1].first);
    }
    check("Step size grows where the solution becomes smooth", largest_step > 3.0 * first_step);
}

/// Shape of the returned curve.
void boundaries()
{
    numcomp::RungeKuttaFehlbergSolver rkf;
    std::vector<std::pair<double, double>> curve = rkf.rkf45(decay_rhs, 0.0, 1.0, 1.0, 0.1, 1e-6);
    check("First point is (t0, y0)", curve.front().first == 0.0 && curve.front().second == 1.0);
    check("Last t is exactly t_end", curve.back().first == 1.0);
    bool increasing = true;
    for (std::size_t i = 1; i < curve.size(); ++i)
    {
        if (!(curve[i].first > curve[i-1].first)) increasing = false;
    }
    check("Time values strictly increase", increasing);
    check("h_initial bigger than the interval still ends at t_end", rkf.rkf45(decay_rhs, 0.0, 1.0, 1.0, 10.0, 1e-6).back().first == 1.0);
}

/// Invalid input throws invalid_argument; an impossible problem throws runtime_error.
void errors()
{
    numcomp::RungeKuttaFehlbergSolver rkf;
    double nan = std::numeric_limits<double>::quiet_NaN();

    test::expect_throw<std::invalid_argument>("h_initial = 0 throws invalid_argument", [&] { rkf.rkf45(decay_rhs, 0.0, 1.0, 1.0, 0.0, 1e-6); });
    test::expect_throw<std::invalid_argument>("h_initial < 0 throws invalid_argument", [&] { rkf.rkf45(decay_rhs, 0.0, 1.0, 1.0, -0.1, 1e-6); });
    test::expect_throw<std::invalid_argument>("t_end == t0 throws invalid_argument", [&] { rkf.rkf45(decay_rhs, 1.0, 1.0, 1.0, 0.1, 1e-6); });
    test::expect_throw<std::invalid_argument>("t_end < t0 throws invalid_argument", [&] { rkf.rkf45(decay_rhs, 1.0, 1.0, 0.0, 0.1, 1e-6); });
    test::expect_throw<std::invalid_argument>("tol = 0 throws invalid_argument", [&] { rkf.rkf45(decay_rhs, 0.0, 1.0, 1.0, 0.1, 0.0); });
    test::expect_throw<std::invalid_argument>("tol = NaN throws invalid_argument", [&] { rkf.rkf45(decay_rhs, 0.0, 1.0, 1.0, 0.1, nan); });
    test::expect_throw<std::invalid_argument>("rkf45_value: t_end = NaN throws invalid_argument", [&] { rkf.rkf45_value(decay_rhs, 0.0, 1.0, nan, 1e-6); });
    // dy/dt = y^2, y(0) = 1 has the solution 1 / (1 - t), infinite at t = 1: the steps shrink without end.
    test::expect_throw<std::runtime_error>("Solution that blows up at t = 1 throws runtime_error",
        [&] { rkf.rkf45([](double, double y) { return y * y; }, 0.0, 1.0, 2.0, 0.1, 1e-6); });
}

/**
 * @brief Runs every test group.
 * @return 0 if all checks passed, 1 otherwise.
 */
int main()
{
    std::cout << "[*] Running Runge-Kutta-Fehlberg Solver Tests..." << std::endl;
    accuracy();
    boundaries();
    errors();
    return test::finish("Runge-Kutta-Fehlberg Solver");
}
