#pragma once
#include <vector>

namespace numcomp
{
    /**
     * @brief Solves square linear systems A x = b.
     *
     * Two methods are given:
     *  - Gaussian elimination with partial pivoting
     *  - LU decomposition (Doolittle) with partial pivoting
     *
     * A matrix is stored as a vector of rows: A[i][j] is row i, column j.
     * The inputs are never changed; the methods work on copies.
     *
     * Meant for small, dense matrices. The class is stateless.
     */
    class MatrixSolver
    {
    public:
        MatrixSolver() = default;

        /**
         * @brief Solves A x = b with Gaussian elimination and partial pivoting.
         *
         * Step 1 turns A into an upper triangular matrix (and changes b the same way).
         * Step 2 uses back substitution to get x.
         *
         * @param A  Square coefficient matrix (n rows, each with n numbers).
         * @param b  Right-hand side, n numbers.
         * @return The solution x.
         *
         * @throws std::invalid_argument If A is empty, A is not square,
         *         or b does not have the same size as A.
         * @throws std::runtime_error If the matrix is singular (a pivot is below 1e-12).
         */
        std::vector<double> gaussian_elimination(const std::vector<std::vector<double>>& A, const std::vector<double>& b);

        /**
         * @brief Solves A x = b with LU decomposition and partial pivoting.
         *
         * Step 1 splits the (row-swapped) A into L * U.
         *        L is lower triangular with 1s on the diagonal. U is upper triangular.
         * Step 2 solves L y = b (forward substitution).
         * Step 3 solves U x = y (back substitution).
         *
         * @param A  Square coefficient matrix (n rows, each with n numbers).
         * @param b  Right-hand side, n numbers.
         * @return The solution x.
         *
         * @throws std::invalid_argument If A is empty, A is not square,
         *         or b does not have the same size as A.
         * @throws std::runtime_error If the matrix is singular (a pivot is below 1e-12).
         */
        std::vector<double> lu_solve(const std::vector<std::vector<double>>& A, const std::vector<double>& b);
    };
}
