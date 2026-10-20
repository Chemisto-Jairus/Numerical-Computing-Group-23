// Tests for numcomp::Interpolator (linear interpolation + Chebyshev nodes)
// Owner: Batte Derrick Calvin (Member 2)
//
// Extends the Week 3 tests (linear at x = 1.5, Chebyshev n = 2) with edge and invalid cases.
// No test framework and no assert(), so the tests also work in Release builds.
// main() returns 1 if anything failed, so ctest can see the failure.

#include "numerical-computing/interpolator.hpp"
#include <iostream>
#include <vector>
#include <cmath>
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

// ---------------- LINEAR INTERPOLATION: NORMAL ----------------

void test_linear_midpoint()
{
    numcomp::Interpolator interp;
    std::vector<double> x = {0.0, 1.0, 2.0, 3.0};
    std::vector<double> y = {0.0, 2.0, 4.0, 6.0};      // y = 2x
    report("Linear: y = 2x at x = 1.5 gives 3", close(interp.linear_interpolate(1.5, x, y), 3.0, 1e-12));
}

void test_linear_uneven_spacing()
{
    numcomp::Interpolator interp;
    std::vector<double> x = {0.0, 1.0, 4.0};
    std::vector<double> y = {0.0, 1.0, 16.0};
    // Between (1,1) and (4,16): 1 + 15*(2-1)/3 = 6
    report("Linear: uneven spacing gives 6 at x = 2", close(interp.linear_interpolate(2.0, x, y), 6.0, 1e-12));
}

void test_linear_negative_values()
{
    numcomp::Interpolator interp;
    std::vector<double> x = {-2.0, -1.0, 0.0};
    std::vector<double> y = {4.0, -1.0, 0.0};
    report("Linear: negative x and y", close(interp.linear_interpolate(-1.5, x, y), 1.5, 1e-12));
}

// ---------------- LINEAR INTERPOLATION: EDGE ----------------

void test_linear_end_points()
{
    numcomp::Interpolator interp;
    std::vector<double> x = {0.0, 1.0, 2.0};
    std::vector<double> y = {5.0, 7.0, 3.0};
    report("Linear: x at first data point", close(interp.linear_interpolate(0.0, x, y), 5.0, 1e-12));
    report("Linear: x at last data point", close(interp.linear_interpolate(2.0, x, y), 3.0, 1e-12));
    report("Linear: x exactly on an inner point", close(interp.linear_interpolate(1.0, x, y), 7.0, 1e-12));
}

void test_linear_two_points()
{
    numcomp::Interpolator interp;
    report("Linear: works with only 2 points", close(interp.linear_interpolate(0.25, {0.0, 1.0}, {0.0, 4.0}), 1.0, 1e-12));
}

// ---------------- LINEAR INTERPOLATION: INVALID ----------------

void test_linear_size_mismatch()
{
    numcomp::Interpolator interp;
    bool ok = false;
    try
    {
        interp.linear_interpolate(0.5, {0.0, 1.0, 2.0}, {0.0, 1.0});
    }
    catch (const std::invalid_argument&)
    {
        ok = true;
    }
    catch (...)
    {
    }
    report("Linear: different sizes throw invalid_argument", ok);
}

void test_linear_one_point()
{
    numcomp::Interpolator interp;
    bool ok = false;
    try
    {
        interp.linear_interpolate(0.0, {0.0}, {1.0});
    }
    catch (const std::invalid_argument&)
    {
        ok = true;
    }
    catch (...)
    {
    }
    report("Linear: one point throws invalid_argument", ok);
}

void test_linear_unsorted()
{
    numcomp::Interpolator interp;
    bool ok = false;
    try
    {
        interp.linear_interpolate(1.5, {0.0, 2.0, 1.0}, {0.0, 4.0, 1.0});
    }
    catch (const std::invalid_argument&)
    {
        ok = true;
    }
    catch (...)
    {
    }
    report("Linear: unsorted x throws invalid_argument", ok);
}

void test_linear_repeated_x()
{
    numcomp::Interpolator interp;
    bool ok = false;
    try
    {
        interp.linear_interpolate(1.0, {0.0, 1.0, 1.0, 2.0}, {0.0, 1.0, 2.0, 3.0});
    }
    catch (const std::invalid_argument&)
    {
        ok = true;
    }
    catch (...)
    {
    }
    report("Linear: repeated x throws invalid_argument", ok);
}

void test_linear_out_of_range()
{
    numcomp::Interpolator interp;
    bool below = false;
    bool above = false;
    try
    {
        interp.linear_interpolate(-1.0, {0.0, 1.0}, {0.0, 1.0});
    }
    catch (const std::out_of_range&)
    {
        below = true;
    }
    catch (...)
    {
    }
    try
    {
        interp.linear_interpolate(1.5, {0.0, 1.0}, {0.0, 1.0});
    }
    catch (const std::out_of_range&)
    {
        above = true;
    }
    catch (...)
    {
    }
    report("Linear: x below range throws out_of_range", below);
    report("Linear: x above range throws out_of_range", above);
}

