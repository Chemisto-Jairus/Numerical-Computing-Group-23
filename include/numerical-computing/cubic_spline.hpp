#pragma once
/**
 * @file cubic_spline.hpp
 * @brief Natural cubic spline interpolation.
 * @author Batte Derrick Calvin (Member 2)
 */

#include <vector>

namespace numcomp
{
    /**
     * @brief Smooth interpolation with a natural cubic spline.
     *
     * A separate cubic polynomial S_i(x) is used on every interval [x_i, x_(i+1)].
     * Neighbouring pieces agree in value, slope and curvature at every inner point,
     * so the whole curve is twice continuously differentiable.
     * "Natural" means the second derivative is zero at both end points.
     *
     * The only unknowns are the second derivatives M_i at the data points.
     * They satisfy a tridiagonal linear system, solved in O(n) with the Thomas algorithm
     * (the shared internal helper linalg::solve_tridiagonal, also used by FiniteDifferenceSolver).
     *
     * The class is stateless: every call recomputes the spline from the data it is given.
     *
     * @code
     * numcomp::CubicSpline spline;
     * std::vector<double> x = {0, 1, 2}, y = {0, 1, 0};
     * double v = spline.interpolate(0.5, x, y);   // 0.6875
     * @endcode
     *
     * @see Interpolator::linear_interpolate for the piecewise linear version.
     */
    class CubicSpline
    {
    public:
        /// Creates a spline object. The class holds no state, so nothing needs to be set up.
        CubicSpline() = default;

        /**
         * @brief Computes the spline's second derivative M_i at every data point.
         *
         * @details With h_i = x_(i+1) - x_i, each inner point i gives one equation:
         * h_(i-1) M_(i-1) + 2 (h_(i-1) + h_i) M_i + h_i M_(i+1)
         *     = 6 [ (y_(i+1) - y_i) / h_i - (y_i - y_(i-1)) / h_(i-1) ],
         * with M_0 = M_(n-1) = 0. The system is strictly diagonally dominant,
         * so the Thomas algorithm needs no pivoting.
         *
         * @param x_vals  Known x values. Must be strictly increasing (no repeats, no NaN).
         * @param y_vals  Known y values. Same size as x_vals.
         * @return One second derivative per data point. The first and last are 0.
         *
         * @throws std::invalid_argument If the vectors differ in size, hold fewer than
         *         2 points, or x_vals is not strictly increasing.
         */
        std::vector<double> second_derivatives(const std::vector<double>& x_vals, const std::vector<double>& y_vals);

        /**
         * @brief Evaluates the natural cubic spline through the data at x.
         *
         * @details On [x_i, x_(i+1)], with t0 = x - x_i and t1 = x_(i+1) - x:
         * S(x) = M_i t1^3 / (6h) + M_(i+1) t0^3 / (6h)
         *      + (y_i / h - M_i h / 6) t1 + (y_(i+1) / h - M_(i+1) h / 6) t0.
         *
         * @param x       The point where the estimate is wanted.
         * @param x_vals  Known x values. Must be strictly increasing.
         * @param y_vals  Known y values. Same size as x_vals.
         * @return The spline value at x.
         *
         * @throws std::invalid_argument Same cases as second_derivatives().
         * @throws std::out_of_range If x is outside [x_vals.front(), x_vals.back()].
         *         The spline never extrapolates.
         */
        double interpolate(double x, const std::vector<double>& x_vals, const std::vector<double>& y_vals);
    };
}
