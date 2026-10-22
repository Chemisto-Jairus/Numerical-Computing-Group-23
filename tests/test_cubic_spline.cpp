// Tests for numcomp::CubicSpline (natural cubic spline)
// Owner: Batte Derrick Calvin (Member 2)
//
// No test framework and no assert(), so the tests also work in Release builds.
// Each check prints PASS or FAIL. main() returns 1 if anything failed,
// so ctest and run_all_tests.sh can see the failure.

#include "numerical-computing/cubic_spline.hpp"
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

// ---------------- NORMAL CASES ----------------

void test_passes_through_data_points()
{
    numcomp::CubicSpline spline;
    std::vector<double> x = {0.0, 1.0, 2.0, 3.0, 4.0};
    std::vector<double> y = {1.0, 3.0, 2.0, 5.0, 4.0};

    bool ok = true;
    for (size_t i = 0; i < x.size(); ++i)
    {
        if (!close(spline.interpolate(x[i], x, y), y[i], 1e-12))
        {
            ok = false;
        }
    }
    report("Spline goes exactly through every data point", ok);
}

void test_hand_worked_example()
{
    // Points (0,0), (1,1), (2,0).
    // By hand: 4*M1 = 6*((0-1) - (1-0)) = -12, so M1 = -3.
    // S(0.5) = -3*(0.5^3)/6 + (1 - (-3)/6)*0.5 = -0.0625 + 0.75 = 0.6875
    numcomp::CubicSpline spline;
    std::vector<double> x = {0.0, 1.0, 2.0};
    std::vector<double> y = {0.0, 1.0, 0.0};

    report("Hand-worked value S(0.5) = 0.6875", close(spline.interpolate(0.5, x, y), 0.6875, 1e-12));
    report("Symmetry: S(1.5) = S(0.5)", close(spline.interpolate(1.5, x, y), 0.6875, 1e-12));
}

void test_second_derivatives_values()
{
    numcomp::CubicSpline spline;
    std::vector<double> x = {0.0, 1.0, 2.0};
    std::vector<double> y = {0.0, 1.0, 0.0};
    std::vector<double> M = spline.second_derivatives(x, y);

    bool ok = M.size() == 3 && close(M[0], 0.0, 1e-12) && close(M[1], -3.0, 1e-12) && close(M[2], 0.0, 1e-12);
    report("second_derivatives gives {0, -3, 0}", ok);
}

void test_natural_end_conditions()
{
    numcomp::CubicSpline spline;
    std::vector<double> x = {0.0, 0.5, 1.5, 2.0, 3.5};
    std::vector<double> y = {2.0, -1.0, 4.0, 0.0, 1.0};
    std::vector<double> M = spline.second_derivatives(x, y);

    report("Natural spline: M is zero at both ends", M[0] == 0.0 && M[M.size() - 1] == 0.0);
}

void test_straight_line_is_exact()
{
    // For a straight line y = 2x + 1 the spline must give the line back
    numcomp::CubicSpline spline;
    std::vector<double> x = {0.0, 1.0, 2.0, 3.0};
    std::vector<double> y = {1.0, 3.0, 5.0, 7.0};

    report("Straight line data: S(2.5) = 6", close(spline.interpolate(2.5, x, y), 6.0, 1e-12));
}

void test_approximates_sine()
{
    // 11 points of sin(x) on [0, pi]. The spline should be very close to sin.
    numcomp::CubicSpline spline;
    const double pi = std::acos(-1.0);
    std::vector<double> x, y;
    for (int i = 0; i <= 10; ++i)
    {
        double xi = i * pi / 10.0;
        x.push_back(xi);
        y.push_back(std::sin(xi));
    }

    bool ok = true;
    for (int j = 0; j < 20; ++j)
    {
        double xt = (j + 0.5) * pi / 20.0;     // points between the nodes
        if (!close(spline.interpolate(xt, x, y), std::sin(xt), 1e-3))
        {
            ok = false;
        }
    }
    report("Spline of sin(x) with 11 points is within 1e-3", ok);
}

// ---------------- EDGE CASES ----------------

void test_two_points_gives_line()
{
    numcomp::CubicSpline spline;
    std::vector<double> x = {1.0, 3.0};
    std::vector<double> y = {2.0, 6.0};

    report("Only 2 points: spline is a straight line", close(spline.interpolate(2.0, x, y), 4.0, 1e-12));
}

