/**
 * @file example_interpolator.cpp
 * @brief Demo 1 of 2 for numcomp::Interpolator: linear interpolation, Chebyshev nodes
 *        and Chebyshev approximation.
 * @author Batte Derrick Calvin (Member 2)
 */

#include "numerical-computing/interpolator.hpp"
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
    numcomp::Interpolator interp;
    demo::title("Interpolator", "Linear interpolation, Chebyshev nodes and Chebyshev approximation");

    demo::section("Linear interpolation of y = x^2 sampled at x = 0, 1, 2, 3, 4", [&] {
        std::vector<double> x = {0.0, 1.0, 2.0, 3.0, 4.0};
        std::vector<double> y = {0.0, 1.0, 4.0, 9.0, 16.0};
        demo::table_header({"x", "linear", "exact x^2", "error"});
        for (double t : {0.5, 1.5, 2.25, 3.0, 3.9})
        {
            double v = interp.linear_interpolate(t, x, y);
            demo::table_row({t, v, t * t, std::abs(v - t * t)});
        }
        std::cout << "  Data points are reproduced exactly; between them the chord lies above the convex curve.\n";
    });

    demo::section("Uneven spacing and negative values", [&] {
        std::vector<double> x = {-3.0, -1.0, 0.5, 4.0};
        std::vector<double> y = {9.0, -2.0, 1.0, 8.0};
        demo::value("y(-2)", interp.linear_interpolate(-2.0, x, y), 6);
        demo::value("y(0)", interp.linear_interpolate(0.0, x, y), 6);
        demo::value("y(2.25)", interp.linear_interpolate(2.25, x, y), 6);
    });

    demo::section("Chebyshev nodes on [-1, 1]", [&] {
        for (int n : {3, 5, 8})
        {
            std::vector<double> nodes = interp.chebyshev_nodes(-1.0, 1.0, n);
            std::cout << "  n = " << n << ":";
            for (double v : nodes) std::cout << "  " << std::setprecision(4) << std::fixed << v;
            std::cout << "\n";
        }
        std::cout << "  The nodes crowd towards the ends of the interval.\n";
    });

    demo::section("Chebyshev approximation of e^x on [0, 1]", [&] {
        auto f = [](double x) { return std::exp(x); };
        demo::table_header({"terms n", "max error", "last coeff"});
        for (int n : {2, 4, 6, 8, 10, 12})
        {
            std::vector<double> c = interp.chebyshev_coefficients(f, 0.0, 1.0, n);
            double worst = 0.0;
            for (int k = 0; k <= 100; ++k)
            {
                double x = k / 100.0;
                worst = std::max(worst, std::abs(interp.chebyshev_approximate(c, 0.0, 1.0, x) - f(x)));
            }
            demo::table_row({static_cast<double>(n), worst, c[n - 1]}, 4);
        }
        std::cout << "  The error falls by a constant factor with each extra term, and the size\n";
        std::cout << "  of the last coefficient shows roughly how large the error is.\n";
    });

    demo::section("Invalid input is rejected", [&] {
        demo::show_rejected("x outside the data (x = 5)", [&] { interp.linear_interpolate(5.0, {0.0, 1.0, 2.0}, {0.0, 1.0, 4.0}); });
        demo::show_rejected("x values not increasing", [&] { interp.linear_interpolate(0.5, {0.0, 2.0, 1.0}, {0.0, 4.0, 1.0}); });
        demo::show_rejected("Chebyshev with a > b", [&] { interp.chebyshev_nodes(2.0, 1.0, 4); });
        demo::show_rejected("Approximation evaluated outside [a, b]", [&] {
            std::vector<double> c = interp.chebyshev_coefficients([](double x) { return x; }, 0.0, 1.0, 3);
            interp.chebyshev_approximate(c, 0.0, 1.0, 2.0);
        });
    });

    return demo::finish();
}
