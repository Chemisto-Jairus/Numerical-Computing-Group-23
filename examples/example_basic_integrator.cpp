/**
 * @file example_basic_integrator.cpp
 * @brief Demo 1 of 2 for numcomp::BasicIntegrator: Trapezoidal and Midpoint rules.
 * @author Aogon Sharon (Member 3)
 */

#include "numerical-computing/basic_integrator.hpp"
#include "example_helpers.hpp"
#include <cmath>

/**
 * @brief Runs the demo sections.
 * @return 0 if every section completed, 1 otherwise.
 */
int main()
{
    numcomp::BasicIntegrator integrator;
    const double pi = std::acos(-1.0);
    demo::title("Basic Integrator", "Composite Trapezoidal and Midpoint rules");

    demo::section("Standard integrals with n = 100 strips", [&] {
        auto square = [](double x) { return x * x; };
        auto sine = [](double x) { return std::sin(x); };
        auto expo = [](double x) { return std::exp(x); };
        demo::compare("Trapezoid: x^2 on [0, 1]", integrator.trapezoidal_rule(square, 0, 1, 100), 1.0 / 3.0);
        demo::compare("Midpoint:  x^2 on [0, 1]", integrator.midpoint_rule(square, 0, 1, 100), 1.0 / 3.0);
        demo::compare("Trapezoid: sin on [0, pi]", integrator.trapezoidal_rule(sine, 0, pi, 100), 2.0);
        demo::compare("Midpoint:  sin on [0, pi]", integrator.midpoint_rule(sine, 0, pi, 100), 2.0);
        demo::compare("Trapezoid: e^x on [0, 1]", integrator.trapezoidal_rule(expo, 0, 1, 100), std::exp(1.0) - 1.0);
        demo::compare("Midpoint:  e^x on [0, 1]", integrator.midpoint_rule(expo, 0, 1, 100), std::exp(1.0) - 1.0);
    });

    demo::section("Second-order convergence: error vs n for sin on [0, pi]", [&] {
        auto sine = [](double x) { return std::sin(x); };
        demo::table_header({"n", "trapezoid err", "midpoint err", "trap ratio"});
        double previous = 0.0;
        for (int n : {4, 8, 16, 32, 64})
        {
            double et = std::abs(integrator.trapezoidal_rule(sine, 0, pi, n) - 2.0);
            double em = std::abs(integrator.midpoint_rule(sine, 0, pi, n) - 2.0);
            demo::table_row({static_cast<double>(n), et, em, previous > 0 ? previous / et : 0.0}, 5);
            previous = et;
        }
        std::cout << "  Doubling n divides the error by about 4; the midpoint error is about half.\n";
    });

    demo::section("Reversed limits change the sign", [&] {
        demo::compare("Trapezoid: x^2 from 1 to 0", integrator.trapezoidal_rule([](double x) { return x * x; }, 1, 0, 1000), -1.0 / 3.0);
    });

    demo::section("Invalid input is rejected", [&] {
        demo::show_rejected("n = 0 strips", [&] { integrator.trapezoidal_rule([](double x) { return x; }, 0, 1, 0); });
        demo::show_rejected("Infinite upper limit", [&] { integrator.midpoint_rule([](double x) { return x; }, 0, INFINITY, 10); });
    });

    return demo::finish();
}
