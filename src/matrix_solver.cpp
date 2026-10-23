#include "numerical-computing/matrix_solver.hpp"
#include <cmath>
#include <stdexcept>
#include <utility>   

namespace numcomp
{
    /// Gaussian elimination. See matrix_solver.hpp.
    std::vector<double> MatrixSolver::gaussian_elimination(const std::vector<std::vector<double>>& A, const std::vector<double>& b)
    {
        // Check 1: the matrix must have at least one row
        if (A.empty())
        {
            throw std::invalid_argument("Matrix A must not be empty.");
        }

        // Check 2: every row must have n numbers (square matrix)
        for (size_t i = 0; i < A.size(); ++i)
        {
            if (A[i].size() != A.size())
            {
                throw std::invalid_argument("Matrix A must be square (n rows, each with n numbers).");
            }
        }

        // Check 3: b must have one number per row of A
        if (b.size() != A.size())
        {
            throw std::invalid_argument("Vector b must have the same size as matrix A.");
        }

        int n = static_cast<int>(A.size());

        // Work on copies so the caller's A and b are not changed
        std::vector<std::vector<double>> M = A;
        std::vector<double> rhs = b;

        // Step 1: forward elimination, one column at a time
        for (int k = 0; k < n; ++k)
        {
            // Find the row (from k down) with the biggest value in column k.
            // Picking the biggest value keeps rounding errors small.
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

            // Move the pivot row up to row k
            if (pivot_row != k)
            {
                std::swap(M[k], M[pivot_row]);
                std::swap(rhs[k], rhs[pivot_row]);
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

        // Step 2: back substitution, last row first
        std::vector<double> x(n, 0.0);
        for (int i = n - 1; i >= 0; --i)
        {
            double sum = 0.0;
            for (int j = i + 1; j < n; ++j)
            {
                sum += M[i][j] * x[j];
            }
            x[i] = (rhs[i] - sum) / M[i][i];
        }

        return x;
    }

    /// LU decomposition solve. See matrix_solver.hpp.
    std::vector<double> MatrixSolver::lu_solve(const std::vector<std::vector<double>>& A, const std::vector<double>& b)
    {
        // Check 1: the matrix must have at least one row
        if (A.empty())
        {
            throw std::invalid_argument("Matrix A must not be empty.");
        }

        // Check 2: every row must have n numbers (square matrix)
        for (size_t i = 0; i < A.size(); ++i)
        {
            if (A[i].size() != A.size())
            {
                throw std::invalid_argument("Matrix A must be square (n rows, each with n numbers).");
            }
        }

        // Check 3: b must have one number per row of A
        if (b.size() != A.size())
        {
            throw std::invalid_argument("Vector b must have the same size as matrix A.");
        }

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
            // Find the row (from k down) with the biggest value in column k
            int pivot_row = k;
            for (int i = k + 1; i < n; ++i)
            {
                if (std::abs(U[i][k]) > std::abs(U[pivot_row][k]))
                {
                    pivot_row = i;
                }
            }

            // If even the biggest value is (almost) zero, we cannot divide by it
            if (std::abs(U[pivot_row][k]) < 1e-12)
            {
                throw std::runtime_error("Matrix is singular (no unique solution).");
            }

            if (pivot_row != k)
            {
                // Swap the rows in U and in rhs
                std::swap(U[k], U[pivot_row]);
                std::swap(rhs[k], rhs[pivot_row]);

                // Swap only the multipliers already stored in L (columns before k).
                // The 1s on the diagonal of L stay where they are.
                for (int j = 0; j < k; ++j)
                {
                    std::swap(L[k][j], L[pivot_row][j]);
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

        // Step 3: back substitution, solve U x = y (last row first)
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
}
