/**
 * @file example_advanced_integrator.cpp
 * @brief Demo 1 of 2 for numcomp::AdvancedIntegrator: Simpson's rule and Romberg integration.
 * @author Wasswa George Mpanga (Member 5)
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
    demo::title("Advanced Integrator", "Simpson's 1/3 rule and Romberg integration");

    demo::section("Simpson's rule", [&] {
        demo::compare("x^3 on [0, 2], n = 2 (exact)", integrator.simpsons_rule([](double x) { return x * x * x; }, 0, 2, 2), 4.0);
        demo::compare("sin on [0, pi], n = 20", integrator.simpsons_rule([](double x) { return std::sin(x); }, 0, pi, 20), 2.0);
        demo::compare("e^x on [0, 1], n = 20", integrator.simpsons_rule([](double x) { return std::exp(x); }, 0, 1, 20), std::exp(1.0) - 1.0);
    });

    demo::section("Fourth-order convergence of Simpson: sin on [0, pi]", [&] {
        auto sine = [](double x) { return std::sin(x); };
        demo::table_header({"n", "error", "ratio"});
        double previous = 0.0;
        for (int n : {4, 8, 16, 32})
        {
            double e = std::abs(integrator.simpsons_rule(sine, 0, pi, n) - 2.0);
            demo::table_row({static_cast<double>(n), e, previous > 0 ? previous / e : 0.0}, 5);
            previous = e;
        }
        std::cout << "  Doubling n divides the error by about 16.\n";
    });

    demo::section("Romberg integration: accuracy against the number of rows", [&] {
        auto sine = [](double x) { return std::sin(x); };
        demo::table_header({"rows", "result", "error"});
        for (int m : {1, 2, 3, 4, 5, 6})
        {
            double r = integrator.romberg_integration(sine, 0, pi, m);
            demo::table_row({static_cast<double>(m), r, std::abs(r - 2.0)}, 12);
        }
    });

    demo::section("Invalid input is rejected", [&] {
        demo::show_rejected("Simpson with odd n = 5", [&] { integrator.simpsons_rule([](double x) { return x; }, 0, 1, 5); });
        demo::show_rejected("Romberg with 30 rows", [&] { integrator.romberg_integration([](double x) { return x; }, 0, 1, 30); });
    });

    return demo::finish();
}
