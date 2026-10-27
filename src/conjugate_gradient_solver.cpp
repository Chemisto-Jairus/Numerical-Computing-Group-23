#include "numerical-computing/conjugate_gradient_solver.hpp"
#include "internal/input_checks.hpp"
#include <algorithm>
#include <cmath>
#include <stdexcept>

namespace numcomp
{
    static double dot(const std::vector<double>& u, const std::vector<double>& v)
    {
        double sum = 0.0;
        for (std::size_t i = 0; i < u.size(); ++i)
        {
            sum += u[i] * v[i];
        }
        return sum;
    }

    static std::vector<double> multiply(const std::vector<std::vector<double>>& A, const std::vector<double>& v)
    {
        std::vector<double> result(A.size(), 0.0);
        for (std::size_t i = 0; i < A.size(); ++i)
        {
            result[i] = dot(A[i], v);   // row i of A times v
        }
        return result;
    }

    static void check_symmetric(const std::vector<std::vector<double>>& A)
    {
        for (std::size_t i = 0; i < A.size(); ++i)
        {
            for (std::size_t j = i + 1; j < A.size(); ++j)
            {
                double scale = std::max(1.0, std::max(std::abs(A[i][j]), std::abs(A[j][i])));
                if (!(std::abs(A[i][j] - A[j][i]) <= 1e-12 * scale))
                {
                    throw std::invalid_argument("The conjugate gradient method needs a symmetric matrix.");
                }
            }
        }
    }

    std::vector<double> ConjugateGradientSolver::solve(const std::vector<std::vector<double>>& A, const std::vector<double>& b, double tol, int max_iter)
    {
        // Same algorithm, starting from the zero vector.
        return solve_from(A, b, std::vector<double>(b.size(), 0.0), tol, max_iter);
    }

    std::vector<double> ConjugateGradientSolver::solve_from(const std::vector<std::vector<double>>& A, const std::vector<double>& b,
                                                            const std::vector<double>& x0, double tol, int max_iter)
    {
        checks::check_square_system(A, b);
        checks::check_tolerance(tol, max_iter);
        if (x0.size() != b.size())
        {
            throw std::invalid_argument("Starting guess x0 must have the same size as b.");
        }
        check_symmetric(A);

        // Stop when ||r|| <= tol ||b||. For b = 0 the answer is x = 0, reached with ||r|| <= tol.
        double b_norm = std::sqrt(dot(b, b));
        double target = tol * (b_norm > 0.0 ? b_norm : 1.0);

        std::vector<double> x = x0;
        std::vector<double> Ax = multiply(A, x);
        std::vector<double> r(b.size());          // residual b - A x
        for (std::size_t i = 0; i < b.size(); ++i)
        {
            r[i] = b[i] - Ax[i];
        }
        std::vector<double> p = r;                // first search direction: steepest descent
        double rr = dot(r, r);

        if (std::sqrt(rr) <= target)
        {
            return x;   // the starting guess is already good enough
        }

        for (int k = 0; k < max_iter; ++k)
        {
            std::vector<double> Ap = multiply(A, p);
            double pAp = dot(p, Ap);

            // For an SPD matrix p.A p > 0 for every non-zero p.
            if (!(pAp > 0.0))
            {
                throw std::runtime_error("Matrix is not positive definite: conjugate gradient cannot be used.");
            }

            double alpha = rr / pAp;              // step length that minimises the error along p
            for (std::size_t i = 0; i < x.size(); ++i)
            {
                x[i] += alpha * p[i];
                r[i] -= alpha * Ap[i];            // updated residual, without another product A x
            }

            double rr_new = dot(r, r);
            if (std::sqrt(rr_new) <= target)
            {
                return x;
            }

            // New direction: the residual made A-conjugate to the previous direction.
            double beta = rr_new / rr;
            for (std::size_t i = 0; i < p.size(); ++i)
            {
                p[i] = r[i] + beta * p[i];
            }
            rr = rr_new;
        }

        throw std::runtime_error("Conjugate gradient did not reach the tolerance within max_iter iterations.");
    }
}
