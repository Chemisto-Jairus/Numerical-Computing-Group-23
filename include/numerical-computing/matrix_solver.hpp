#pragma once
/**
 * @file matrix_solver.hpp
 * @brief Direct solvers for square linear systems: Gaussian elimination and LU decomposition.
 * @author Batte Derrick Calvin (Member 2)
 */

#include <vector>

namespace numcomp
{
    /**
     * @brief Solves square linear systems A x = b.
     *
     * - gaussian_elimination(): reduces [A | b] to upper triangular form, then back-substitutes.
     * - lu_solve(): factorises the row-swapped A as L U (Doolittle form, 1s on the diagonal
     *   of L), then solves L y = b and U x = y.
     *
     * Both use partial pivoting: at each column the row with the largest absolute
     * entry is moved to the pivot position. This avoids dividing by zero and keeps
     * rounding errors small. Both cost O(n^3) operations and are meant for small,
     * dense systems.
     *
     * A matrix is a vector of rows: A[i][j] is row i, column j.
     * The inputs are taken by const reference and copied internally, so the
     * caller's A and b are never modified. The class is stateless.
     *
     * @code
     * numcomp::MatrixSolver solver;
     * std::vector<std::vector<double>> A = {{2, 1}, {1, 3}};
     * std::vector<double> b = {5, 10};
     * std::vector<double> x = solver.gaussian_elimination(A, b);   // {1, 3}
     * @endcode
     */
    class MatrixSolver
    {
    public:
        /// Creates a solver. The class holds no state, so nothing needs to be set up.
        MatrixSolver() = default;

        /**
         * @brief Solves A x = b with Gaussian elimination and partial pivoting.
         *
         * @param A  Square coefficient matrix (n rows of n numbers).
         * @param b  Right-hand side (n numbers).
         * @return The solution vector x (n numbers).
         *
         * @throws std::invalid_argument If A is empty, not square, or b has the wrong size.
         * @throws std::runtime_error If a pivot is smaller than 1e-12 in absolute value
         *         (the matrix is singular or nearly singular).
         */
        std::vector<double> gaussian_elimination(const std::vector<std::vector<double>>& A, const std::vector<double>& b);

        /**
         * @brief Solves A x = b with LU decomposition (Doolittle) and partial pivoting.
         *
         * @details Builds P A = L U, applies the same row swaps P to b, then solves
         * L y = P b by forward substitution and U x = y by back substitution.
         *
         * @param A  Square coefficient matrix (n rows of n numbers).
         * @param b  Right-hand side (n numbers).
         * @return The solution vector x (n numbers).
         *
         * @throws std::invalid_argument If A is empty, not square, or b has the wrong size.
         * @throws std::runtime_error If a pivot is smaller than 1e-12 in absolute value
         *         (the matrix is singular or nearly singular).
         */
        std::vector<double> lu_solve(const std::vector<std::vector<double>>& A, const std::vector<double>& b);
    };
}
