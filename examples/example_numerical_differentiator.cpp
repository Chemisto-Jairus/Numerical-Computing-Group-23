/**
 * @file example_numerical_differentiator.cpp
 * @brief Demo 1 of 2 for numcomp::NumericalDifferentiator: the five difference formulas.
 * @author Chemisto Jairus (Member 1)
 */

#include "numerical-computing/numerical_differentiator.hpp"
#include "example_helpers.hpp"
#include <cmath>

/**
 * @brief Runs the demo sections.
 * @return 0 if every section completed, 1 otherwise.
 */
int main()
{
    numcomp::NumericalDifferentiator d;
    demo::title("Numerical Differentiator", "Forward, backward, central, second derivative, Richardson");

    demo::section("d/dx sin(x) at x = 1 (exact cos 1 = 0.5403023059)", [&] {
        auto f = [](double x) { return std::sin(x); };
        double h = 0.01;
        demo::compare("Forward,  h = 0.01", d.forward_difference(f, 1.0, h), std::cos(1.0));
        demo::compare("Backward, h = 0.01", d.backward_difference(f, 1.0, h), std::cos(1.0));
        demo::compare("Central,  h = 0.01", d.central_difference(f, 1.0, h), std::cos(1.0));
        demo::compare("Richardson, h = 0.01", d.richardson_extrapolation(f, 1.0, h), std::cos(1.0));
        demo::compare("Second deriv., h = 0.01", d.second_derivative(f, 1.0, h), -std::sin(1.0));
    });

    demo::section("Order of accuracy: error of each formula as h is halved", [&] {
        auto f = [](double x) { return std::exp(x); };   // every derivative of e^x at 0 is 1
        demo::table_header({"h", "forward", "central", "second", "Richardson"});
        for (double h : {0.1, 0.05, 0.025, 0.0125})
        {
            demo::table_row({h, std::abs(d.forward_difference(f, 0.0, h) - 1.0), std::abs(d.central_difference(f, 0.0, h) - 1.0),
                             std::abs(d.second_derivative(f, 0.0, h) - 1.0), std::abs(d.richardson_extrapolation(f, 0.0, h) - 1.0)}, 4);
        }
        std::cout << "  Halving h divides the errors by about 2, 4, 4 and 16: orders 1, 2, 2 and 4.\n";
    });

    demo::section("Too small a step: rounding error takes over (central difference of e^x at 0)", [&] {
        auto f = [](double x) { return std::exp(x); };
        demo::table_header({"h", "error"});
        for (double h : {1e-2, 1e-4, 1e-6, 1e-8, 1e-10, 1e-12})
        {
            demo::table_row({h, std::abs(d.central_difference(f, 0.0, h) - 1.0)}, 3);
        }
        std::cout << "  The error is smallest near h = 1e-5. Below that, f(x + h) and f(x - h) agree in\n";
        std::cout << "  so many digits that their difference is mostly rounding noise.\n";
    });

    demo::section("A corner: f(x) = |x| at x = 0 has no derivative", [&] {
        auto f = [](double x) { return std::abs(x); };
        demo::value("Forward (slope on the right)", d.forward_difference(f, 0.0, 0.1), 4);
        demo::value("Backward (slope on the left)", d.backward_difference(f, 0.0, 0.1), 4);
        demo::value("Central (average of the two)", d.central_difference(f, 0.0, 0.1), 4);
    });

    demo::section("Invalid input is rejected", [&] {
        auto f = [](double x) { return std::sin(x); };
        demo::show_rejected("Step size h = 0", [&] { d.central_difference(f, 1.0, 0.0); });
        demo::show_rejected("Negative step size", [&] { d.forward_difference(f, 1.0, -0.1); });
        demo::show_rejected("log(x) needs x - h > 0", [&] { d.central_difference([](double x) { return std::log(x); }, 0.05, 0.1); });
    });

    return demo::finish();
}
