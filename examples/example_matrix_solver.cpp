/**
 * @file example_matrix_solver.cpp
 * @brief Demo 1 of 2 for numcomp::MatrixSolver: Gaussian elimination and LU decomposition.
 * @author Batte Derrick Calvin (Member 2)
 */

#include "numerical-computing/matrix_solver.hpp"
#include "example_helpers.hpp"
#include <vector>

/**
 * @brief Prints a solution vector as x1 = ..., x2 = ..., ...
 * @param label  Text printed before the vector.
 * @param x      The solution vector.
 */
void print_solution(const std::string& label, const std::vector<double>& x)
{
    std::cout << "  " << label << ":";
    for (std::size_t i = 0; i < x.size(); ++i)
    {
        std::cout << "  x" << i + 1 << " = " << std::setprecision(8) << std::fixed << x[i];
    }
    std::cout << "\n";
}

/**
 * @brief Runs the demo sections.
 * @return 0 if every section completed, 1 otherwise.
 */
int main()
{
    numcomp::MatrixSolver solver;
    demo::title("Matrix Solver", "Gaussian elimination and LU decomposition with partial pivoting");

    demo::section("3 x 3 system with answer (2, 3, -1)", [&] {
        std::vector<std::vector<double>> A = {{2, 1, -1}, {-3, -1, 2}, {-2, 1, 2}};
        std::vector<double> b = {8, -11, -3};
        print_solution("Gaussian", solver.gaussian_elimination(A, b));
        print_solution("LU      ", solver.lu_solve(A, b));
    });

    demo::section("Zero in the first pivot position: rows are swapped automatically", [&] {
        std::vector<std::vector<double>> A = {{0, 2, 1}, {1, 1, 1}, {2, 1, 0}};
        std::vector<double> b = {7, 6, 4};
        print_solution("Gaussian (answer 1, 2, 3)", solver.gaussian_elimination(A, b));
    });

    demo::section("4 x 4 system built from x = (1, -2, 3, 0.5)", [&] {
        std::vector<std::vector<double>> A = {{4, -1, 0, 2}, {1, 5, -2, 0}, {0, 2, 6, -1}, {3, 0, 1, 7}};
        std::vector<double> x = {1, -2, 3, 0.5};
        std::vector<double> b(4, 0.0);
        for (int i = 0; i < 4; ++i) for (int j = 0; j < 4; ++j) b[i] += A[i][j] * x[j];
        print_solution("LU", solver.lu_solve(A, b));
    });

    demo::section("Invalid input is rejected", [&] {
        demo::show_rejected("Singular matrix (row 2 = 2 x row 1)", [&] { solver.gaussian_elimination({{1, 2}, {2, 4}}, {3, 6}); });
        demo::show_rejected("Non-square matrix", [&] { solver.lu_solve({{1, 2, 3}, {4, 5, 6}}, {1, 2}); });
        demo::show_rejected("b of the wrong size", [&] { solver.lu_solve({{1, 2}, {3, 4}}, {1}); });
    });

    return demo::finish();
}
