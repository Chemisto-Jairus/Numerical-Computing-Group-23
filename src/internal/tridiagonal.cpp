/**
 * @file tridiagonal.cpp
 * @brief Implementation of numcomp::linalg::solve_tridiagonal (Thomas algorithm).
 * @author Chemisto Jairus (Member 1)
 */

#include "tridiagonal.hpp"
#include <cmath>
#include <stdexcept>

namespace numcomp
{
    namespace linalg
    {
        std::vector<double> solve_tridiagonal(std::vector<double> lower, std::vector<double> diag,
                                              std::vector<double> upper, std::vector<double> rhs)
        {
            std::size_t m = diag.size();
            if (m == 0 || lower.size() != m || upper.size() != m || rhs.size() != m)
            {
                throw std::invalid_argument("Tridiagonal system: all four vectors must have the same, non-zero size.");
            }

            // Forward sweep: subtract a multiple of row k-1 from row k to remove lower[k].
            for (std::size_t k = 1; k < m; ++k)
            {
                if (std::abs(diag[k-1]) < 1e-14)
                {
                    throw std::runtime_error("Tridiagonal system is singular (zero pivot).");
                }
                double factor = lower[k] / diag[k-1];
                diag[k] = diag[k] - factor * upper[k-1];
                rhs[k]  = rhs[k]  - factor * rhs[k-1];
            }
            if (std::abs(diag[m-1]) < 1e-14)
            {
                throw std::runtime_error("Tridiagonal system is singular (zero pivot).");
            }

            // Back substitution from the last row upwards.
            std::vector<double> x(m);
            x[m-1] = rhs[m-1] / diag[m-1];
            for (std::size_t k = m - 1; k-- > 0; )
            {
                x[k] = (rhs[k] - upper[k] * x[k+1]) / diag[k];
            }
            return x;
        }
    }
}
