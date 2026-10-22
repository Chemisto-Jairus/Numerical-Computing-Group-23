// Tests for numcomp::AdvancedIntegrator (Simpson's rule + Romberg integration)
// Owner: Wasswa George Mpanga (Member 5)
//
// Keeps the Week 3 tests (x^3 on [0, 2]) and adds exact, edge and invalid cases.
// No assert(), so the tests also work in Release builds.
// main() returns 1 if anything failed, so ctest can see the failure.

#include "numerical-computing/advanced_integrator.hpp"
#include <iostream>
#include <cmath>
#include <limits>
#include <stdexcept>
#include <string>

int passed = 0;
int failed = 0;

// Prints PASS/FAIL for one test and updates the counters
void report(const std::string& name, bool ok)
{
    if (ok)
    {
        std::cout << "[PASS] " << name << std::endl;
        passed++;
    }
    else
    {
        std::cout << "[FAIL] " << name << std::endl;
        failed++;
    }
}

// True if a and b are equal within tol
bool close(double a, double b, double tol)
{
    return std::abs(a - b) < tol;
}

// Test functions
double cube(double x) { return x * x * x; }    // integral 0..2 = 4
double quartic(double x) { return x * x * x * x; }    // integral 0..1 = 1/5

// ---------------- NORMAL CASES (from Week 3) ----------------

void test_simpsons_rule()
{
    numcomp::AdvancedIntegrator integrator;
    report("Simpson: x^3 on [0, 2] with n = 100 gives 4", close(integrator.simpsons_rule(cube, 0.0, 2.0, 100), 4.0, 1e-6));
}

void test_romberg_integration()
{
    numcomp::AdvancedIntegrator integrator;
    report("Romberg: x^3 on [0, 2] with 5 rows gives 4", close(integrator.romberg_integration(cube, 0.0, 2.0, 5), 4.0, 1e-6));
}

void test_sine()
{
    numcomp::AdvancedIntegrator integrator;
    const double pi = std::acos(-1.0);
    auto s = [](double x) { return std::sin(x); };
    report("Simpson: sin on [0, pi] with n = 100 is close to 2", close(integrator.simpsons_rule(s, 0.0, pi, 100), 2.0, 1e-7));
    report("Romberg: sin on [0, pi] with 8 rows is close to 2", close(integrator.romberg_integration(s, 0.0, pi, 8), 2.0, 1e-10));
}

// ---------------- EXACT RESULTS ----------------

void test_cubic_is_exact()
{
    // Simpson is exact for cubics, even with only 2 strips
    numcomp::AdvancedIntegrator integrator;
    report("Simpson: x^3 on [0, 2] with n = 2 is exactly 4", close(integrator.simpsons_rule(cube, 0.0, 2.0, 2), 4.0, 1e-12));
    report("Romberg: x^3 on [0, 2] with 2 rows is exactly 4", close(integrator.romberg_integration(cube, 0.0, 2.0, 2), 4.0, 1e-12));
}

void test_quartic_romberg_exact()
{
    // Romberg with 3 rows is exact up to degree 5
    numcomp::AdvancedIntegrator integrator;
    report("Romberg: x^4 on [0, 1] with 3 rows is exactly 1/5", close(integrator.romberg_integration(quartic, 0.0, 1.0, 3), 0.2, 1e-12));
}

// ---------------- EDGE CASES ----------------

void test_reversed_limits()
{
    numcomp::AdvancedIntegrator integrator;
    report("Simpson: reversed limits [2, 0] give -4", close(integrator.simpsons_rule(cube, 2.0, 0.0, 10), -4.0, 1e-12));
    report("Romberg: reversed limits [2, 0] give -4", close(integrator.romberg_integration(cube, 2.0, 0.0, 4), -4.0, 1e-12));
}

