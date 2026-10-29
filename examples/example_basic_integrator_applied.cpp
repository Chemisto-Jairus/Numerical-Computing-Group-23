/**
 * @file example_basic_integrator_applied.cpp
 * @brief Demo 2 of 2 for numcomp::BasicIntegrator: applied integrals.
 * @author Aogon Sharon (Member 3)
 *
 * - Distance travelled from a velocity function.
 * - Probability that a normal variable lies within one standard deviation.
 * - Work done by a spring.
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
    demo::title("Basic Integrator (applied)", "Distance, probability and work");

    demo::section("Distance: v(t) = 3 t^2 + 2 t m/s from t = 0 to 4 s", [&] {
        auto v = [](double t) { return 3.0 * t * t + 2.0 * t; };
        // Exact: [t^3 + t^2] from 0 to 4 = 80 m.
        demo::compare("Trapezoid, n = 200", integrator.trapezoidal_rule(v, 0, 4, 200), 80.0);
        demo::compare("Midpoint,  n = 200", integrator.midpoint_rule(v, 0, 4, 200), 80.0);
    });

    demo::section("Normal distribution: P(-1 < Z < 1)", [&] {
        auto pdf = [=](double z) { return std::exp(-0.5 * z * z) / std::sqrt(2.0 * pi); };
        double exact = std::erf(1.0 / std::sqrt(2.0));
        demo::compare("Trapezoid, n = 100", integrator.trapezoidal_rule(pdf, -1, 1, 100), exact);
        demo::compare("Midpoint,  n = 100", integrator.midpoint_rule(pdf, -1, 1, 100), exact);
    });

    demo::section("Work to stretch a spring (k = 250 N/m) from 0.1 m to 0.3 m", [&] {
        auto force = [](double x) { return 250.0 * x; };
        // Exact: k (0.3^2 - 0.1^2) / 2 = 10 J. A straight-line force is integrated exactly.
        demo::compare("Trapezoid, n = 1", integrator.trapezoidal_rule(force, 0.1, 0.3, 1), 10.0);
        demo::compare("Midpoint,  n = 1", integrator.midpoint_rule(force, 0.1, 0.3, 1), 10.0);
    });

    return demo::finish();
}
