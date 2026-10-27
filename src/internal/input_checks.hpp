#pragma once
/**
 * @file input_checks.hpp
 * @brief Internal input-validation helpers shared by all library modules.
 * @author Chemisto Jairus (Member 1)
 *
 * This header lives in src/internal/, not in include/, so it is not part of
 * the public API. Library source files include it as "internal/input_checks.hpp".
 * Each check is defined once here and reused by every method that needs it,
 * so the rules and the error messages are identical across the library.
 */

#include <cstddef>
#include <vector>

namespace numcomp
{
    /**
     * @namespace numcomp::checks
     * @brief Shared argument checks. Every function either returns normally
     *        or throws the exception documented for it.
     */
    namespace checks
    {
        /// Largest number of ODE steps or Chebyshev nodes accepted (keeps memory use bounded).
        constexpr int MAX_POINTS = 1000000;

        /**
         * @brief Validates the stopping settings of an iterative root finder.
         *
         * Used by bisection, newton_raphson, secant_method, fixed_point_iteration,
         * the conjugate gradient solver and the Runge-Kutta-Fehlberg solver.
         * The test is written as !(tol > 0) so that NaN is rejected as well.
         *
         * @param tol       Convergence tolerance. Must be > 0.
         * @param max_iter  Iteration limit. Must be > 0.
         * @throws std::invalid_argument If tol <= 0, tol is NaN, or max_iter <= 0.
         */
        void check_tolerance(double tol, int max_iter);

        /**
         * @brief Validates that both integration limits are finite numbers.
         *
         * Used by all four integration methods.
         *
         * @param a  Lower limit.
         * @param b  Upper limit.
         * @throws std::invalid_argument If a or b is NaN or infinite.
         */
        void check_finite_limits(double a, double b);

        /**
         * @brief Validates the inputs of a composite rule that uses n strips.
         *
         * Used by trapezoidal_rule, midpoint_rule, simpsons_rule and the
         * finite difference boundary value solver (where n is the number of grid intervals).
         *
         * @param a  Lower limit.
         * @param b  Upper limit.
         * @param n  Number of strips. Must be > 0.
         * @throws std::invalid_argument If n <= 0, or a or b is NaN or infinite.
         */
        void check_integration_inputs(double a, double b, int n);

        /**
         * @brief Validates a set of interpolation data points.
         *
         * Used by Interpolator::linear_interpolate and CubicSpline.
         *
         * @param x_vals  Known x values. Must be strictly increasing (no repeats, no NaN).
         * @param y_vals  Known y values. Same size as x_vals.
         * @throws std::invalid_argument If the sizes differ, there are fewer than 2 points,
         *         or x_vals is not strictly increasing.
         */
        void check_data_points(const std::vector<double>& x_vals, const std::vector<double>& y_vals);

        /**
         * @brief Finds the interval of the data that contains x.
         *
         * Used by Interpolator::linear_interpolate and CubicSpline::interpolate.
         * Assumes check_data_points() has already succeeded. The search is a
         * binary search, so it needs about log2(n) comparisons for n data points.
         *
         * @param x       Query point.
         * @param x_vals  Strictly increasing x values.
         * @return The index i with x_vals[i] <= x <= x_vals[i + 1].
         * @throws std::out_of_range If x is outside [x_vals.front(), x_vals.back()].
         */
        std::size_t find_interval(double x, const std::vector<double>& x_vals);

        /**
         * @brief Validates the time interval and step size of a fixed-step ODE solver.
         *
         * Used by ode::run_fixed_steps (euler, heun, rk3, rk4), ode::run_multistep
         * (the Adams-Bashforth and Adams-Moulton methods) and the Runge-Kutta-Fehlberg solver.
         *
         * @param t0     Start time.
         * @param t_end  End time. Must be greater than t0.
         * @param h      Step size. Must be > 0.
         * @throws std::invalid_argument If h <= 0, t_end <= t0 (either may be NaN),
         *         or (t_end - t0) / h exceeds MAX_POINTS.
         */
        void check_ode_inputs(double t0, double t_end, double h);

        /**
         * @brief Validates the shape of a linear system A x = b.
         *
         * Used by MatrixSolver::gaussian_elimination, MatrixSolver::lu_solve and
         * ConjugateGradientSolver.
         *
         * @param A  Coefficient matrix, stored as a vector of rows.
         * @param b  Right-hand side.
         * @throws std::invalid_argument If A is empty, any row length differs from the
         *         number of rows, or b.size() differs from the number of rows.
         */
        void check_square_system(const std::vector<std::vector<double>>& A, const std::vector<double>& b);

        /**
         * @brief Validates the point and step size of a finite difference formula.
         *
         * Used by every method of NumericalDifferentiator.
         *
         * @param x  Point where the derivative is wanted. Must be finite.
         * @param h  Step size. Must be > 0 and finite.
         * @throws std::invalid_argument If x is NaN or infinite, or h is not a positive finite number.
         */
        void check_difference_inputs(double x, double h);

        /**
         * @brief Confirms that a computed value is a finite number.
         *
         * Used after a calculation whose result depends on the user's function:
         * numerical derivatives, the adaptive and multistep ODE solvers and the
         * finite difference boundary value solver. A NaN or infinite value means
         * that f overflowed or was evaluated at a singularity.
         *
         * @param value  The value to check.
         * @throws std::runtime_error If value is NaN or infinite.
         */
        void check_finite_result(double value);
    }
}
