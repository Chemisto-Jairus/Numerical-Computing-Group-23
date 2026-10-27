/**
 * @file cubic_spline.cpp
 * @brief Implementation of numcomp::CubicSpline (natural cubic spline).
 * @author Batte Derrick Calvin (Member 2)
 */

#include "numerical-computing/cubic_spline.hpp"
#include "internal/input_checks.hpp"
#include "internal/tridiagonal.hpp"
#include <utility>

namespace numcomp
{
    std::vector<double> CubicSpline::second_derivatives(const std::vector<double>& x_vals, const std::vector<double>& y_vals)
    {
        checks::check_data_points(x_vals, y_vals);

        int n = static_cast<int>(x_vals.size());

        // Natural boundary conditions: M[0] = M[n-1] = 0.
        std::vector<double> M(n, 0.0);

        // Two points have no inner point; the spline is the straight line between them.
        if (n == 2)
        {
            return M;
        }

        // Interval widths h[i] = x[i+1] - x[i].
        std::vector<double> h(n - 1);
        for (int i = 0; i < n - 1; ++i)
        {
            h[i] = x_vals[i+1] - x_vals[i];
        }

        // Tridiagonal system for the m = n - 2 inner unknowns M[1] .. M[n-2].
        int m = n - 2;
        std::vector<double> lower(m);   // sub-diagonal
        std::vector<double> diag(m);    // main diagonal
        std::vector<double> upper(m);   // super-diagonal
        std::vector<double> rhs(m);     // right-hand side

        for (int k = 0; k < m; ++k)
        {
            int i = k + 1;   // data point that row k belongs to
            lower[k] = h[i-1];
            diag[k]  = 2.0 * (h[i-1] + h[i]);
            upper[k] = h[i];
            rhs[k]   = 6.0 * ((y_vals[i+1] - y_vals[i]) / h[i] - (y_vals[i] - y_vals[i-1]) / h[i-1]);
        }

        // Solve the system with the shared Thomas algorithm (also used by FiniteDifferenceSolver).
        // The matrix is strictly diagonally dominant, so no pivot can become zero.
        // std::move hands the vectors over without copying them; they are not used again.
        std::vector<double> inner = linalg::solve_tridiagonal(std::move(lower), std::move(diag),
                                                               std::move(upper), std::move(rhs));

        for (int k = 0; k < m; ++k)
        {
            M[k+1] = inner[k];
        }

        return M;
    }

    double CubicSpline::interpolate(double x, const std::vector<double>& x_vals, const std::vector<double>& y_vals)
    {
        // Also validates the data.
        std::vector<double> M = second_derivatives(x_vals, y_vals);

        std::size_t i = checks::find_interval(x, x_vals);

        double h  = x_vals[i+1] - x_vals[i];
        double t0 = x - x_vals[i];        // distance from the left data point
        double t1 = x_vals[i+1] - x;      // distance from the right data point

        // Cubic piece on [x_i, x_(i+1)] written in terms of the end values and M.
        double part1 = M[i]   * t1 * t1 * t1 / (6.0 * h);
        double part2 = M[i+1] * t0 * t0 * t0 / (6.0 * h);
        double part3 = (y_vals[i]   / h - M[i]   * h / 6.0) * t1;
        double part4 = (y_vals[i+1] / h - M[i+1] * h / 6.0) * t0;

        return part1 + part2 + part3 + part4;
    }
}
