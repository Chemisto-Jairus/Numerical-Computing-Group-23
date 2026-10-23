// Tests for numcomp::CubicSpline (natural cubic spline)
// Owner: Batte Derrick Calvin (Member 2)
// Uses the shared helpers in test_helpers.hpp.

#include "numerical-computing/cubic_spline.hpp"
#include "test_helpers.hpp"
#include <cmath>
#include <stdexcept>
#include <vector>

using test::check;
using test::close;

void normal_cases()
{
    numcomp::CubicSpline spline;

    std::vector<double> x = {0.0, 1.0, 2.0, 3.0, 4.0};
    std::vector<double> y = {1.0, 3.0, 2.0, 5.0, 4.0};
    bool through_points = true;
    for (std::size_t i = 0; i < x.size(); ++i)
    {
        if (!close(spline.interpolate(x[i], x, y), y[i], 1e-12)) through_points = false;
    }
    check("Spline goes exactly through every data point", through_points);

    // Points (0,0), (1,1), (2,0). By hand: 4*M1 = -12, so M1 = -3.
    // S(0.5) = -3*(0.5^3)/6 + (1 - (-3)/6)*0.5 = -0.0625 + 0.75 = 0.6875
    std::vector<double> hx = {0.0, 1.0, 2.0};
    std::vector<double> hy = {0.0, 1.0, 0.0};
    check("Hand-worked value S(0.5) = 0.6875", close(spline.interpolate(0.5, hx, hy), 0.6875, 1e-12));
    check("Symmetry: S(1.5) = S(0.5)", close(spline.interpolate(1.5, hx, hy), 0.6875, 1e-12));

    std::vector<double> M = spline.second_derivatives(hx, hy);
    check("second_derivatives gives {0, -3, 0}", M.size() == 3 && close(M[0], 0.0, 1e-12) && close(M[1], -3.0, 1e-12) && close(M[2], 0.0, 1e-12));

    std::vector<double> M2 = spline.second_derivatives({0.0, 0.5, 1.5, 2.0, 3.5}, {2.0, -1.0, 4.0, 0.0, 1.0});
    check("Natural spline: M is zero at both ends", M2.front() == 0.0 && M2.back() == 0.0);

    check("Straight line data: S(2.5) = 6", close(spline.interpolate(2.5, {0.0, 1.0, 2.0, 3.0}, {1.0, 3.0, 5.0, 7.0}), 6.0, 1e-12));

    // 11 points of sin(x) on [0, pi]: the spline should be very close to sin
    const double pi = std::acos(-1.0);
    std::vector<double> sx, sy;
    for (int i = 0; i <= 10; ++i)
    {
        sx.push_back(i * pi / 10.0);
        sy.push_back(std::sin(i * pi / 10.0));
    }
    bool near_sin = true;
    for (int j = 0; j < 20; ++j)
    {
        double xt = (j + 0.5) * pi / 20.0;
        if (!close(spline.interpolate(xt, sx, sy), std::sin(xt), 1e-3)) near_sin = false;
    }
    check("Spline of sin(x) with 11 points is within 1e-3", near_sin);
}

void edge_cases()
{
    numcomp::CubicSpline spline;
    check("Only 2 points: spline is a straight line", close(spline.interpolate(2.0, {1.0, 3.0}, {2.0, 6.0}), 4.0, 1e-12));

    std::vector<double> x = {0.0, 1.0, 2.0, 3.0};
    std::vector<double> y = {5.0, 1.0, 4.0, 2.0};
    check("x equal to first data point", close(spline.interpolate(0.0, x, y), 5.0, 1e-12));
    check("x equal to last data point", close(spline.interpolate(3.0, x, y), 2.0, 1e-12));

    // y = 2x + 1 again, with uneven spacing and negative x
    check("Uneven spacing and negative x: line is exact", close(spline.interpolate(-2.0, {-4.0, -3.5, -1.0, 0.0, 2.5}, {-7.0, -6.0, -1.0, 1.0, 6.0}), -3.0, 1e-12));
}

void invalid_input()
{
    numcomp::CubicSpline spline;
    test::expect_throw<std::invalid_argument>("Different sizes throw invalid_argument", [&] { spline.interpolate(0.5, {0.0, 1.0, 2.0}, {0.0, 1.0}); });
    test::expect_throw<std::invalid_argument>("One point throws invalid_argument", [&] { spline.interpolate(0.0, {0.0}, {1.0}); });
    test::expect_throw<std::invalid_argument>("Empty vectors throw invalid_argument", [&] { spline.second_derivatives({}, {}); });
    test::expect_throw<std::invalid_argument>("Unsorted x throws invalid_argument", [&] { spline.interpolate(1.5, {0.0, 2.0, 1.0, 3.0}, {0.0, 4.0, 1.0, 9.0}); });
    test::expect_throw<std::invalid_argument>("Repeated x throws invalid_argument", [&] { spline.second_derivatives({0.0, 1.0, 1.0, 2.0}, {0.0, 1.0, 2.0, 3.0}); });
    test::expect_throw<std::out_of_range>("x below the data throws out_of_range", [&] { spline.interpolate(-0.1, {0.0, 1.0, 2.0}, {0.0, 1.0, 0.0}); });
    test::expect_throw<std::out_of_range>("x above the data throws out_of_range", [&] { spline.interpolate(2.1, {0.0, 1.0, 2.0}, {0.0, 1.0, 0.0}); });
}

int main()
{
    std::cout << "[*] Running Cubic Spline Tests..." << std::endl;
    normal_cases();
    edge_cases();
    invalid_input();
    return test::finish("Cubic Spline");
}
