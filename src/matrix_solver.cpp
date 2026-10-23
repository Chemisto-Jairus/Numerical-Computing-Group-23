#include "numerical-computing/matrix_solver.hpp"
#include "internal/input_checks.hpp"
#include <cmath>
#include <stdexcept>
#include <utility>   // std::swap

namespace numcomp
{
    // -----------------------------------------------------------------
    // Helpers used by BOTH solvers. "static" means they are private to
    // this file: they are not part of the public MatrixSolver API.
    // Before, each solver had its own copy of this code.
    // -----------------------------------------------------------------

    /// Finds the row (from k down) with the biggest absolute value in column k.
    /// Picking the biggest value keeps rounding errors small.
    /// @throws std::runtime_error If every candidate is (almost) zero: the matrix is singular.
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

        // If even the biggest value is (almost) zero, we cannot divide by it
        if (std::abs(M[pivot_row][k]) < 1e-12)
        {
            throw std::runtime_error("Matrix is singular (no unique solution).");
        }
        return pivot_row;
    }

    /// Solves U x = y when U is upper triangular, last row first.
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

    /// Gaussian elimination. See matrix_solver.hpp.
    std::vector<double> MatrixSolver::gaussian_elimination(const std::vector<std::vector<double>>& A, const std::vector<double>& b)
    {
        checks::check_square_system(A, b);   // shared helper: non-empty, square, b fits

        int n = static_cast<int>(A.size());

        // Work on copies so the caller's A and b are not changed
        std::vector<std::vector<double>> M = A;
        std::vector<double> rhs = b;

        // Step 1: forward elimination, one column at a time
        for (int k = 0; k < n; ++k)
        {
            // Move the best pivot row up to row k
            int p = find_pivot_row(M, k);
            if (p != k)
            {
                std::swap(M[k], M[p]);
                std::swap(rhs[k], rhs[p]);
            }

            // Make every entry below the pivot zero
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

        // Step 2: M is now upper triangular, so solve from the bottom up
        return back_substitution(M, rhs);
    }

    /// LU decomposition solve. See matrix_solver.hpp.
    std::vector<double> MatrixSolver::lu_solve(const std::vector<std::vector<double>>& A, const std::vector<double>& b)
    {
        checks::check_square_system(A, b);   // shared helper

        int n = static_cast<int>(A.size());

        // U starts as a copy of A and becomes upper triangular.
        // L starts as the identity matrix and collects the multipliers.
        // rhs is b, with the same row swaps applied to it.
        std::vector<std::vector<double>> U = A;
        std::vector<std::vector<double>> L(n, std::vector<double>(n, 0.0));
        for (int i = 0; i < n; ++i)
        {
            L[i][i] = 1.0;
        }
        std::vector<double> rhs = b;

        // Step 1: build L and U, one column at a time
        for (int k = 0; k < n; ++k)
        {
            int p = find_pivot_row(U, k);
            if (p != k)
            {
                // Swap the rows in U and in rhs
                std::swap(U[k], U[p]);
                std::swap(rhs[k], rhs[p]);

                // Swap only the multipliers already stored in L (columns before k).
                // The 1s on the diagonal of L stay where they are.
                for (int j = 0; j < k; ++j)
                {
                    std::swap(L[k][j], L[p][j]);
                }
            }

            // Eliminate below the pivot and store each multiplier in L
            for (int i = k + 1; i < n; ++i)
            {
                L[i][k] = U[i][k] / U[k][k];
                for (int j = k; j < n; ++j)
                {
                    U[i][j] = U[i][j] - L[i][k] * U[k][j];
                }
            }
        }

        // Step 2: forward substitution, solve L y = rhs (top row first).
        // L has 1s on its diagonal, so there is no division.
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

        // Step 3: back substitution, solve U x = y (shared helper)
        return back_substitution(U, y);
    }
}
