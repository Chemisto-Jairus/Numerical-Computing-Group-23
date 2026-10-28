#include "numerical-computing/finite_difference_solver.hpp"
#include "internal/input_checks.hpp"
#include "internal/tridiagonal.hpp"
#include <cmath>
#include <stdexcept>
#include <utility>

namespace numcomp
{
    std::vector<std::pair<double, double>> FiniteDifferenceSolver::linear_bvp(std::function<double(double)> p, std::function<double(double)> q,
                                                                              std::function<double(double)> r, double a, double b,
                                                                              double alpha, double beta, int n)
    {
        // Shared checks: n > 0 and finite a, b. Then the rules specific to this method.
        checks::check_integration_inputs(a, b, n);
        if (n < 2)
        {
            throw std::invalid_argument("The grid needs at least 2 intervals (one inner point).");
        }
        if (n > checks::MAX_POINTS)
        {
            throw std::invalid_argument("Number of grid intervals must not be more than 1,000,000.");
        }
        if (!(a < b))
        {
            throw std::invalid_argument("a must be less than b.");
        }
        if (!std::isfinite(alpha) || !std::isfinite(beta))
        {
            throw std::invalid_argument("Boundary values alpha and beta must be finite numbers.");
        }

        double h = (b - a) / n;
        int m = n - 1;   // number of inner grid points, i.e. unknowns

        std::vector<double> lower(m), diag(m), upper(m), rhs(m);
        for (int k = 0; k < m; ++k)
        {
            double x  = a + (k + 1) * h;   // inner point x_i with i = k + 1
            double p_i = p(x);
            double q_i = q(x);
            double r_i = r(x);
            checks::check_finite_result(p_i + q_i + r_i);

            lower[k] = -(1.0 + h * p_i / 2.0);   // coefficient of y_(i-1)
            diag[k]  = 2.0 + h * h * q_i;         // coefficient of y_i
            upper[k] = -(1.0 - h * p_i / 2.0);   // coefficient of y_(i+1)
            rhs[k]   = -h * h * r_i;
        }

        // Known boundary values move to the right-hand side.
        rhs[0]   -= lower[0] * alpha;
        rhs[m-1] -= upper[m-1] * beta;

        // Shared Thomas algorithm (also used by CubicSpline). The vectors are not needed afterwards.
        std::vector<double> inner = linalg::solve_tridiagonal(std::move(lower), std::move(diag),
                                                              std::move(upper), std::move(rhs));

        // Assemble the full grid, including both boundary points.
        std::vector<std::pair<double, double>> grid;
        grid.reserve(n + 1);
        grid.push_back({a, alpha});
        for (int k = 0; k < m; ++k)
        {
            grid.push_back({a + (k + 1) * h, inner[k]});
        }
        grid.push_back({b, beta});   // exactly b, without rounding from a + n h
        return grid;
    }

    std::vector<std::pair<double, double>> FiniteDifferenceSolver::poisson(std::function<double(double)> r, double a, double b,
                                                                           double alpha, double beta, int n)
    {
        // y'' = r(x) is the case p = q = 0 of the general problem.
        auto zero = [](double) { return 0.0; };
        return linear_bvp(zero, zero, r, a, b, alpha, beta, n);
    }
}
