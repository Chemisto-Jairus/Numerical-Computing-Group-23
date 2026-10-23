#pragma once
#include <cstddef>
#include <vector>

// =====================================================================
// INTERNAL HELPERS: input checks shared by several modules.
//
// This header lives in src/internal/, NOT in include/, so it is not part
// of the public library API. Users of numcomp2 never include it.
// The library's own .cpp files include it with:
//     #include "internal/input_checks.hpp"
//
// Before: every method wrote these checks again (the same if-statements
// and messages were copied up to 4 times). Now each check is written once.
// =====================================================================

namespace numcomp
{
    namespace checks
    {
        /// Largest number of ODE steps or Chebyshev nodes we allow.
        /// More than this would need millions of points of memory.
        constexpr int MAX_POINTS = 1000000;

        /**
         * @brief Root finders: tol must be > 0 and max_iter must be > 0.
         *
         * Used by bisection, newton_raphson, secant_method and fixed_point_iteration.
         * "!(tol > 0)" also catches NaN, which "tol <= 0" lets through.
         *
         * @throws std::invalid_argument If tol <= 0, tol is NaN, or max_iter <= 0.
         */
        void check_tolerance(double tol, int max_iter);

        /**
         * @brief Integrators: limits a and b must be finite numbers (not NaN or infinity).
         *
         * Used by all four integration methods.
         *
         * @throws std::invalid_argument If a or b is NaN or infinite.
         */
        void check_finite_limits(double a, double b);

        /**
         * @brief Integrators that split [a, b] into n strips: n > 0 and finite limits.
         *
         * Used by trapezoidal_rule, midpoint_rule and simpsons_rule.
         *
         * @throws std::invalid_argument If n <= 0, or a or b is NaN or infinite.
         */
        void check_integration_inputs(double a, double b, int n);

        /**
         * @brief Interpolation data: same sizes, at least 2 points, x strictly increasing.
         *
         * Used by Interpolator::linear_interpolate and CubicSpline.
         *
         * @throws std::invalid_argument If any of those rules is broken (NaN in x counts too).
         */
        void check_data_points(const std::vector<double>& x_vals, const std::vector<double>& y_vals);

        /**
         * @brief Interpolation: finds i so that x is inside [x_vals[i], x_vals[i+1]].
         *
         * Used by Interpolator::linear_interpolate and CubicSpline::interpolate.
         * Assumes check_data_points() has already passed.
         *
         * @return The left index i of the interval.
         * @throws std::out_of_range If x is outside the data (no extrapolation).
         */
        std::size_t find_interval(double x, const std::vector<double>& x_vals);

        /**
         * @brief ODE solvers: h > 0, t_end > t0, and at most MAX_POINTS steps.
         *
         * Used by euler, heun, rk3 and rk4 (through run_fixed_steps).
         *
         * @throws std::invalid_argument If h <= 0, t_end <= t0 (or NaN), or too many steps.
         */
        void check_ode_inputs(double t0, double t_end, double h);

        /**
         * @brief Matrix solver: A is non-empty and square, b has one number per row.
         *
         * Used by gaussian_elimination and lu_solve.
         *
         * @throws std::invalid_argument If A is empty, not square, or b is the wrong size.
         */
        void check_square_system(const std::vector<std::vector<double>>& A, const std::vector<double>& b);
    }
}
