#pragma once
/**
 * @file interpolator.hpp
 * @brief Piecewise linear interpolation, Chebyshev nodes and Chebyshev approximation.
 * @author Batte Derrick Calvin (Member 2)
 */

#include <functional>
#include <vector>

namespace numcomp
{
    /**
     * @brief Estimates values between known data points, and chooses good sample points.
     *
     * - linear_interpolate(): joins neighbouring data points with straight lines.
     * - chebyshev_nodes(): returns sample points that cluster near the ends of an
     *   interval, which keeps polynomial interpolation from oscillating (Runge's phenomenon).
     * - chebyshev_coefficients() and chebyshev_approximate(): Chebyshev approximation.
     *   A function is sampled at the Chebyshev nodes and replaced by a short sum
     *   c_0/2 + c_1 T_1(u) + ... + c_(n-1) T_(n-1)(u) of Chebyshev polynomials,
     *   which is close to the best possible polynomial of that degree.
     *
     * The class is stateless. The data is passed by const reference, so the
     * caller's vectors are never copied or changed.
     *
     * @code
     * numcomp::Interpolator interp;
     * std::vector<double> x = {0, 1, 2}, y = {0, 1, 4};
     * double v = interp.linear_interpolate(1.5, x, y);              // 2.5
     * std::vector<double> nodes = interp.chebyshev_nodes(-1, 1, 3); // 0.866, 0, -0.866
     *
     * std::vector<double> c = interp.chebyshev_coefficients([](double t) { return std::exp(t); }, 0, 1, 10);
     * double e = interp.chebyshev_approximate(c, 0, 1, 0.5);       // e^0.5 to about 1e-13
     * @endcode
     *
     * @see CubicSpline for a smooth (twice differentiable) interpolant through the same data.
     */
    class Interpolator
    {
    public:
        /// Creates an interpolator. The class holds no state, so nothing needs to be set up.
        Interpolator() = default;

        /**
         * @brief Estimates y at x using the straight line through the two neighbouring data points.
         *
         * @details If x lies in [x_i, x_(i+1)], the result is
         * y_i + (y_(i+1) - y_i) (x - x_i) / (x_(i+1) - x_i).
         * The data points themselves are reproduced exactly.
         *
         * @param x       The point where the estimate is wanted.
         * @param x_vals  Known x values. Must be strictly increasing (no repeats, no NaN).
         * @param y_vals  Known y values. Same size as x_vals.
         * @return The interpolated value at x.
         *
         * @throws std::invalid_argument If the vectors differ in size, hold fewer than
         *         2 points, or x_vals is not strictly increasing.
         * @throws std::out_of_range If x is outside [x_vals.front(), x_vals.back()].
         *         The method never extrapolates.
         */
        double linear_interpolate(double x, const std::vector<double>& x_vals, const std::vector<double>& y_vals);

        /**
         * @brief Generates n Chebyshev nodes on [a, b].
         *
         * @details x_k = (a + b)/2 + (b - a)/2 cos((2k - 1) pi / (2n)), k = 1..n.
         * The nodes are the roots of the Chebyshev polynomial T_n mapped onto [a, b].
         *
         * @param a  Left end of the interval.
         * @param b  Right end of the interval. Must be greater than a.
         * @param n  Number of nodes, 1 to 1,000,000.
         * @return n nodes in decreasing order (largest first).
         *
         * @throws std::invalid_argument If n <= 0, n > 1,000,000, or !(a < b) (this includes NaN).
         */
        std::vector<double> chebyshev_nodes(double a, double b, int n);

        /**
         * @brief Computes the coefficients of the degree n-1 Chebyshev approximation of f on [a, b].
         *
         * @details f is sampled at the n Chebyshev nodes x_k = chebyshev_nodes(a, b, n)[k-1],
         * which correspond to the angles theta_k = (2k - 1) pi / (2n), k = 1..n. Then
         * c_j = (2/n) sum_k f(x_k) cos(j theta_k), j = 0..n-1.
         * The approximation p(x) = c_0/2 + sum_(j>=1) c_j T_j(u), with u = (2x - a - b)/(b - a),
         * interpolates f at the nodes. For smooth f the coefficients shrink quickly, so
         * a few terms already give a very accurate approximation.
         *
         * @param f  The function to approximate.
         * @param a  Left end of the interval.
         * @param b  Right end of the interval. Must be greater than a.
         * @param n  Number of coefficients (degree n - 1), 1 to 1,000.
         * @return The coefficients c_0 .. c_(n-1).
         *
         * @throws std::invalid_argument If n <= 0, n > 1,000, or !(a < b) (this includes NaN).
         * @throws std::runtime_error If f returns a value that is not a finite number.
         */
        std::vector<double> chebyshev_coefficients(std::function<double(double)> f, double a, double b, int n);

        /**
         * @brief Evaluates a Chebyshev approximation at x with Clenshaw's recurrence.
         *
         * @details With u = (2x - a - b)/(b - a), Clenshaw's recurrence
         * b_j = 2u b_(j+1) - b_(j+2) + c_j (j = n-1 down to 1) gives
         * p(x) = u b_1 - b_2 + c_0/2 in O(n) operations without forming any T_j.
         *
         * @param coeffs  Coefficients from chebyshev_coefficients(). Must not be empty.
         * @param a       Left end of the interval the coefficients were computed on.
         * @param b       Right end of that interval. Must be greater than a.
         * @param x       Point of evaluation, inside [a, b].
         * @return The value of the approximation at x.
         *
         * @throws std::invalid_argument If coeffs is empty or !(a < b).
         * @throws std::out_of_range If x is outside [a, b]. The approximation never extrapolates.
         */
        double chebyshev_approximate(const std::vector<double>& coeffs, double a, double b, double x);
    };
}
