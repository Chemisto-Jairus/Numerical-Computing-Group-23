/**
 * @file example_conjugate_gradient_solver.cpp
 * @brief Demo 1 of 2 for numcomp::ConjugateGradientSolver: the conjugate gradient method.
 * @author Bakalu Valeria (Member 4)
 */

#include "numerical-computing/conjugate_gradient_solver.hpp"
#include "numerical-computing/matrix_solver.hpp"
#include "example_helpers.hpp"
#include <cmath>
#include <stdexcept>
#include <vector>

/// Matrix type used in the demo (a vector of rows).
using Matrix = std::vector<std::vector<double>>;

/**
 * @brief Prints a solution vector as x1 = ..., x2 = ..., ...
 * @param x  The vector to print.
 */
void print_vector(const std::vector<double>& x)
{
    for (std::size_t i = 0; i < x.size(); ++i)
    {
        demo::value("x" + std::to_string(i + 1), x[i], 10);
    }
}

/**
 * @brief Runs the demo sections.
 * @return 0 if every section completed, 1 otherwise.
 */
int main()
{
    numcomp::ConjugateGradientSolver cg;
    demo::title("Conjugate Gradient Solver", "Iterative solution of symmetric positive definite systems");

    demo::section("3 x 3 SPD system with answer (1, 2, 3)", [&] {
        print_vector(cg.solve({{4, 1, 0}, {1, 3, 1}, {0, 1, 2}}, {6, 10, 8}, 1e-12, 100));
    });

    demo::section("Smallest max_iter that reaches tol = 1e-10, 2 x 2 to 40 x 40 tridiagonal systems", [&] {
        demo::table_header({"unknowns n", "iterations"});
        for (int n : {2, 5, 10, 20, 40})
        {
            Matrix A(n, std::vector<double>(n, 0.0));
            for (int i = 0; i < n; ++i)
            {
                A[i][i] = 2.0;
                if (i > 0) A[i][i-1] = -1.0;
                if (i < n - 1) A[i][i+1] = -1.0;
            }
            std::vector<double> b(n, 1.0);
            int needed = 0;
            for (int k = 1; k <= 2 * n && needed == 0; ++k)
            {
                try
                {
                    cg.solve(A, b, 1e-10, k);
                    needed = k;
                }
                catch (const std::runtime_error&)
                {
                    // not converged yet with k iterations: try one more
                }
            }
            demo::table_row({static_cast<double>(n), static_cast<double>(needed)}, 4);
        }
        std::cout << "  CG never needs more than n iterations here (n steps is the theoretical limit).\n";
    });

    demo::section("Same answer as Gaussian elimination", [&] {
        Matrix A = {{10, -1, 2, 0}, {-1, 11, -1, 3}, {2, -1, 10, -1}, {0, 3, -1, 8}};
        std::vector<double> b = {6, 25, -11, 15};
        numcomp::MatrixSolver direct;
        std::vector<double> x_cg = cg.solve(A, b, 1e-12, 100);
        std::vector<double> x_ge = direct.gaussian_elimination(A, b);
        for (std::size_t i = 0; i < b.size(); ++i)
        {
            demo::compare("x" + std::to_string(i + 1) + " (CG vs Gauss)", x_cg[i], x_ge[i]);
        }
        std::cout << "  Here the \"exact\" column is the answer from Gaussian elimination.\n";
    });

    demo::section("Invalid input is rejected", [&] {
        demo::show_rejected("Non-symmetric matrix", [&] { cg.solve({{4, 1}, {2, 3}}, {1, 2}, 1e-10, 10); });
        demo::show_rejected("Symmetric but not positive definite", [&] { cg.solve({{1, 2}, {2, 1}}, {1, -1}, 1e-10, 10); });
        demo::show_rejected("Tolerance 0", [&] { cg.solve({{4, 1}, {1, 3}}, {1, 2}, 0.0, 10); });
    });

    return demo::finish();
}
