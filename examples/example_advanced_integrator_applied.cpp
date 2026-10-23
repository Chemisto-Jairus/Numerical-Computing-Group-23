/**
 * @file example_advanced_integrator_applied.cpp
 * @brief Demo 2 of 2 for numcomp::AdvancedIntegrator: applied integrals.
 * @author Wasswa George Mpanga (Member 5)
 *
 * - The error function erf(1).
 * - Arc length of the parabola y = x^2.
 * - Comparing the cost of Simpson and Romberg for the same accuracy.
 */

#include "numerical-computing/advanced_integrator.hpp"
#include "example_helpers.hpp"
#include <cmath>

/**
 * @brief Runs the demo sections.
 * @return 0 if every section completed, 1 otherwise.
 */
int main()
{
    numcomp::AdvancedIntegrator integrator;
    const double pi = std::acos(-1.0);
    demo::title("Advanced Integrator (applied)", "Error function, arc length, cost comparison");

    demo::section("erf(1) = (2 / sqrt(pi)) * integral of e^(-t^2) from 0 to 1", [&] {
        auto g = [](double t) { return std::exp(-t * t); };
        double scale = 2.0 / std::sqrt(pi);
        demo::compare("Simpson, n = 10", scale * integrator.simpsons_rule(g, 0, 1, 10), std::erf(1.0));
        demo::compare("Romberg, 5 rows", scale * integrator.romberg_integration(g, 0, 1, 5), std::erf(1.0));
    });

    demo::section("Arc length of y = x^2 from x = 0 to 1: integral of sqrt(1 + 4x^2)", [&] {
        auto ds = [](double x) { return std::sqrt(1.0 + 4.0 * x * x); };
        double exact = std::sqrt(5.0) / 2.0 + std::asinh(2.0) / 4.0;
        demo::compare("Simpson, n = 20", integrator.simpsons_rule(ds, 0, 1, 20), exact);
        demo::compare("Romberg, 6 rows", integrator.romberg_integration(ds, 0, 1, 6), exact);
    });

    demo::section("Function evaluations needed for error < 1e-10 (integral of e^x on [0, 1])", [&] {
        int calls = 0;
        auto counted = [&calls](double x) { ++calls; return std::exp(x); };
        double exact = std::exp(1.0) - 1.0;

        int n = 2;
        while (true)
        {
            calls = 0;
            if (std::abs(integrator.simpsons_rule(counted, 0, 1, n) - exact) < 1e-10) break;
            n *= 2;
        }
        demo::value("Simpson: strips n", n, 0);
        demo::value("Simpson: f evaluations", calls, 0);

        int rows = 1;
        while (true)
        {
            calls = 0;
            if (std::abs(integrator.romberg_integration(counted, 0, 1, rows) - exact) < 1e-10) break;
            ++rows;
        }
        demo::value("Romberg: rows", rows, 0);
        demo::value("Romberg: f evaluations", calls, 0);
    });

    return demo::finish();
}
