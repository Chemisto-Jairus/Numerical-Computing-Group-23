/**
 * @file example_advanced_root_finder_applied.cpp
 * @brief Demo 2 of 2 for numcomp::AdvancedRootFinder: applied problems.
 * @author Bakalu Valeria (Member 4)
 *
 * - Interest rate of a loan from its monthly payment (Secant method).
 * - Square roots with the Babylonian iteration g(x) = (x + a/x) / 2 (Fixed-Point).
 * - Water depth in a spherical tank from its volume (Secant method).
 */

#include "numerical-computing/advanced_root_finder.hpp"
#include "example_helpers.hpp"
#include <cmath>

/**
 * @brief Runs the demo sections.
 * @return 0 if every section completed, 1 otherwise.
 */
int main()
{
    numcomp::AdvancedRootFinder finder;
    demo::title("Advanced Root Finder (applied)", "Loan interest rate, Babylonian square roots, tank depth");

    demo::section("Loan: 20,000 borrowed, 60 monthly payments of 400. Monthly rate r?", [&] {
        // Annuity formula: payment = P r / (1 - (1 + r)^-n). Solve f(r) = 0.
        const double P = 20000.0, payment = 400.0;
        const int n = 60;
        auto f = [=](double r) { return P * r / (1.0 - std::pow(1.0 + r, -n)) - payment; };
        double r = finder.secant_method(f, 0.001, 0.02, 1e-14, 100);
        demo::value("Monthly rate r", r, 10);
        demo::value("Annual rate (12 r, %)", 1200.0 * r, 6);
        demo::value("Check: payment at r", P * r / (1.0 - std::pow(1.0 + r, -n)), 6);
    });

    demo::section("Babylonian square roots: g(x) = (x + a / x) / 2", [&] {
        demo::table_header({"a", "fixed point", "sqrt(a)"});
        for (double a : {2.0, 10.0, 144.0, 1e6})
        {
            double r = finder.fixed_point_iteration([=](double x) { return 0.5 * (x + a / x); }, a, 1e-12, 100);
            demo::table_row({a, r, std::sqrt(a)}, 14);
        }
    });

    demo::section("Spherical tank (R = 1 m) holding 1.5 m^3: water depth h?", [&] {
        // Volume of a spherical cap: V(h) = pi h^2 (3R - h) / 3.
        const double pi = std::acos(-1.0), R = 1.0, V = 1.5;
        auto f = [=](double h) { return pi * h * h * (3.0 * R - h) / 3.0 - V; };
        double h = finder.secant_method(f, 0.5, 1.5, 1e-12, 100);
        demo::value("Depth h (m)", h, 10);
        demo::value("Check: volume at h (m^3)", pi * h * h * (3.0 * R - h) / 3.0, 10);
    });

    return demo::finish();
}
