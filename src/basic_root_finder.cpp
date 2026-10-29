/**
 * @file basic_root_finder.cpp
 * @brief Implementation of numcomp::BasicRootFinder (Bisection and Newton-Raphson).
 * @author Chemisto Jairus (Member 1)
 */

#include "numerical-computing/basic_root_finder.hpp"
#include "internal/input_checks.hpp"
#include <cmath>
#include <stdexcept>
#include <utility>

namespace numcomp
{
    double BasicRootFinder::bisection(std::function<double(double)> f, double a, double b, double tol, int max_iter)
    {
        checks::check_tolerance(tol, max_iter);

        // Work with a <= b so that (b - a) / 2 is always the true half-width.
        if (a > b)
        {
            std::swap(a, b);
        }

        // f is evaluated once per point; fa is kept up to date as a moves.
        double fa = f(a);
        double fb = f(b);

        // NaN compares false with everything, so it must be rejected explicitly.
        if (std::isnan(fa) || std::isnan(fb))
        {
            throw std::invalid_argument("f(a) or f(b) is not a number (NaN).");
        }

        // A root sitting exactly on an end point is returned straight away.
        if (std::abs(fa) < tol) return a;
        if (std::abs(fb) < tol) return b;

        // Without a sign change the interval is not guaranteed to contain a root.
        if (fa * fb > 0)
        {
            throw std::invalid_argument("Function must have opposite signs at interval endpoints.");
        }

        for (int iter = 0; iter < max_iter; ++iter)
        {
            double c = (a + b) / 2.0;
            double fc = f(c);

            // Converged: the residual is small or the bracket is narrower than tol.
            if (std::abs(fc) < tol || (b - a) / 2.0 < tol)
            {
                return c;
            }

            // Keep the half [a, c] or [c, b] in which f still changes sign.
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

    double BasicRootFinder::newton_raphson(std::function<double(double)> f, std::function<double(double)> df, double x0, double tol, int max_iter)
    {
        checks::check_tolerance(tol, max_iter);

        double x = x0;
        for (int iter = 0; iter < max_iter; ++iter)
        {
            double derivative = df(x);

            // A (nearly) flat tangent never reaches the x-axis, so the step is undefined.
            if (std::abs(derivative) < 1e-12)
            {
                throw std::runtime_error("Derivative too close to zero.");
            }

            // Move to where the tangent line at x crosses the x-axis.
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
