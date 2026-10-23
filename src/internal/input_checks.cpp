#include "input_checks.hpp"
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
            // Rule 1: same number of x and y values, and at least 2 points
            if (x_vals.size() != y_vals.size() || x_vals.size() < 2)
            {
                throw std::invalid_argument("Input vectors must have the same size and at least 2 points.");
            }

            // Rule 2: x strictly increasing. "!(x[i] < x[i+1])" also catches NaN.
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
            for (std::size_t i = 0; i < x_vals.size() - 1; ++i)
            {
                if (x >= x_vals[i] && x <= x_vals[i+1])
                {
                    return i;
                }
            }
            throw std::out_of_range("Target x is out of the interpolation bounds.");
        }

        void check_ode_inputs(double t0, double t_end, double h)
        {
            // "!(h > 0)" also catches NaN, which "h <= 0" lets through
            if (!(h > 0) || !(t_end > t0))
            {
                throw std::invalid_argument("Step size h must be strictly positive and t_end must be greater than t0.");
            }

            // Stop absurdly small steps: they would need millions of points of memory
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
    }
}
