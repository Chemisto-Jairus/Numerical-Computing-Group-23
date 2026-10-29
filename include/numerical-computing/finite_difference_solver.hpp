#pragma once
/**
 * @file finite_difference_solver.hpp
 * @brief The finite difference method for linear two-point boundary value problems.
 * @author Aogon Sharon (Member 3)
 */

#include <functional>
#include <utility>
#include <vector>

namespace numcomp
{
    /**
     * @brief Solves y'' = p(x) y' + q(x) y + r(x) on [a, b] with y(a) = alpha, y(b) = beta.
     *
     * Unlike an initial value problem, the conditions are given at both ends,
     * so the solution cannot be marched forward from one side. The finite
     * difference method instead places n + 1 equally spaced grid points
     * x_i = a + i h, h = (b - a)/n, and replaces the derivatives at every
     * inner point with central differences:
     * y'' ~ (y_(i-1) - 2 y_i + y_(i+1)) / h^2 and y' ~ (y_(i+1) - y_(i-1)) / (2h).
     * This gives one linear equation per inner point. Each equation links only
     * three neighbours, so the system is tridiagonal and is solved in O(n)
     * operations with the Thomas algorithm (shared with CubicSpline).
     * The error is ~ h^2.
     *
     * The system is guaranteed to be solvable when q(x) >= 0 on [a, b] and
     * h |p(x)| / 2 < 1 (the matrix is then diagonally dominant).
     *
     * The class is stateless.
     *
     * @code
     * numcomp::FiniteDifferenceSolver fd;
     * // y'' = -pi^2 sin(pi x), y(0) = y(1) = 0; the exact solution is y = sin(pi x)
     * const double pi = std::acos(-1.0);
     * auto grid = fd.poisson([=](double x) { return -pi * pi * std::sin(pi * x); }, 0.0, 1.0, 0.0, 0.0, 100);
     * // grid[50] is the point x = 0.5, where y is close to 1
     * @endcode
     */
    class FiniteDifferenceSolver
    {
    public:
        /// Creates a solver. The class holds no state, so nothing needs to be set up.
        FiniteDifferenceSolver() = default;

        /**
         * @brief Solves the linear boundary value problem y'' = p(x) y' + q(x) y + r(x).
         *
         * @details Multiplying the difference equation at x_i by -h^2 gives
         * -(1 + h p_i / 2) y_(i-1) + (2 + h^2 q_i) y_i - (1 - h p_i / 2) y_(i+1) = -h^2 r_i,
         * for i = 1 .. n-1. The known boundary values y_0 = alpha and y_n = beta are
         * moved to the right-hand side of the first and last equations.
         *
         * @param p      Coefficient of y'.
         * @param q      Coefficient of y.
         * @param r      Source term.
         * @param a      Left end of the interval.
         * @param b      Right end of the interval. Must be greater than a.
         * @param alpha  Boundary value y(a).
         * @param beta   Boundary value y(b).
         * @param n      Number of grid intervals, 2 to 1,000,000.
         * @return The n + 1 grid points as (x_i, y_i) pairs, from (a, alpha) to (b, beta).
         *
         * @throws std::invalid_argument If n < 2, n > 1,000,000, !(a < b), or a, b,
         *         alpha or beta is not a finite number.
         * @throws std::runtime_error If the tridiagonal system is singular, or p, q or r
         *         give values that are not finite numbers.
         */
        std::vector<std::pair<double, double>> linear_bvp(std::function<double(double)> p, std::function<double(double)> q,
                                                          std::function<double(double)> r, double a, double b,
                                                          double alpha, double beta, int n);

        /**
         * @brief Solves the one-dimensional Poisson problem y'' = r(x), y(a) = alpha, y(b) = beta.
         *
         * @details The special case p = q = 0 of linear_bvp(). It describes, for example,
         * the deflection of a string under a load or the steady temperature in a rod
         * with a heat source r(x).
         *
         * @param r      Source term (the required second derivative).
         * @param a      Left end of the interval.
         * @param b      Right end of the interval. Must be greater than a.
         * @param alpha  Boundary value y(a).
         * @param beta   Boundary value y(b).
         * @param n      Number of grid intervals, 2 to 1,000,000.
         * @return The n + 1 grid points as (x_i, y_i) pairs.
         *
         * @throws std::invalid_argument For the same reasons as linear_bvp().
         * @throws std::runtime_error If r gives a value that is not a finite number.
         */
        std::vector<std::pair<double, double>> poisson(std::function<double(double)> r, double a, double b,
                                                       double alpha, double beta, int n);
    };
}
