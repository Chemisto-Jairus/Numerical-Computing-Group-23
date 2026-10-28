#pragma once
/**
 * @file conjugate_gradient_solver.hpp
 * @brief Iterative solution of symmetric positive definite linear systems (conjugate gradient method).
 * @author Bakalu Valeria (Member 4)
 */

#include <vector>

namespace numcomp
{
    /**
     * @brief Solves A x = b iteratively when A is symmetric positive definite (SPD).
     *
     * The conjugate gradient (CG) method improves a guess x step by step. Each
     * step moves along a search direction that is "A-conjugate" to all earlier
     * directions, so in exact arithmetic CG finishes in at most n steps for an
     * n x n system. In practice it stops as soon as the residual r = b - A x is small:
     * ||r|| <= tol * ||b||.
     *
     * Each step needs one matrix-vector product and no row operations, so A is
     * never changed. This makes CG the method of choice for large, sparse SPD
     * systems (for example from finite differences), where Gaussian elimination
     * would fill the matrix in.
     *
     * The class is stateless.
     *
     * @code
     * numcomp::ConjugateGradientSolver cg;
     * std::vector<std::vector<double>> A = {{4, 1}, {1, 3}};
     * std::vector<double> x = cg.solve(A, {1, 2}, 1e-12, 100);   // 1/11, 7/11
     * @endcode
     *
     * @see MatrixSolver for direct methods that work for any non-singular matrix.
     */
    class ConjugateGradientSolver
    {
    public:
        /// Creates a solver. The class holds no state, so nothing needs to be set up.
        ConjugateGradientSolver() = default;

        /**
         * @brief Solves A x = b with the conjugate gradient method, starting from x = 0.
         *
         * @param A         Symmetric positive definite matrix, stored as a vector of rows.
         * @param b         Right-hand side.
         * @param tol       Relative residual tolerance: stop when ||b - A x|| <= tol ||b||. Must be > 0.
         * @param max_iter  Largest number of iterations. Must be > 0.
         * @return The solution x.
         *
         * @throws std::invalid_argument If A is not square, b has the wrong size, A is not
         *         symmetric, tol <= 0 (or NaN), or max_iter <= 0.
         * @throws std::runtime_error If A turns out not to be positive definite, or the
         *         tolerance is not reached within max_iter iterations.
         */
        std::vector<double> solve(const std::vector<std::vector<double>>& A, const std::vector<double>& b, double tol, int max_iter);

        /**
         * @brief Solves A x = b with the conjugate gradient method from a given starting guess.
         *
         * @details r_0 = b - A x_0, p_0 = r_0. For k = 0, 1, ...:
         * alpha = (r.r) / (p.A p), x = x + alpha p, r = r - alpha A p,
         * beta = (r_new.r_new) / (r.r), p = r + beta p.
         * A good starting guess (for example the answer to a nearby problem) saves iterations.
         *
         * @param A         Symmetric positive definite matrix, stored as a vector of rows.
         * @param b         Right-hand side.
         * @param x0        Starting guess, same size as b.
         * @param tol       Relative residual tolerance. Must be > 0.
         * @param max_iter  Largest number of iterations. Must be > 0.
         * @return The solution x.
         *
         * @throws std::invalid_argument If the sizes do not match, A is not symmetric,
         *         tol <= 0 (or NaN), or max_iter <= 0.
         * @throws std::runtime_error If A turns out not to be positive definite, or the
         *         tolerance is not reached within max_iter iterations.
         */
        std::vector<double> solve_from(const std::vector<std::vector<double>>& A, const std::vector<double>& b,
                                       const std::vector<double>& x0, double tol, int max_iter);
    };
}
