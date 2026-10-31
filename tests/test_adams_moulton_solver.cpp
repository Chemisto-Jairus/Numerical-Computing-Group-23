/**
 * @file test_adams_moulton_solver.cpp
 * @brief Unit tests for numcomp::AdamsMoultonSolver (AM2 and AM4).
 * @author Muwanguzi Samuel (Member 7)
 *
 * Covers normal use, edge cases and invalid input. Each check prints [PASS] or [FAIL];
 * the program returns 0 only if every check passed (see test_helpers.hpp).
 */

#include "numerical-computing/adams_moulton_solver.hpp"
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
/// @brief Test function: returns 2.0 * t (exact solution y = t^2).
/// @param t  Time.
/// @return The function value.
double linear_rhs(double t, double) { return 2.0 * t; }
/// @brief Test function: returns y - t (exact solution y = e^t + t + 1 for y(0) = 2).
/// @param t  Time.
/// @param y  Current value.
/// @return The function value.
double mixed_rhs(double t, double y) { return y - t; }

/// Accuracy, the observed order, and the advantage over Adams-Bashforth's error constant.
void accuracy()
{
    numcomp::AdamsMoultonSolver am;
    double exact = std::exp(-2.0);

    check("AM4: decay dy/dt = -2y, h = 0.05, close to e^-2", close(am.am4(decay_rhs, 0.0, 1.0, 1.0, 0.05).back().second, exact, 1e-6));
    check("AM2: decay dy/dt = -2y, h = 0.05, close to e^-2", close(am.am2(decay_rhs, 0.0, 1.0, 1.0, 0.05).back().second, exact, 5e-4));
    // The trapezoidal rule integrates a linear slope exactly.
    check("AM2: dy/dt = 2t gives y(3) = 9 exactly", close(am.am2(linear_rhs, 0.0, 0.0, 3.0, 0.5).back().second, 9.0, 1e-12));
    check("AM4: dy/dt = y - t, y(0) = 2, gives e + 2 at t = 1", close(am.am4(mixed_rhs, 0.0, 2.0, 1.0, 0.01).back().second, std::exp(1.0) + 2.0, 1e-9));

    double r2 = std::abs(am.am2(decay_rhs, 0.0, 1.0, 1.0, 0.05).back().second - exact)
              / std::abs(am.am2(decay_rhs, 0.0, 1.0, 1.0, 0.025).back().second - exact);
    double r4 = std::abs(am.am4(decay_rhs, 0.0, 1.0, 1.0, 0.05).back().second - exact)
              / std::abs(am.am4(decay_rhs, 0.0, 1.0, 1.0, 0.025).back().second - exact);
    check("AM2: halving h divides the error by about 4", r2 > 3.5 && r2 < 4.5);
    check("AM4: halving h divides the error by about 16", r4 > 13.0 && r4 < 19.0);
}

/// Shape of the returned curve and the shortened final step.
void boundaries()
{
    numcomp::AdamsMoultonSolver am;
    std::vector<std::pair<double, double>> curve = am.am4(decay_rhs, 0.0, 1.0, 1.0, 0.1);
    check("AM4: first point is (t0, y0)", curve.front().first == 0.0 && curve.front().second == 1.0);
    check("AM4: h = 0.1 on [0, 1] gives 11 points", curve.size() == 11);
    check("AM4: last t is exactly t_end", curve.back().first == 1.0);

    std::vector<std::pair<double, double>> uneven = am.am2(decay_rhs, 0.0, 1.0, 1.0, 0.3);
    check("AM2: h = 0.3 gives 5 points ending exactly at 1", uneven.size() == 5 && uneven.back().first == 1.0);
    std::vector<std::pair<double, double>> one_step = am.am2(decay_rhs, 0.0, 1.0, 1.0, 5.0);
    check("AM2: h bigger than the interval gives one step to t_end", one_step.size() == 2 && one_step.back().first == 1.0);
}

/// Invalid input throws invalid_argument; a step too large for the corrector throws runtime_error.
void errors()
{
    numcomp::AdamsMoultonSolver am;
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
        test::expect_throw<std::invalid_argument>(std::string("AM2: ") + b.label + " throws invalid_argument", [&] { am.am2(decay_rhs, b.t0, 1.0, b.t_end, b.h); });
        test::expect_throw<std::invalid_argument>(std::string("AM4: ") + b.label + " throws invalid_argument", [&] { am.am4(decay_rhs, b.t0, 1.0, b.t_end, b.h); });
    }

    // dy/dt = -50 y with h = 0.1: h * (1/2) * 50 = 2.5 > 1, so the corrector iteration diverges.
    auto stiff = [](double, double y) { return -50.0 * y; };
    test::expect_throw<std::runtime_error>("AM2: corrector cannot converge for a stiff problem with large h", [&] { am.am2(stiff, 0.0, 1.0, 1.0, 0.1); });
    test::expect_no_throw("AM2: the same stiff problem works with a small h", [&] { am.am2(stiff, 0.0, 1.0, 1.0, 0.01); });
}

/**
 * @brief Runs every test group.
 * @return 0 if all checks passed, 1 otherwise.
 */
int main()
{
    std::cout << "[*] Running Adams-Moulton Solver Tests..." << std::endl;
    accuracy();
    boundaries();
    errors();
    return test::finish("Adams-Moulton Solver");
}
