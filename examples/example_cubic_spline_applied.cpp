/**
 * @file example_cubic_spline_applied.cpp
 * @brief Demo 2 of 2 for numcomp::CubicSpline: smooth paths and convergence.
 * @author Batte Derrick Calvin (Member 2)
 *
 * - A smooth car speed profile from a few speedometer readings.
 * - How the spline error shrinks as more points are used (about h^4).
 */

#include "numerical-computing/cubic_spline.hpp"
#include "example_helpers.hpp"
#include <algorithm>
#include <cmath>
#include <vector>

/**
 * @brief Runs the demo sections.
 * @return 0 if every section completed, 1 otherwise.
 */
int main()
{
    numcomp::CubicSpline spline;
    demo::title("Cubic Spline (applied)", "Smooth speed profile and convergence study");

    demo::section("Speedometer readings (km/h) every 10 s, smoothed by the spline", [&] {
        std::vector<double> t = {0, 10, 20, 30, 40, 50};
        std::vector<double> v = {0, 32, 55, 61, 58, 70};
        demo::table_header({"time (s)", "speed (km/h)"});
        for (double s = 0.0; s <= 50.0; s += 5.0)
        {
            demo::table_row({s, spline.interpolate(s, t, v)}, 6);
        }
    });

    demo::section("Error of the spline for f(x) = sin(x) on [0, pi] as points are added", [&] {
        // sin'' is zero at 0 and pi, so the natural end conditions are exact and the
        // fourth-order behaviour shows on the whole interval.
        const double pi = std::acos(-1.0);
        demo::table_header({"points", "h", "max error", "ratio"});
        double previous = 0.0;
        for (int n : {5, 9, 17, 33, 65}) 
        {
            std::vector<double> x, y;
            for (int i = 0; i < n; ++i) { x.push_back(pi * i / (n - 1)); y.push_back(std::sin(x.back())); }
            double worst = 0.0;
            for (int k = 0; k <= 2000; ++k) 
            {
                double s = pi * k / 2000.0;
                worst = std::max(worst, std::abs(spline.interpolate(s, x, y) - std::sin(s)));
            }
            demo::table_row({static_cast<double>(n), pi / (n - 1), worst, previous > 0 ? previous / worst : 0.0}, 5);
            previous = worst;
        }
        std::cout << "  Halving h divides the error by about 16: fourth-order accuracy.\n";
    });

    return demo::finish();
}
