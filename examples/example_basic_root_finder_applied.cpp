/**
 * @file example_basic_root_finder_applied.cpp
 * @brief Demo 2 of 2 for numcomp::BasicRootFinder: applied problems.
 * @author Chemisto Jairus (Member 1)
 *
 * - Projectile: when does a ball thrown upwards hit the ground?
 * - Kepler's equation M = E - e sin(E) for an orbit (Newton-Raphson).
 * - Break-even point of a business (Bisection).
 */

#include "numerical-computing/basic_root_finder.hpp"
#include "example_helpers.hpp"
#include <cmath>
#include <stdexcept>

/**
 * @brief Runs the demo sections.
 * @return 0 if every section completed, 1 otherwise.
 */
int main()
{
    numcomp::BasicRootFinder finder;
    demo::title("Basic Root Finder (applied)", "Projectile flight time, Kepler's equation, break-even point");

    demo::section("Projectile: h(t) = 1.5 + 20 t - 4.905 t^2 = 0", [&] {
        // Thrown from 1.5 m at 20 m/s; g = 9.81 m/s^2. The positive root is the landing time.
        auto h  = [](double t) { return 1.5 + 20.0 * t - 4.905 * t * t; };
        auto dh = [](double t) { return 20.0 - 9.81 * t; };
        double exact = (20.0 + std::sqrt(400.0 + 4.0 * 4.905 * 1.5)) / (2.0 * 4.905);
        demo::compare("Landing time (bisection)", finder.bisection(h, 1.0, 10.0, 1e-12, 200), exact);
        demo::compare("Landing time (Newton)", finder.newton_raphson(h, dh, 5.0, 1e-12, 50), exact);
    });

    demo::section("Kepler's equation: E - e sin(E) = M  (e = 0.3, M = 1.2 rad)", [&] {
        const double e = 0.3;
        const double M = 1.2;
        auto f  = [=](double E) { return E - e * std::sin(E) - M; };
        auto df = [=](double E) { return 1.0 - e * std::cos(E); };
        double E = finder.newton_raphson(f, df, M, 1e-14, 50);
        demo::value("Eccentric anomaly E (rad)", E, 12);
        demo::value("Check: E - e sin(E)", E - e * std::sin(E), 12);
        demo::value("True anomaly (rad)", 2.0 * std::atan(std::sqrt((1 + e) / (1 - e)) * std::tan(E / 2.0)), 12);
    });

    demo::section("Break-even: revenue 50 q = cost 2000 + 10 q + 0.02 q^2", [&] {
        // Profit P(q) = 50 q - 2000 - 10 q - 0.02 q^2 has two roots; the first is the break-even quantity.
        auto profit = [](double q) { return 40.0 * q - 2000.0 - 0.02 * q * q; };
        double exact = (40.0 - std::sqrt(1600.0 - 4.0 * 0.02 * 2000.0)) / (2.0 * 0.02);
        demo::compare("Break-even quantity", finder.bisection(profit, 0.0, 500.0, 1e-10, 200), exact);
    });

    demo::section("Iterations needed for tol = 1e-12: Newton vs bisection (x^2 - 2)", [&] {
        auto f  = [](double x) { return x * x - 2.0; };
        auto df = [](double x) { return 2.0 * x; };
        demo::table_header({"max_iter", "bisection", "newton"});
        for (int it : {3, 5, 10, 20, 30, 45})
        {
            double b = std::nan("");
            double n = std::nan("");
            try { b = finder.bisection(f, 0.0, 2.0, 1e-12, it); } catch (const std::runtime_error&) {}
            try { n = finder.newton_raphson(f, df, 1.0, 1e-12, it); } catch (const std::runtime_error&) {}
            demo::table_row({static_cast<double>(it), b, n}, 12);
        }
        std::cout << "  (nan = not converged within max_iter: Newton needs far fewer steps)\n";
    });

    return demo::finish();
}