void test_romberg_one_row()
{
    // With 1 row Romberg is just the trapezoidal rule with 1 strip: (f(0) + f(2)) / 2 * 2 = 8
    numcomp::AdvancedIntegrator integrator;
    report("Romberg: 1 row equals one trapezoid (8)", close(integrator.romberg_integration(cube, 0.0, 2.0, 1), 8.0, 1e-12));
}

void test_romberg_max_rows_allowed()
{
    numcomp::AdvancedIntegrator integrator;
    bool ok = true;
    try
    {
        integrator.romberg_integration(cube, 0.0, 2.0, 20);
    }
    catch (...)
    {
        ok = false;
    }
    report("Romberg: 20 rows runs without error", ok);
}

// ---------------- INVALID INPUT ----------------

void test_simpson_odd_n_throws()
{
    numcomp::AdvancedIntegrator integrator;
    bool ok = false;
    try
    {
        integrator.simpsons_rule(cube, 0.0, 2.0, 3);
    }
    catch (const std::invalid_argument&)
    {
        ok = true;
    }
    catch (...)
    {
    }
    report("Simpson: odd n = 3 throws invalid_argument", ok);
}

void test_simpson_zero_n_throws()
{
    numcomp::AdvancedIntegrator integrator;
    bool ok = false;
    try
    {
        integrator.simpsons_rule(cube, 0.0, 2.0, 0);
    }
    catch (const std::invalid_argument&)
    {
        ok = true;
    }
    catch (...)
    {
    }
    report("Simpson: n = 0 throws invalid_argument", ok);
}

void test_romberg_too_many_rows_throws()
{
    numcomp::AdvancedIntegrator integrator;
    bool ok = false;
    try
    {
        integrator.romberg_integration(cube, 0.0, 2.0, 26);
    }
    catch (const std::invalid_argument&)
    {
        ok = true;
    }
    catch (...)
    {
    }
    report("Romberg: max_iter = 26 (above 25) throws invalid_argument", ok);
}

void test_romberg_zero_rows_throws()
{
    numcomp::AdvancedIntegrator integrator;
    bool ok = false;
    try
    {
        integrator.romberg_integration(cube, 0.0, 2.0, 0);
    }
    catch (const std::invalid_argument&)
    {
        ok = true;
    }
    catch (...)
    {
    }
    report("Romberg: max_iter = 0 throws invalid_argument", ok);
}

void test_nan_limits_throw()
{
    numcomp::AdvancedIntegrator integrator;
    bool simpson_ok = false;
    bool romberg_ok = false;
    try
    {
        integrator.simpsons_rule(cube, std::numeric_limits<double>::quiet_NaN(), 2.0, 10);
    }
    catch (const std::invalid_argument&)
    {
        simpson_ok = true;
    }
    catch (...)
    {
    }
    try
    {
        integrator.romberg_integration(cube, 0.0, std::numeric_limits<double>::infinity(), 5);
    }
    catch (const std::invalid_argument&)
    {
        romberg_ok = true;
    }
    catch (...)
    {
    }
    report("Simpson: a = NaN throws invalid_argument", simpson_ok);
    report("Romberg: b = infinity throws invalid_argument", romberg_ok);
}

int main()
{
    std::cout << "[*] Running Advanced Integration Tests..." << std::endl;

    // Normal cases
    test_simpsons_rule();
    test_romberg_integration();
    test_sine();

    // Exact results
    test_cubic_is_exact();
    test_quartic_romberg_exact();

    // Edge cases
    test_reversed_limits();
    test_romberg_one_row();
    test_romberg_max_rows_allowed();

    // Invalid input
    test_simpson_odd_n_throws();
    test_simpson_zero_n_throws();
    test_romberg_too_many_rows_throws();
    test_romberg_zero_rows_throws();
    test_nan_limits_throw();

    std::cout << std::endl << "Advanced Integration: " << passed << " passed, " << failed << " failed" << std::endl;

    if (failed > 0)
    {
        return 1;
    }
    return 0;
}
