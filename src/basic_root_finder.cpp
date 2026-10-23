#include "numerical-computing/basic_root_finder.hpp"
#include "internal/input_checks.hpp"
#include <cmath>
#include <stdexcept>
#include <utility>   // std::swap

namespace numcomp
{
    /// Bisection method. See basic_root_finder.hpp.
    double BasicRootFinder::bisection(std::function<double(double)> f, double a, double b, double tol, int max_iter)
    {
        checks::check_tolerance(tol, max_iter);   // shared helper: tol > 0, max_iter > 0

        // If the user gives the interval backwards (a > b), swap it.
        // Without this, (b - a) / 2 is negative and the method stops at once.
        if (a > b)
        {
            std::swap(a, b);
        }

        // Work out f at the two ends once, and reuse the values
        double fa = f(a);
        double fb = f(b);

        // NaN passes the sign check below, so reject it here
        if (std::isnan(fa) || std::isnan(fb))
        {
            throw std::invalid_argument("f(a) or f(b) is not a number (NaN).");
        }

        // If a root is exactly at an end point, return it
        if (std::abs(fa) < tol) return a;
        if (std::abs(fb) < tol) return b;

        // The signs must be different, or there may be no root in between
        if (fa * fb > 0)
        {
            throw std::invalid_argument("Function must have opposite signs at interval endpoints.");
        }

        for (int iter = 0; iter < max_iter; ++iter)
        {
            double c = (a + b) / 2.0;   // middle of the interval
            double fc = f(c);

            // Stop if f(c) is small enough or the interval is small enough
            if (std::abs(fc) < tol || (b - a) / 2.0 < tol)
            {
                return c;
            }

            // Keep the half where the sign changes
            if (fc * fa < 0)
            {
                b = c;
            }
            else
            {
                a = c;
                fa = fc;
            }
        }
        throw std::runtime_error("Bisection method failed to converge.");
    }

    /// Newton-Raphson method. See basic_root_finder.hpp.
    double BasicRootFinder::newton_raphson(std::function<double(double)> f, std::function<double(double)> df, double x0, double tol, int max_iter)
    {
        checks::check_tolerance(tol, max_iter);   // shared helper

        double x = x0;
        for (int iter = 0; iter < max_iter; ++iter)
        {
            double derivative = df(x);

            // We divide by the derivative, so it must not be (almost) zero
            if (std::abs(derivative) < 1e-12)
            {
                throw std::runtime_error("Derivative too close to zero.");
            }

            double next_x = x - f(x) / derivative;
            if (std::abs(next_x - x) < tol)
            {
                return next_x;
            }
            x = next_x;
        }
        throw std::runtime_error("Newton-Raphson failed to converge.");
    }
}
