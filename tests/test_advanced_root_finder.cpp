// Tests for numcomp::AdvancedRootFinder (Secant + Fixed-Point Iteration)
// Owner: Bakalu Valeria (Member 4)
//
// Keeps the Week 3 tests and adds edge and invalid cases.
// No assert(), so the tests also work in Release builds.
// main() returns 1 if anything failed, so ctest can see the failure.

#include "numerical-computing/advanced_root_finder.hpp"
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
double f_square(double x) { return x * x - 4.0; }          // root at 2
double g_sqrt(double x) { return std::sqrt(x + 2.0); }     // fixed point at 2 (x^2 - x - 2 = 0)
double g_cos(double x) { return std::cos(x); }             // fixed point near 0.7390851332

// ---------------- NORMAL CASES (from Week 3) ----------------

void test_secant_method()
{
    numcomp::AdvancedRootFinder finder;
    report("Secant: x^2 - 4 from 1 and 3 gives 2", close(finder.secant_method(f_square, 1.0, 3.0, 1e-6, 100), 2.0, 1e-5));
}

void test_fixed_point_iteration()
{
    numcomp::AdvancedRootFinder finder;
    report("Fixed-point: g(x) = sqrt(x + 2) from 1 gives 2", close(finder.fixed_point_iteration(g_sqrt, 1.0, 1e-6, 100), 2.0, 1e-5));
}

void test_more_functions()
{
    numcomp::AdvancedRootFinder finder;
    report("Secant: cos(x) - x from 0 and 1", close(finder.secant_method([](double x) { return std::cos(x) - x; }, 0.0, 1.0, 1e-12, 100), 0.7390851332, 1e-9));
    report("Fixed-point: g(x) = cos(x) from 1", close(finder.fixed_point_iteration(g_cos, 1.0, 1e-12, 1000), 0.7390851332, 1e-9));
}

// ---------------- EDGE CASES ----------------

void test_secant_guesses_reversed()
{
    numcomp::AdvancedRootFinder finder;
    report("Secant: guesses in either order (3 then 1)", close(finder.secant_method(f_square, 3.0, 1.0, 1e-10, 100), 2.0, 1e-8));
}

void test_fixed_point_start_on_answer()
{
    numcomp::AdvancedRootFinder finder;
    report("Fixed-point: starting on the answer returns it", close(finder.fixed_point_iteration(g_sqrt, 2.0, 1e-10, 10), 2.0, 1e-12));
}

// ---------------- INVALID INPUT ----------------

void test_no_convergence_throws()
{
    // max_iter = 2 is far too few steps for tol = 1e-12
    numcomp::AdvancedRootFinder finder;
    bool secant_ok = false;
    bool fixed_ok = false;
    try
    {
        finder.secant_method(f_square, 1.0, 3.0, 1e-12, 2);
    }
    catch (const std::runtime_error&)
    {
        secant_ok = true;
    }
    catch (...)
    {
    }
    try
    {
        finder.fixed_point_iteration(g_sqrt, 1.0, 1e-12, 2);
    }
    catch (const std::runtime_error&)
    {
        fixed_ok = true;
    }
    catch (...)
    {
    }
    report("Secant: max_iter = 2 throws runtime_error", secant_ok);
    report("Fixed-point: max_iter = 2 throws runtime_error", fixed_ok);
}

void test_zero_tolerance_throws()
{
    numcomp::AdvancedRootFinder finder;
    bool secant_ok = false;
    bool fixed_ok = false;
    try
    {
        finder.secant_method(f_square, 1.0, 3.0, 0.0, 100);
    }
    catch (const std::invalid_argument&)
    {
        secant_ok = true;
    }
    catch (...)
    {
    }
    try
    {
        finder.fixed_point_iteration(g_sqrt, 1.0, 0.0, 100);
    }
    catch (const std::invalid_argument&)
    {
        fixed_ok = true;
    }
    catch (...)
    {
    }
    report("Secant: tol = 0 throws invalid_argument", secant_ok);
    report("Fixed-point: tol = 0 throws invalid_argument", fixed_ok);
}

void test_zero_max_iter_throws()
{
    numcomp::AdvancedRootFinder finder;
    bool secant_ok = false;
    bool fixed_ok = false;
    try
    {
        finder.secant_method(f_square, 1.0, 3.0, 1e-6, 0);
    }
    catch (const std::invalid_argument&)
    {
        secant_ok = true;
    }
    catch (...)
    {
    }
    try
    {
        finder.fixed_point_iteration(g_sqrt, 1.0, 1e-6, 0);
    }
    catch (const std::invalid_argument&)
    {
        fixed_ok = true;
    }
    catch (...)
    {
    }
    report("Secant: max_iter = 0 throws invalid_argument", secant_ok);
    report("Fixed-point: max_iter = 0 throws invalid_argument", fixed_ok);
}

void test_nan_tolerance_throws()
{
    numcomp::AdvancedRootFinder finder;
    bool ok = false;
    try
    {
        finder.secant_method(f_square, 1.0, 3.0, std::numeric_limits<double>::quiet_NaN(), 100);
    }
    catch (const std::invalid_argument&)
    {
        ok = true;
    }
    catch (...)
    {
    }
    report("Secant: tol = NaN throws invalid_argument", ok);
}

void test_secant_equal_guesses_throws()
{
    // x0 == x1 means f(x1) - f(x0) = 0, so the method would divide by zero
    numcomp::AdvancedRootFinder finder;
    bool ok = false;
    try
    {
        finder.secant_method(f_square, 1.0, 1.0, 1e-6, 100);
    }
    catch (const std::runtime_error&)
    {
        ok = true;
    }
    catch (...)
    {
    }
    report("Secant: equal guesses throw runtime_error", ok);
}

void test_fixed_point_diverges_throws()
{
    // g(x) = 2x + 1 runs away from its fixed point (-1), so it never converges
    numcomp::AdvancedRootFinder finder;
    bool ok = false;
    try
    {
        finder.fixed_point_iteration([](double x) { return 2.0 * x + 1.0; }, 1.0, 1e-8, 100);
    }
    catch (const std::runtime_error&)
    {
        ok = true;
    }
    catch (...)
    {
    }
    report("Fixed-point: diverging g throws runtime_error", ok);
}

int main()
{
    std::cout << "[*] Running Advanced Root Finder Tests..." << std::endl;

    // Normal cases
    test_secant_method();
    test_fixed_point_iteration();
    test_more_functions();

    // Edge cases
    test_secant_guesses_reversed();
    test_fixed_point_start_on_answer();

    // Invalid input
    test_no_convergence_throws();
    test_zero_tolerance_throws();
    test_zero_max_iter_throws();
    test_nan_tolerance_throws();
    test_secant_equal_guesses_throws();
    test_fixed_point_diverges_throws();

    std::cout << std::endl << "Advanced Root Finder: " << passed << " passed, " << failed << " failed" << std::endl;

    if (failed > 0)
    {
        return 1;
    }
    return 0;
}
