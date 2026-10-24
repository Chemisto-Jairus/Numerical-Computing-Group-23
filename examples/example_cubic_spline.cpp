/**
 * @file example_cubic_spline.cpp
 * @brief Demo 1 of 2 for numcomp::CubicSpline: natural cubic spline basics.
 * @author Batte Derrick Calvin (Member 2)
 */

#include "numerical-computing/cubic_spline.hpp"
#include "numerical-computing/interpolator.hpp"
#include "example_helpers.hpp"
#include <cmath>
#include <vector>

/**
 * @brief Runs the demo sections.
 * @return 0 if every section completed, 1 otherwise.
 */
int main()
{
    numcomp::CubicSpline spline;
    numcomp::Interpolator interp;
    demo::title("Cubic Spline", "Natural cubic spline through data points");

    demo::section("Hand-checkable case: points (0,0), (1,1), (2,0)", [&] {
        std::vector<double> x = {0.0, 1.0, 2.0};
        std::vector<double> y = {0.0, 1.0, 0.0};
        std::vector<double> M = spline.second_derivatives(x, y);
        demo::value("M0 (natural end)", M[0], 6);
        demo::value("M1 = -3 by hand", M[1], 6);
        demo::value("M2 (natural end)", M[2], 6);
        demo::compare("S(0.5)", spline.interpolate(0.5, x, y), 0.6875);
    });

    demo::section("Spline vs linear interpolation for y = sin(x), 6 points on [0, pi]", [&] {
        const double pi = std::acos(-1.0);
        std::vector<double> x, y;
        for (int i = 0; i <= 5; ++i) { x.push_back(i * pi / 5.0); y.push_back(std::sin(i * pi / 5.0)); }
        demo::table_header({"x", "spline", "linear", "sin(x)"});
        for (double t : {0.3, 0.9, 1.5, 2.2, 2.9})
        {
            demo::table_row({t, spline.interpolate(t, x, y), interp.linear_interpolate(t, x, y), std::sin(t)});
        }
    });

    demo::section("A straight line is reproduced exactly", [&] {
        std::vector<double> x = {0.0, 0.7, 2.0, 3.1};
        std::vector<double> y = {1.0, 2.4, 5.0, 7.2};   // y = 2x + 1
        demo::compare("S(1.6) for y = 2x + 1", spline.interpolate(1.6, x, y), 4.2);
    });

    demo::section("Invalid input is rejected", [&] {
        demo::show_rejected("Only one data point", [&] { spline.interpolate(0.0, {0.0}, {1.0}); });
        demo::show_rejected("Repeated x value", [&] { spline.second_derivatives({0.0, 1.0, 1.0}, {0.0, 1.0, 2.0}); });
        demo::show_rejected("x outside the data", [&] { spline.interpolate(-1.0, {0.0, 1.0, 2.0}, {0.0, 1.0, 0.0}); });
    });

    return demo::finish();
}
