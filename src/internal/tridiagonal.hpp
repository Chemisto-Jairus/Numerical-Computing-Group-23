#pragma once
/**
 * @file tridiagonal.hpp
 * @brief Internal solver for tridiagonal linear systems (the Thomas algorithm).
 * @author Chemisto Jairus (Member 1)
 *
 * Two modules build a tridiagonal system and need to solve it:
 * CubicSpline (for the spline's second derivatives) and FiniteDifferenceSolver
 * (for the values of y at the grid points). The algorithm is written once here.
 * Not part of the public API (lives in src/internal/).
 */

#include <vector>

namespace numcomp
{
    /**
     * @namespace numcomp::linalg
     * @brief Shared linear algebra routines used inside the library.
     */
    namespace linalg
    {
        /**
         * @brief Solves a tridiagonal system with the Thomas algorithm in O(m) operations.
         *
         * @details Row k of the system is
         * lower[k] x[k-1] + diag[k] x[k] + upper[k] x[k+1] = rhs[k],
         * where lower[0] and upper[m-1] are ignored. The forward sweep removes the
         * sub-diagonal; back substitution then finds x from the last row upwards.
         * No pivoting is done, which is safe for the diagonally dominant systems
         * built by CubicSpline and FiniteDifferenceSolver.
         *
         * The four vectors are taken by value because the sweep overwrites them.
         *
         * @param lower  Sub-diagonal (size m).
         * @param diag   Main diagonal (size m).
         * @param upper  Super-diagonal (size m).
         * @param rhs    Right-hand side (size m).
         * @return The solution x (size m).
         * @throws std::invalid_argument If the vectors are empty or differ in size.
         * @throws std::runtime_error If a pivot becomes (almost) zero, so the sweep cannot continue.
         */
        std::vector<double> solve_tridiagonal(std::vector<double> lower, std::vector<double> diag,
                                              std::vector<double> upper, std::vector<double> rhs);
    }
}
