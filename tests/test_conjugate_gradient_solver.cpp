#include "numerical-computing/conjugate_gradient_solver.hpp"
#include "test_helpers.hpp"
#include <cmath>
#include <limits>
#include <stdexcept>
#include <vector>

using test::check;
using test::close;

using Matrix = std::vector<std::vector<double>>;

bool vectors_close(const std::vector<double>& a, const std::vector<double>& b, double tol)
{
    if (a.size() != b.size()) return false;
    for (std::size_t i = 0; i < a.size(); ++i)
    {
        if (!close(a[i], b[i], tol)) return false;
    }
    return true;
}

Matrix second_difference_matrix(int n)
{
    Matrix A(n, std::vector<double>(n, 0.0));
    for (int i = 0; i < n; ++i)
    {
        A[i][i] = 2.0;
        if (i > 0) A[i][i-1] = -1.0;
        if (i < n - 1) A[i][i+1] = -1.0;
    }
    return A;
}

void normal_cases()
{
    numcomp::ConjugateGradientSolver cg;

    check("2 x 2: [[4,1],[1,3]] x = [1,2] gives (1/11, 7/11)",
          vectors_close(cg.solve({{4, 1}, {1, 3}}, {1, 2}, 1e-12, 100), {1.0 / 11.0, 7.0 / 11.0}, 1e-10));
    check("3 x 3 SPD system gives (1, 2, 3)",
          vectors_close(cg.solve({{4, 1, 0}, {1, 3, 1}, {0, 1, 2}}, {6, 10, 8}, 1e-12, 100), {1, 2, 3}, 1e-10));
    check("Diagonal matrix: x_i = b_i / A_ii",
          vectors_close(cg.solve({{2, 0, 0}, {0, 4, 0}, {0, 0, 8}}, {2, 2, 2}, 1e-12, 10), {1, 0.5, 0.25}, 1e-12));

    // The 50 x 50 second-difference system with b = (1, 0, ..., 0, 1) has the solution x = (1, ..., 1).
    // In exact arithmetic CG needs at most n iterations, so max_iter = 50 must be enough.
    int n = 50;
    std::vector<double> b(n, 0.0);
    b[0] = 1.0;
    b[n-1] = 1.0;
    std::vector<double> x = cg.solve(second_difference_matrix(n), b, 1e-10, n);
    check("50 x 50 tridiagonal SPD system solved within n iterations", vectors_close(x, std::vector<double>(n, 1.0), 1e-7));

    // A good starting guess is returned unchanged.
    check("solve_from: the exact answer as x0 is returned immediately",
          vectors_close(cg.solve_from({{4, 1}, {1, 3}}, {1, 2}, {1.0 / 11.0, 7.0 / 11.0}, 1e-12, 1), {1.0 / 11.0, 7.0 / 11.0}, 1e-14));
    check("solve_from: a rough guess still converges",
          vectors_close(cg.solve_from({{4, 1}, {1, 3}}, {1, 2}, {10, -10}, 1e-12, 100), {1.0 / 11.0, 7.0 / 11.0}, 1e-10));
}

void edge_cases()
{
    numcomp::ConjugateGradientSolver cg;
    check("1 x 1 system 5x = 10 gives 2", vectors_close(cg.solve({{5}}, {10}, 1e-12, 5), {2}, 1e-12));
    check("b = 0 gives x = 0", vectors_close(cg.solve({{4, 1}, {1, 3}}, {0, 0}, 1e-12, 10), {0, 0}, 1e-14));

    Matrix A = {{4, 1}, {1, 3}};
    std::vector<double> b = {1, 2};
    cg.solve(A, b, 1e-12, 100);
    check("The caller's A and b are not changed", A[0][0] == 4 && A[0][1] == 1 && A[1][1] == 3 && b[0] == 1 && b[1] == 2);
}

void errors()
{
    numcomp::ConjugateGradientSolver cg;
    double nan = std::numeric_limits<double>::quiet_NaN();

    test::expect_throw<std::invalid_argument>("Empty matrix throws invalid_argument", [&] { cg.solve({}, {}, 1e-10, 10); });
    test::expect_throw<std::invalid_argument>("Non-square matrix throws invalid_argument", [&] { cg.solve({{1, 2, 3}, {4, 5, 6}}, {1, 2}, 1e-10, 10); });
    test::expect_throw<std::invalid_argument>("b of the wrong size throws invalid_argument", [&] { cg.solve({{4, 1}, {1, 3}}, {1, 2, 3}, 1e-10, 10); });
    test::expect_throw<std::invalid_argument>("x0 of the wrong size throws invalid_argument", [&] { cg.solve_from({{4, 1}, {1, 3}}, {1, 2}, {0}, 1e-10, 10); });
    test::expect_throw<std::invalid_argument>("Non-symmetric matrix throws invalid_argument", [&] { cg.solve({{4, 1}, {2, 3}}, {1, 2}, 1e-10, 10); });
    test::expect_throw<std::invalid_argument>("tol = 0 throws invalid_argument", [&] { cg.solve({{4, 1}, {1, 3}}, {1, 2}, 0.0, 10); });
    test::expect_throw<std::invalid_argument>("tol = NaN throws invalid_argument", [&] { cg.solve({{4, 1}, {1, 3}}, {1, 2}, nan, 10); });
    test::expect_throw<std::invalid_argument>("max_iter = 0 throws invalid_argument", [&] { cg.solve({{4, 1}, {1, 3}}, {1, 2}, 1e-10, 0); });
    test::expect_throw<std::runtime_error>("Symmetric but indefinite matrix throws runtime_error", [&] { cg.solve({{1, 2}, {2, 1}}, {1, -1}, 1e-10, 10); });
    test::expect_throw<std::runtime_error>("Negative definite matrix throws runtime_error", [&] { cg.solve({{-2, 0}, {0, -3}}, {1, 1}, 1e-10, 10); });
    test::expect_throw<std::runtime_error>("Too few iterations (max_iter = 2 for 50 unknowns) throws runtime_error",
        [&] { cg.solve(second_difference_matrix(50), std::vector<double>(50, 1.0), 1e-12, 2); });
}

int main()
{
    std::cout << "[*] Running Conjugate Gradient Solver Tests..." << std::endl;
    normal_cases();
    edge_cases();
    errors();
    return test::finish("Conjugate Gradient Solver");
}
