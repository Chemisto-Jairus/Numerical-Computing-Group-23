/**
 * @file matrix_solver.cpp
 * @brief Implementation of numcomp::MatrixSolver (Gaussian elimination and LU decomposition).
 * @author Batte Derrick Calvin (Member 2)
 */

#include "numerical-computing/matrix_solver.hpp"
#include "internal/input_checks.hpp"
#include <cmath>
#include <stdexcept>
#include <utility>

namespace numcomp
{
    /**
     * @brief Finds the partial-pivoting row for column k.
     *
     * Searches rows k..n-1 for the largest absolute value in column k.
     * Shared by both solvers; static, so it is private to this file.
     *
     * @param M  Matrix being reduced.
     * @param k  Current column, which is also the first candidate row.
     * @return Index of the pivot row.
     * @throws std::runtime_error If the largest candidate is below 1e-12 (singular matrix).
     */
    static int find_pivot_row(const std::vector<std::vector<double>>& M, int k)
    {
        int n = static_cast<int>(M.size());
        int pivot_row = k;
        for (int i = k + 1; i < n; ++i)
        {
            if (std::abs(M[i][k]) > std::abs(M[pivot_row][k]))
            {
                pivot_row = i;
            }
        }

        if (std::abs(M[pivot_row][k]) < 1e-12)
        {
            throw std::runtime_error("Matrix is singular (no unique solution).");
        }
        return pivot_row;
    }

    /**
     * @brief Solves U x = y for an upper triangular U, from the last row upwards.
     *
     * x_i = ( y_i - sum_{j > i} U_ij x_j ) / U_ii.
     * Shared by both solvers; static, so it is private to this file.
     *
     * @param U  Upper triangular matrix with non-zero diagonal.
     * @param y  Right-hand side.
     * @return The solution x.
     */
    static std::vector<double> back_substitution(const std::vector<std::vector<double>>& U, const std::vector<double>& y)
    {
        int n = static_cast<int>(U.size());
        std::vector<double> x(n, 0.0);
        for (int i = n - 1; i >= 0; --i)
        {
            double sum = 0.0;
            for (int j = i + 1; j < n; ++j)
            {
                sum += U[i][j] * x[j];
            }
            x[i] = (y[i] - sum) / U[i][i];
        }
        return x;
    }

    std::vector<double> MatrixSolver::gaussian_elimination(const std::vector<std::vector<double>>& A, const std::vector<double>& b)
    {
        checks::check_square_system(A, b);

        int n = static_cast<int>(A.size());

        // Copies: the elimination overwrites them, the caller's data stays intact.
        std::vector<std::vector<double>> M = A;
        std::vector<double> rhs = b;

        // Forward elimination, column by column.
        for (int k = 0; k < n; ++k)
        {
            int p = find_pivot_row(M, k);
            if (p != k)
            {
                std::swap(M[k], M[p]);
                std::swap(rhs[k], rhs[p]);
            }

            // Subtract a multiple of the pivot row to zero every entry below the pivot.
            for (int i = k + 1; i < n; ++i)
            {
                double factor = M[i][k] / M[k][k];
                for (int j = k; j < n; ++j)
                {
                    M[i][j] = M[i][j] - factor * M[k][j];
                }
                rhs[i] = rhs[i] - factor * rhs[k];
            }
        }

        // M is now upper triangular.
        return back_substitution(M, rhs);
    }

    std::vector<double> MatrixSolver::lu_solve(const std::vector<std::vector<double>>& A, const std::vector<double>& b)
    {
        checks::check_square_system(A, b);

        int n = static_cast<int>(A.size());

        // U starts as A and is reduced to upper triangular form.
        // L starts as the identity and collects the elimination multipliers.
        // rhs receives the same row swaps as A (that is, it becomes P b).
        std::vector<std::vector<double>> U = A;
        std::vector<std::vector<double>> L(n, std::vector<double>(n, 0.0));
        for (int i = 0; i < n; ++i)
        {
            L[i][i] = 1.0;
        }
        std::vector<double> rhs = b;

        // Factorisation P A = L U.
        for (int k = 0; k < n; ++k)
        {
            int p = find_pivot_row(U, k);
            if (p != k)
            {
                std::swap(U[k], U[p]);
                std::swap(rhs[k], rhs[p]);

                // Only the multipliers already stored (columns 0..k-1) move with the row;
                // the unit diagonal of L stays in place.
                for (int j = 0; j < k; ++j)
                {
                    std::swap(L[k][j], L[p][j]);
                }
            }

            for (int i = k + 1; i < n; ++i)
            {
                L[i][k] = U[i][k] / U[k][k];
                for (int j = k; j < n; ++j)
                {
                    U[i][j] = U[i][j] - L[i][k] * U[k][j];
                }
            }
        }

        // Forward substitution L y = P b. L has a unit diagonal, so no division is needed.
        std::vector<double> y(n, 0.0);
        for (int i = 0; i < n; ++i)
        {
            double sum = 0.0;
            for (int j = 0; j < i; ++j)
            {
                sum += L[i][j] * y[j];
            }
            y[i] = rhs[i] - sum;
        }

        // Back substitution U x = y.
        return back_substitution(U, y);
    }
}
