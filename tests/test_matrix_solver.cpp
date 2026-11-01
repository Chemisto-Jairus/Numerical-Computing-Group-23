/**
 * @file test_matrix_solver.cpp
 * @brief Independent unit tests for numcomp::MatrixSolver (Gaussian elimination and LU).
 * @author Kyeyune Praise Pauline (Member 8)
 *
 * Covers normal use, edge cases and invalid input. Each check prints [PASS] or [FAIL];
 * the program returns 0 only if every check passed (see test_helpers.hpp).
 */

#include "numerical-computing/matrix_solver.hpp"
#include "test_helpers.hpp"
#include <cmath>
#include <stdexcept>
#include <string>
#include <vector>

using test::check;
/// A dense matrix stored as a vector of rows.
using Matrix = std::vector<std::vector<double>>;
/// A column vector.
using Vector = std::vector<double>;

/**
 * @brief Compares two vectors entry by entry.
 * @param a    First vector.
 * @param b    Second vector.
 * @param tol  Largest allowed difference per entry.
 * @return true if the sizes match and every |a[i] - b[i]| <= tol.
 */
bool vectors_close(const Vector& a, const Vector& b, double tol)
{
    if (a.size() != b.size()) return false;
    for (std::size_t i = 0; i < a.size(); ++i)
    {
        if (std::abs(a[i] - b[i]) > tol) return false;
    }
    return true;
}

/**
 * @brief Solves one system with both Gaussian elimination and LU and checks both answers.
 * @param name      Description used in the two result lines.
 * @param A         Coefficient matrix.
 * @param b         Right-hand side.
 * @param expected  Known exact solution.
 * @param tol       Allowed error per entry.
 */
void check_both(const std::string& name, const Matrix& A, const Vector& b, const Vector& expected, double tol)
{
    numcomp::MatrixSolver solver;
    check("Gaussian: " + name, vectors_close(solver.gaussian_elimination(A, b), expected, tol));
    check("LU: " + name, vectors_close(solver.lu_solve(A, b), expected, tol));
}

/// Typical inputs compared with known exact answers.
void normal_cases()
{
    // 2x + y = 5,  x + 3y = 10  ->  x = 1, y = 3
    check_both("2x2 system", {{2.0, 1.0}, {1.0, 3.0}}, {5.0, 10.0}, {1.0, 3.0}, 1e-12);

    // Classic textbook system -> x = 2, y = 3, z = -1
    check_both("3x3 system", {{2.0, 1.0, -1.0}, {-3.0, -1.0, 2.0}, {-2.0, 1.0, 2.0}}, {8.0, -11.0, -3.0}, {2.0, 3.0, -1.0}, 1e-12);

    // Built from the answer x = {1, -2, 3, 0.5}, so b = A * x
    Matrix A4 = {{4.0, -1.0, 0.0, 2.0}, {1.0, 5.0, -2.0, 0.0}, {0.0, 2.0, 6.0, -1.0}, {3.0, 0.0, 1.0, 7.0}};
    Vector x4 = {1.0, -2.0, 3.0, 0.5};
    Vector b4(4, 0.0);
    for (int i = 0; i < 4; ++i)
    {
        for (int j = 0; j < 4; ++j) b4[i] += A4[i][j] * x4[j];
    }
    check_both("4x4 system", A4, b4, x4, 1e-10);

    numcomp::MatrixSolver solver;
    Matrix A = {{3.0, 2.0, -4.0}, {2.0, 3.0, 3.0}, {5.0, -3.0, 1.0}};
    Vector b = {3.0, 15.0, 14.0};
    check("Gaussian and LU give the same answer", vectors_close(solver.gaussian_elimination(A, b), solver.lu_solve(A, b), 1e-12));
}

/// Boundary situations: end points, reversed intervals, minimum sizes.
void edge_cases()
{
    check_both("1x1 system", {{4.0}}, {8.0}, {2.0}, 1e-12);
    // A[0][0] = 0, so without pivoting we would divide by zero
    check_both("zero first pivot (row swap)", {{0.0, 2.0, 1.0}, {1.0, 1.0, 1.0}, {2.0, 1.0, 0.0}}, {7.0, 6.0, 4.0}, {1.0, 2.0, 3.0}, 1e-12);

    numcomp::MatrixSolver solver;
    check("Identity matrix: x equals b", vectors_close(solver.lu_solve({{1.0, 0.0, 0.0}, {0.0, 1.0, 0.0}, {0.0, 0.0, 1.0}}, {4.0, -5.0, 6.0}), {4.0, -5.0, 6.0}, 1e-12));

    Matrix A = {{0.0, 1.0}, {1.0, 0.0}};
    Vector b = {2.0, 3.0};
    Matrix A_copy = A;
    Vector b_copy = b;
    solver.gaussian_elimination(A, b);
    solver.lu_solve(A, b);
    check("Solvers do not change the caller's A and b", A == A_copy && b == b_copy);
}

/// Invalid arguments must throw the exception documented in the header.
void invalid_input()
{
    numcomp::MatrixSolver solver;
    Matrix singular = {{1.0, 2.0}, {2.0, 4.0}};   // row 2 is 2 x row 1

    test::expect_throw<std::invalid_argument>("Gaussian: empty matrix throws invalid_argument", [&] { solver.gaussian_elimination({}, {}); });
    test::expect_throw<std::invalid_argument>("LU: empty matrix throws invalid_argument", [&] { solver.lu_solve({}, {}); });
    test::expect_throw<std::invalid_argument>("Non-square matrix throws invalid_argument", [&] { solver.gaussian_elimination({{1.0, 2.0, 3.0}, {4.0, 5.0, 6.0}}, {1.0, 2.0}); });
    test::expect_throw<std::invalid_argument>("Rows of different length throw invalid_argument", [&] { solver.lu_solve({{1.0, 2.0}, {3.0}}, {1.0, 2.0}); });
    test::expect_throw<std::invalid_argument>("b of the wrong size throws invalid_argument", [&] { solver.lu_solve({{1.0, 2.0}, {3.0, 4.0}}, {1.0}); });
    test::expect_throw<std::runtime_error>("Gaussian: singular matrix throws runtime_error", [&] { solver.gaussian_elimination(singular, {3.0, 6.0}); });
    test::expect_throw<std::runtime_error>("LU: singular matrix throws runtime_error", [&] { solver.lu_solve(singular, {3.0, 6.0}); });
    test::expect_throw<std::runtime_error>("All-zero matrix throws runtime_error", [&] { solver.gaussian_elimination({{0.0, 0.0}, {0.0, 0.0}}, {1.0, 1.0}); });
}

/**
 * @brief Runs every test group.
 * @return 0 if all checks passed, 1 otherwise.
 */
int main()
{
    std::cout << "[*] Running Matrix Solver Tests..." << std::endl;
    normal_cases();
    edge_cases();
    invalid_input();
    return test::finish("Matrix Solver");
}
