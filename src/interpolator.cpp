/**
 * @file interpolator.cpp
 * @brief Implementation of numcomp::Interpolator (linear interpolation, Chebyshev nodes
 *        and Chebyshev approximation).
 * @author Batte Derrick Calvin (Member 2)
 */

#include "numerical-computing/interpolator.hpp"
#include "internal/input_checks.hpp"
#include <cmath>
#include <stdexcept>

#ifndef M_PI
/// Pi, defined here for compilers whose cmath header does not provide M_PI (for example MSVC).
#define M_PI 3.14159265358979323846
#endif

namespace numcomp
{
    double Interpolator::linear_interpolate(double x, const std::vector<double>& x_vals, const std::vector<double>& y_vals)
    {
        checks::check_data_points(x_vals, y_vals);
        std::size_t i = checks::find_interval(x, x_vals);

        double x0 = x_vals[i], x1 = x_vals[i+1];
        double y0 = y_vals[i], y1 = y_vals[i+1];

        // Point on the straight line through (x0, y0) and (x1, y1).
        return y0 + (y1 - y0) * (x - x0) / (x1 - x0);
    }

    std::vector<double> Interpolator::chebyshev_nodes(double a, double b, int n)
    {
        if (n <= 0)
        {
            throw std::invalid_argument("Number of nodes must be strictly positive.");
        }

        // Shared memory limit with the ODE solvers.
        if (n > checks::MAX_POINTS)
        {
            throw std::invalid_argument("Number of nodes must not be more than 1,000,000.");
        }

        // Written as !(a < b) so that NaN end points are rejected as well.
        if (!(a < b))
        {
            throw std::invalid_argument("a must be less than b.");
        }

        std::vector<double> nodes(n);
        for (int k = 1; k <= n; ++k)
        {
            // Root of T_n on [-1, 1], shifted and scaled onto [a, b].
            nodes[k-1] = 0.5 * (a + b) + 0.5 * (b - a) * std::cos((2.0 * k - 1.0) * M_PI / (2.0 * n));
        }
        return nodes;
    }

    /// Largest number of Chebyshev coefficients: computing them costs n^2 cosine evaluations.
    static const int MAX_CHEBYSHEV_TERMS = 1000;

    std::vector<double> Interpolator::chebyshev_coefficients(std::function<double(double)> f, double a, double b, int n)
    {
        if (n > MAX_CHEBYSHEV_TERMS)
        {
            throw std::invalid_argument("Number of Chebyshev coefficients must not be more than 1,000.");
        }

        // Reuses the node formula (and its checks of a, b and n) instead of repeating it.
        std::vector<double> nodes = chebyshev_nodes(a, b, n);

        // Sample f once at every node.
        std::vector<double> fx(n);
        for (int k = 0; k < n; ++k)
        {
            fx[k] = f(nodes[k]);
            checks::check_finite_result(fx[k]);
        }

        // c_j = (2/n) sum_k f(x_k) cos(j theta_k), with theta_k = (2k + 1) pi / (2n) for k = 0..n-1.
        std::vector<double> coeffs(n, 0.0);
        for (int j = 0; j < n; ++j)
        {
            double sum = 0.0;
            for (int k = 0; k < n; ++k)
            {
                sum += fx[k] * std::cos(j * (2.0 * k + 1.0) * M_PI / (2.0 * n));
            }
            coeffs[j] = 2.0 * sum / n;
        }
        return coeffs;
    }

    double Interpolator::chebyshev_approximate(const std::vector<double>& coeffs, double a, double b, double x)
    {
        if (coeffs.empty())
        {
            throw std::invalid_argument("The list of Chebyshev coefficients must not be empty.");
        }
        if (!(a < b))
        {
            throw std::invalid_argument("a must be less than b.");
        }
        // Written as !(...) so that a NaN x is rejected as well.
        if (!(x >= a && x <= b))
        {
            throw std::out_of_range("x is outside the interval [a, b] of the approximation.");
        }

        // Map [a, b] onto [-1, 1], where the Chebyshev polynomials are defined.
        double u = (2.0 * x - a - b) / (b - a);

        // Clenshaw's recurrence, from the highest coefficient down to c_1.
        double b1 = 0.0;   // b_(j+1)
        double b2 = 0.0;   // b_(j+2)
        for (std::size_t j = coeffs.size() - 1; j >= 1; --j)
        {
            double bj = 2.0 * u * b1 - b2 + coeffs[j];
            b2 = b1;
            b1 = bj;
        }
        return u * b1 - b2 + 0.5 * coeffs[0];
    }
}
