/**
 * @file example_numerical_differentiator_applied.cpp
 * @brief Demo 2 of 2 for numcomp::NumericalDifferentiator: applied problems.
 * @author Chemisto Jairus (Member 1)
 *
 * - Velocity and acceleration of a car from its position formula.
 * - Marginal cost in economics.
 * - Newton-Raphson without a hand-written derivative (uses BasicRootFinder too).
 */

#include "numerical-computing/numerical_differentiator.hpp"
#include "numerical-computing/basic_root_finder.hpp"
#include "example_helpers.hpp"
#include <cmath>

/**
 * @brief Runs the demo sections.
 * @return 0 if every section completed, 1 otherwise.
 */
int main()
{
    numcomp::NumericalDifferentiator d;
    demo::title("Numerical Differentiator (applied)", "Motion, marginal cost and a derivative-free Newton method");

    demo::section("Car position s(t) = 2 t^3 - 3 t^2 + 5 t (metres): velocity and acceleration", [&] {
        auto s = [](double t) { return 2.0 * t * t * t - 3.0 * t * t + 5.0 * t; };
        demo::table_header({"t (s)", "v (m/s)", "exact v", "a (m/s^2)", "exact a"});
        for (double t : {0.0, 1.0, 2.0, 3.0})
        {
            demo::table_row({t, d.central_difference(s, t, 1e-4), 6.0 * t * t - 6.0 * t + 5.0,
                             d.second_derivative(s, t, 1e-3), 12.0 * t - 6.0}, 8);
        }
    });

    demo::section("Marginal cost: C(q) = 500 + 20 q + 2000 ln(1 + q / 50) dollars", [&] {
        auto cost = [](double q) { return 500.0 + 20.0 * q + 2000.0 * std::log(1.0 + q / 50.0); };
        for (double q : {10.0, 100.0, 400.0})
        {
            double exact = 20.0 + 2000.0 / (50.0 + q);
            demo::compare("C'(" + std::to_string(static_cast<int>(q)) + ") ($ per unit)", d.richardson_extrapolation(cost, q, 1.0), exact);
        }
        std::cout << "  C'(q) is the cost of producing one more unit; it falls as production grows.\n";
    });

    demo::section("Newton-Raphson for x^3 - 2x - 5 = 0 with a numerical derivative", [&] {
        numcomp::BasicRootFinder finder;
        auto f = [](double x) { return x * x * x - 2.0 * x - 5.0; };
        auto df_numeric = [&](double x) { return d.central_difference(f, x, 1e-6); };
        auto df_exact = [](double x) { return 3.0 * x * x - 2.0; };
        double root = 2.0945514815423265;
        demo::compare("Newton with exact f'", finder.newton_raphson(f, df_exact, 2.0, 1e-12, 50), root);
        demo::compare("Newton with numerical f'", finder.newton_raphson(f, df_numeric, 2.0, 1e-12, 50), root);
        std::cout << "  The numerical derivative is accurate enough for Newton's method to keep its speed.\n";
    });

    return demo::finish();
}