void test_end_points()
{
    numcomp::CubicSpline spline;
    std::vector<double> x = {0.0, 1.0, 2.0, 3.0};
    std::vector<double> y = {5.0, 1.0, 4.0, 2.0};

    report("x equal to first data point", close(spline.interpolate(0.0, x, y), 5.0, 1e-12));
    report("x equal to last data point", close(spline.interpolate(3.0, x, y), 2.0, 1e-12));
}

void test_uneven_spacing_and_negative_x()
{
    numcomp::CubicSpline spline;
    std::vector<double> x = {-4.0, -3.5, -1.0, 0.0, 2.5};
    std::vector<double> y = {-7.0, -6.0, -1.0, 1.0, 6.0};    // y = 2x + 1 again

    report("Uneven spacing and negative x: line is exact", close(spline.interpolate(-2.0, x, y), -3.0, 1e-12));
}

// ---------------- INVALID INPUT ----------------

void test_size_mismatch_throws()
{
    numcomp::CubicSpline spline;
    bool ok = false;
    try
    {
        spline.interpolate(0.5, {0.0, 1.0, 2.0}, {0.0, 1.0});
    }
    catch (const std::invalid_argument&)
    {
        ok = true;
    }
    catch (...)
    {
    }
    report("Different sizes throw invalid_argument", ok);
}

void test_one_point_throws()
{
    numcomp::CubicSpline spline;
    bool ok = false;
    try
    {
        spline.interpolate(0.0, {0.0}, {1.0});
    }
    catch (const std::invalid_argument&)
    {
        ok = true;
    }
    catch (...)
    {
    }
    report("One point throws invalid_argument", ok);
}

void test_empty_throws()
{
    numcomp::CubicSpline spline;
    bool ok = false;
    try
    {
        spline.second_derivatives({}, {});
    }
    catch (const std::invalid_argument&)
    {
        ok = true;
    }
    catch (...)
    {
    }
    report("Empty vectors throw invalid_argument", ok);
}

void test_unsorted_x_throws()
{
    numcomp::CubicSpline spline;
    bool ok = false;
    try
    {
        spline.interpolate(1.5, {0.0, 2.0, 1.0, 3.0}, {0.0, 4.0, 1.0, 9.0});
    }
    catch (const std::invalid_argument&)
    {
        ok = true;
    }
    catch (...)
    {
    }
    report("Unsorted x throws invalid_argument", ok);
}

void test_repeated_x_throws()
{
    numcomp::CubicSpline spline;
    bool ok = false;
    try
    {
        spline.second_derivatives({0.0, 1.0, 1.0, 2.0}, {0.0, 1.0, 2.0, 3.0});
    }
    catch (const std::invalid_argument&)
    {
        ok = true;
    }
    catch (...)
    {
    }
    report("Repeated x throws invalid_argument", ok);
}

void test_x_below_range_throws()
{
    numcomp::CubicSpline spline;
    bool ok = false;
    try
    {
        spline.interpolate(-0.1, {0.0, 1.0, 2.0}, {0.0, 1.0, 0.0});
    }
    catch (const std::out_of_range&)
    {
        ok = true;
    }
    catch (...)
    {
    }
    report("x below the data throws out_of_range", ok);
}

void test_x_above_range_throws()
{
    numcomp::CubicSpline spline;
    bool ok = false;
    try
    {
        spline.interpolate(2.1, {0.0, 1.0, 2.0}, {0.0, 1.0, 0.0});
    }
    catch (const std::out_of_range&)
    {
        ok = true;
    }
    catch (...)
    {
    }
    report("x above the data throws out_of_range", ok);
}

int main()
{
    std::cout << "[*] Running Cubic Spline Tests..." << std::endl;

    // Normal cases
    test_passes_through_data_points();
    test_hand_worked_example();
    test_second_derivatives_values();
    test_natural_end_conditions();
    test_straight_line_is_exact();
    test_approximates_sine();

    // Edge cases
    test_two_points_gives_line();
    test_end_points();
    test_uneven_spacing_and_negative_x();

    // Invalid input
    test_size_mismatch_throws();
    test_one_point_throws();
    test_empty_throws();
    test_unsorted_x_throws();
    test_repeated_x_throws();
    test_x_below_range_throws();
    test_x_above_range_throws();

    std::cout << std::endl << "Cubic Spline: " << passed << " passed, " << failed << " failed" << std::endl;

    // Non-zero exit code tells ctest / run_all_tests.sh that something failed
    if (failed > 0)
    {
        return 1;
    }
    return 0;
}
