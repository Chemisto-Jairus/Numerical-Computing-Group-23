/**
 * @file example_interpolator_applied.cpp
 * @brief Demo 2 of 2 for numcomp::Interpolator: table look-up and sampling design.
 * @author Batte Derrick Calvin (Member 2)
 *
 * - Reading values between the rows of a measured temperature table.
 * - Replacing the Runge function by one Chebyshev approximation polynomial.
 * - Comparing equally spaced samples with Chebyshev samples when a curve
 *   is rebuilt by linear interpolation, and explaining which wins and why.
 */

#include "numerical-computing/interpolator.hpp"
#include "example_helpers.hpp"
#include <algorithm>
#include <cmath>
#include <vector>

/**
 * @brief Largest error of linear interpolation through the given samples of f on [a, b].
 * @tparam F     Callable double(double): the function being sampled.
 * @param interp  Interpolator used for the reconstruction.
 * @param f       The function being sampled.
 * @param xs      Sample positions, strictly increasing, covering [a, b].
 * @param a       Left end of the interval.
 * @param b       Right end of the interval.
 * @return The maximum absolute error over 2001 test points.
 */
template <typename F>
double max_linear_error(numcomp::Interpolator& interp, F f, const std::vector<double>& xs, double a, double b)
{
    std::vector<double> ys;
    for (double x : xs) ys.push_back(f(x));
    double worst = 0.0;
    for (int k = 0; k <= 2000; ++k)
    {
        double t = a + (b - a) * k / 2000.0;
        worst = std::max(worst, std::abs(interp.linear_interpolate(t, xs, ys) - f(t)));
    }
    return worst;
}

/**
 * @brief Runs the demo sections.
 * @return 0 if every section completed, 1 otherwise.
 */
int main()
{
    numcomp::Interpolator interp;
    demo::title("Interpolator (applied)", "Table look-up, sample points and a global Chebyshev polynomial");

    demo::section("Hourly temperature readings (deg C) between 06:00 and 18:00", [&] {
        std::vector<double> hour = {6, 8, 10, 12, 14, 16, 18};
        std::vector<double> temp = {14.2, 17.8, 22.5, 26.1, 27.4, 24.9, 20.3};
        demo::value("Estimated at 07:00", interp.linear_interpolate(7.0, hour, temp), 2);
        demo::value("Estimated at 11:30", interp.linear_interpolate(11.5, hour, temp), 2);
        demo::value("Estimated at 15:15", interp.linear_interpolate(15.25, hour, temp), 2);
        demo::show_rejected("Estimate at 20:00 (outside the table)", [&] { interp.linear_interpolate(20.0, hour, temp); });
    });

    demo::section("Rebuilding f(x) = 1 / (1 + 25 x^2) on [-1, 1] from n samples", [&] {
        auto f = [](double x) { return 1.0 / (1.0 + 25.0 * x * x); };
        demo::table_header({"n", "equal", "Chebyshev"});
        for (int n : {5, 9, 17, 33})
        {
            std::vector<double> equal;
            for (int i = 0; i < n; ++i) equal.push_back(-1.0 + 2.0 * i / (n - 1));

            // Chebyshev nodes are interior points; add both ends so the data covers [-1, 1].
            std::vector<double> cheb = interp.chebyshev_nodes(-1.0, 1.0, n - 2);
            cheb.push_back(-1.0);
            cheb.push_back(1.0);
            std::sort(cheb.begin(), cheb.end());

            demo::table_row({static_cast<double>(n), max_linear_error(interp, f, equal, -1.0, 1.0), max_linear_error(interp, f, cheb, -1.0, 1.0)}, 6);
        }
        std::cout << "  Largest error of the piecewise linear reconstruction for each set of samples.\n";
        std::cout << "  Equal spacing wins here: Chebyshev nodes pay off for one global polynomial\n";
        std::cout << "  through all the points (they stop Runge oscillations), not for piecewise\n";
        std::cout << "  linear interpolation, where the error depends on the widest gap.\n";
    });

    demo::section("One global polynomial for 1 / (1 + 25 x^2): Chebyshev approximation", [&] {
        auto f = [](double x) { return 1.0 / (1.0 + 25.0 * x * x); };
        demo::table_header({"degree", "max error"});
        for (int n : {5, 9, 17, 33, 65})
        {
            std::vector<double> c = interp.chebyshev_coefficients(f, -1.0, 1.0, n);
            double worst = 0.0;
            for (int k = 0; k <= 400; ++k)
            {
                double x = -1.0 + k / 200.0;
                worst = std::max(worst, std::abs(interp.chebyshev_approximate(c, -1.0, 1.0, x) - f(x)));
            }
            demo::table_row({static_cast<double>(n - 1), worst}, 4);
        }
        std::cout << "  Built on Chebyshev nodes, a single polynomial converges to the Runge function\n";
        std::cout << "  as the degree grows. Through equally spaced points it would oscillate instead.\n";
    });

    return demo::finish();
}
