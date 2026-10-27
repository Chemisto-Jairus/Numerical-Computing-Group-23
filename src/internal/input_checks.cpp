/**
 * @file input_checks.cpp
 * @brief Implementation of the shared validation helpers in numcomp::checks.
 * @author Chemisto Jairus (Member 1)
 */

#include "input_checks.hpp"
#include <algorithm>
#include <cmath>
#include <stdexcept>

namespace numcomp
{
    namespace checks
    {
        void check_tolerance(double tol, int max_iter)
        {
            if (!(tol > 0) || max_iter <= 0)
            {
                throw std::invalid_argument("tol and max_iter must be positive.");
            }
        }

        void check_finite_limits(double a, double b)
        {
            if (!std::isfinite(a) || !std::isfinite(b))
            {
                throw std::invalid_argument("Limits a and b must be finite numbers.");
            }
        }

        void check_integration_inputs(double a, double b, int n)
        {
            if (n <= 0)
            {
                throw std::invalid_argument("Number of intervals must be strictly positive.");
            }
            check_finite_limits(a, b);
        }

        void check_data_points(const std::vector<double>& x_vals, const std::vector<double>& y_vals)
        {
            if (x_vals.size() != y_vals.size() || x_vals.size() < 2)
            {
                throw std::invalid_argument("Input vectors must have the same size and at least 2 points.");
            }

            // !(x[i] < x[i+1]) catches equal values, decreasing values and NaN.
            for (std::size_t i = 0; i < x_vals.size() - 1; ++i)
            {
                if (!(x_vals[i] < x_vals[i+1]))
                {
                    throw std::invalid_argument("x values must be strictly increasing.");
                }
            }
        }

        std::size_t find_interval(double x, const std::vector<double>& x_vals)
        {
            // Written as !(...) so that a NaN x is rejected as well.
            if (!(x >= x_vals.front() && x <= x_vals.back()))
            {
                throw std::out_of_range("Target x is out of the interpolation bounds.");
            }

            // Binary search: upper_bound returns the first x value that is greater than x.
            // The interval starts one position before it. x equal to the last data point
            // belongs to the last interval.
            std::size_t i = std::upper_bound(x_vals.begin(), x_vals.end(), x) - x_vals.begin();
            if (i >= x_vals.size())
            {
                i = x_vals.size() - 1;
            }
            return i - 1;
        }

        void check_ode_inputs(double t0, double t_end, double h)
        {
            if (!(h > 0) || !(t_end > t0))
            {
                throw std::invalid_argument("Step size h must be strictly positive and t_end must be greater than t0.");
            }

            // Each step stores one (t, y) pair, so the step count bounds the memory used.
            if ((t_end - t0) / h > MAX_POINTS)
            {
                throw std::invalid_argument("Too many steps: please use a bigger step size h.");
            }
        }

        void check_square_system(const std::vector<std::vector<double>>& A, const std::vector<double>& b)
        {
            if (A.empty())
            {
                throw std::invalid_argument("Matrix A must not be empty.");
            }
            for (std::size_t i = 0; i < A.size(); ++i)
            {
                if (A[i].size() != A.size())
                {
                    throw std::invalid_argument("Matrix A must be square (n rows, each with n numbers).");
                }
            }
            if (b.size() != A.size())
            {
                throw std::invalid_argument("Vector b must have the same size as matrix A.");
            }
        }

        void check_difference_inputs(double x, double h)
        {
            if (!std::isfinite(x))
            {
                throw std::invalid_argument("The point x must be a finite number.");
            }
            if (!(h > 0) || !std::isfinite(h))
            {
                throw std::invalid_argument("Step size h must be a positive finite number.");
            }
        }

        void check_finite_result(double value)
        {
            if (!std::isfinite(value))
            {
                throw std::runtime_error("Result is not finite: f overflowed or hit a singularity.");
            }
        }
    }
}
