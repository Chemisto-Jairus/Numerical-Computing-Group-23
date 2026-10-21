#pragma once
#include <vector>

namespace numcomp
{
    /**
     * @brief Natural cubic spline interpolation.
     *
     * A cubic spline joins the data points with a separate cubic curve on each
     * interval. The curves meet smoothly: at every inner point the value, the
     * slope (1st derivative) and the bending (2nd derivative) all match.
     *
     * "Natural" means the 2nd derivative is zero at the two end points.
     *
     * The class is stateless (it stores no data). Each call works the
     * spline out again from the data you pass in.
     */
    class CubicSpline
    {
    public:
        CubicSpline() = default;

        /**
         * @brief Works out the 2nd derivative M_i of the spline at every data point.
         *
         * These M values are all we need to build every cubic piece.
         * M_0 and M_(n-1) are 0 (natural spline). The inner values come from
         * a tridiagonal system, solved with the Thomas algorithm.
         *
         * @param x_vals  Known x values. Must be strictly increasing (no NaN).
         * @param y_vals  Known y values. Same size as x_vals.
         * @return A vector M with one 2nd derivative per data point.
         *
         * @throws std::invalid_argument If the vectors have different sizes,
         *         fewer than 2 points, or x_vals is not strictly increasing.
         */
        std::vector<double> second_derivatives(const std::vector<double>& x_vals, const std::vector<double>& y_vals);

        /**
         * @brief Estimates y at a point x using the natural cubic spline.
         *
         * @param x       The point where we want the estimate.
         * @param x_vals  Known x values. Must be strictly increasing.
         * @param y_vals  Known y values. Same size as x_vals.
         * @return The spline value at x.
         *
         * @throws std::invalid_argument Same cases as second_derivatives().
         * @throws std::out_of_range If x is outside [x_vals.front(), x_vals.back()].
         *         The spline does not extrapolate.
         */
        double interpolate(double x, const std::vector<double>& x_vals, const std::vector<double>& y_vals);
    };
}