void test_linear_nan_in_data() 
{
    // Regression test for the Week 3 bug: NaN slipped past the order check
    numcomp::Interpolator interp;
    bool ok = false;
    try 
    {
        interp.linear_interpolate(0.5, {0.0, std::nan(""), 2.0}, {0.0, 1.0, 2.0});
    } 
    catch (const std::invalid_argument&) 
    {
        ok = true;
    } 
    catch (...) 
    {
    }
    report("Linear: NaN in x values throws invalid_argument", ok);
}

// ---------------- CHEBYSHEV NODES ----------------

void test_chebyshev_two_nodes()
{
    numcomp::Interpolator interp;
    std::vector<double> nodes = interp.chebyshev_nodes(-1.0, 1.0, 2);
    double r = std::sqrt(2.0) / 2.0;     // cos(pi/4)
    bool ok = nodes.size() == 2 && close(nodes[0], r, 1e-12) && close(nodes[1], -r, 1e-12);
    report("Chebyshev: n = 2 on [-1,1] gives +/- 0.7071", ok);
}

void test_chebyshev_inside_interval()
{
    numcomp::Interpolator interp;
    std::vector<double> nodes = interp.chebyshev_nodes(2.0, 5.0, 7);
    bool ok = nodes.size() == 7;
    for (size_t i = 0; i < nodes.size(); ++i)
    {
        if (nodes[i] <= 2.0 || nodes[i] >= 5.0)
        {
            ok = false;
        }
    }
    report("Chebyshev: all 7 nodes lie strictly inside [2, 5]", ok);
}

void test_chebyshev_one_node()
{
    numcomp::Interpolator interp;
    std::vector<double> nodes = interp.chebyshev_nodes(0.0, 4.0, 1);
    report("Chebyshev: n = 1 gives the midpoint", nodes.size() == 1 && close(nodes[0], 2.0, 1e-12));
}

void test_chebyshev_bad_n()
{
    numcomp::Interpolator interp;
    bool zero = false;
    bool negative = false;
    try
    {
        interp.chebyshev_nodes(0.0, 1.0, 0);
    }
    catch (const std::invalid_argument&)
    {
        zero = true;
    }
    catch (...)
    {
    }
    try
    {
        interp.chebyshev_nodes(0.0, 1.0, -3);
    }
    catch (const std::invalid_argument&)
    {
        negative = true;
    }
    catch (...)
    {
    }
    report("Chebyshev: n = 0 throws invalid_argument", zero);
    report("Chebyshev: n < 0 throws invalid_argument", negative);
}

void test_chebyshev_bad_interval()
{
    numcomp::Interpolator interp;
    bool equal = false;
    bool reversed = false;
    try
    {
        interp.chebyshev_nodes(1.0, 1.0, 3);
    }
    catch (const std::invalid_argument&)
    {
        equal = true;
    }
    catch (...)
    {
    }
    try
    {
        interp.chebyshev_nodes(2.0, 1.0, 3);
    }
    catch (const std::invalid_argument&)
    {
        reversed = true;
    }
    catch (...)
    {
    }
    report("Chebyshev: a == b throws invalid_argument", equal);
    report("Chebyshev: a > b throws invalid_argument", reversed);
}

void test_chebyshev_too_many_nodes() 
{
    // Regression test for the Week 3 bug: a huge n tried to allocate gigabytes
    numcomp::Interpolator interp;
    bool ok = false;
    try 
    {
        interp.chebyshev_nodes(0.0, 1.0, 1000000001);
    } 
    catch (const std::invalid_argument&) 
    {
        ok = true;
    } 
    catch (...) 
    {
    }
    report("Chebyshev: n > 1,000,000 throws invalid_argument", ok);
}

void test_chebyshev_nan_interval() 
{
    numcomp::Interpolator interp;
    bool ok = false;
    try 
    {
        interp.chebyshev_nodes(std::nan(""), 1.0, 3);
    } 
    catch (const std::invalid_argument&) 
    {
        ok = true;
    } 
    catch (...) 
    {
    }
    report("Chebyshev: NaN end point throws invalid_argument", ok);
}

int main()
{
    std::cout << "[*] Running Interpolator Tests..." << std::endl;

    test_linear_midpoint();
    test_linear_uneven_spacing();
    test_linear_negative_values();
    test_linear_end_points();
    test_linear_two_points();
    test_linear_size_mismatch();
    test_linear_one_point();
    test_linear_unsorted();
    test_linear_repeated_x();
    test_linear_out_of_range();

    test_linear_nan_in_data();

    test_chebyshev_two_nodes();
    test_chebyshev_inside_interval();
    test_chebyshev_one_node();
    test_chebyshev_bad_n();
    test_chebyshev_bad_interval();
    test_chebyshev_too_many_nodes();
    test_chebyshev_nan_interval();

    std::cout << std::endl << "Interpolator: " << passed << " passed, " << failed << " failed" << std::endl;

    if (failed > 0)
    {
        return 1;
    }
    return 0;
}
