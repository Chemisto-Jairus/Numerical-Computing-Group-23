#pragma once
#include <vector>

namespace numcomp
{
    /**
     * @brief Simple interpolation tools: linear interpolation and Chebyshev nodes.
     *
     * The class is stateless (it stores no data). Create one object and
     * call its methods as many times as you like.
     */
    class Interpolator
    {
    public:
        Interpolator() = default;

        /**
         * @brief Estimates y at a point x by drawing a straight line between
         *        the two data points on either side of x.
         *
         * @param x       The point where we want the estimate.
         * @param x_vals  Known x values. Must be strictly increasing (no NaN).
         * @param y_vals  Known y values. Same size as x_vals.
         * @return The estimated y value at x.
         *
         * @throws std::invalid_argument If the vectors have different sizes,
         *         fewer than 2 points, or x_vals is not strictly increasing.
         * @throws std::out_of_range If x is outside [x_vals.front(), x_vals.back()].
         *         The method does not extrapolate.
         */
        double linear_interpolate(double x, const std::vector<double>& x_vals, const std::vector<double>& y_vals);

        /**
         * @brief Makes n Chebyshev nodes on the interval [a, b].
         *
         * Chebyshev nodes bunch up near the ends of the interval. Using them as
         * sample points reduces the wiggles (Runge's phenomenon) of polynomial
         * interpolation.
         *
         * Formula: x_k = (a+b)/2 + (b-a)/2 * cos((2k-1)*pi / (2n)),  k = 1..n
         *
         * @param a  Left end of the interval.
         * @param b  Right end of the interval. Must be greater than a.
         * @param n  Number of nodes. Must be 1 to 1,000,000.
         * @return A vector of n nodes (largest first, because cos decreases).
         *
         * @throws std::invalid_argument If n <= 0, n > 1,000,000, or a >= b (or a, b is NaN).
         */
        std::vector<double> chebyshev_nodes(double a, double b, int n);
    };
}
