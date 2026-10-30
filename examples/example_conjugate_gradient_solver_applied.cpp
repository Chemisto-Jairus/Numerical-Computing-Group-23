/**
 * @file example_conjugate_gradient_solver_applied.cpp
 * @brief Demo 2 of 2 for numcomp::ConjugateGradientSolver: applied SPD systems.
 * @author Bakalu Valeria (Member 4)
 *
 * - Displacements of a chain of springs (stiffness matrix).
 * - Steady temperature along a heated rod with 99 unknowns.
 * - A straight-line fit through measurements (normal equations).
 */

#include "numerical-computing/conjugate_gradient_solver.hpp"
#include "example_helpers.hpp"
#include <cmath>
#include <vector>

/// Matrix type used in the demo (a vector of rows).
using Matrix = std::vector<std::vector<double>>;

/**
 * @brief Runs the demo sections.
 * @return 0 if every section completed, 1 otherwise.
 */
int main()
{
    numcomp::ConjugateGradientSolver cg;
    demo::title("Conjugate Gradient Solver (applied)", "Springs, a heated rod and a line of best fit");

    demo::section("Three masses between springs k = 100, 200, 200, 100 N/m, loads 10, 20, 10 N", [&] {
        // Stiffness matrix: K_ii = k_i + k_(i+1), K_(i,i+1) = -k_(i+1). It is always SPD.
        Matrix K = {{300, -200, 0}, {-200, 400, -200}, {0, -200, 300}};
        std::vector<double> u = cg.solve(K, {10, 20, 10}, 1e-12, 50);
        for (std::size_t i = 0; i < u.size(); ++i)
        {
            demo::value("Displacement of mass " + std::to_string(i + 1) + " (m)", u[i], 6);
        }
        std::cout << "  The load is symmetric, so masses 1 and 3 move by the same amount.\n";
    });

    demo::section("Rod of length 1 m, ends at 0 deg C, heat source: -T'' = 1, 99 inner points", [&] {
        int n = 99;
        double h = 1.0 / (n + 1);
        Matrix A(n, std::vector<double>(n, 0.0));
        for (int i = 0; i < n; ++i)
        {
            A[i][i] = 2.0;
            if (i > 0) A[i][i-1] = -1.0;
            if (i < n - 1) A[i][i+1] = -1.0;
        }
        std::vector<double> b(n, h * h);   // -T'' = 1 multiplied by h^2
        std::vector<double> T = cg.solve(A, b, 1e-12, n);
        demo::table_header({"x (m)", "T (CG)", "exact x(1-x)/2"});
        for (int i = 9; i < n; i += 20)
        {
            double x = (i + 1) * h;
            demo::table_row({x, T[i], x * (1.0 - x) / 2.0}, 8);
        }
        std::cout << "  99 unknowns solved in at most 99 iterations, using only matrix-vector products.\n";
    });

    demo::section("Line y = c0 + c1 x through (0, 1.1), (1, 2.9), (2, 5.2), (3, 6.8), (4, 9.1)", [&] {
        std::vector<double> x = {0, 1, 2, 3, 4};
        std::vector<double> y = {1.1, 2.9, 5.2, 6.8, 9.1};
        // Normal equations (X^T X) c = X^T y: the matrix X^T X is SPD.
        double n = 5, sx = 0, sxx = 0, sy = 0, sxy = 0;
        for (std::size_t i = 0; i < x.size(); ++i)
        {
            sx += x[i]; sxx += x[i] * x[i]; sy += y[i]; sxy += x[i] * y[i];
        }
        std::vector<double> c = cg.solve({{n, sx}, {sx, sxx}}, {sy, sxy}, 1e-12, 10);
        demo::value("Intercept c0", c[0], 6);
        demo::value("Slope c1", c[1], 6);
    });

    return demo::finish();
}
