/**
 * @file example_matrix_solver_applied.cpp
 * @brief Demo 2 of 2 for numcomp::MatrixSolver: applied linear systems.
 * @author Batte Derrick Calvin (Member 2)
 *
 * - Loop currents of a resistor circuit (Kirchhoff's voltage law).
 * - The parabola through three measured points.
 * - Mixing problem: litres of three alloys needed for a target composition.
 */

#include "numerical-computing/matrix_solver.hpp"
#include "example_helpers.hpp"
#include <vector>

/**
 * @brief Runs the demo sections.
 * @return 0 if every section completed, 1 otherwise.
 */
int main()
{
    numcomp::MatrixSolver solver;
    demo::title("Matrix Solver (applied)", "Circuit currents, curve through points, mixing problem");

    demo::section("Three-loop resistor circuit (ohms, volts): loop currents I1, I2, I3", [&] {
        // ( 15  -5   0) (I1)   (10)
        // ( -5  20 -10) (I2) = ( 0)
        // (  0 -10  25) (I3)   ( 5)
        std::vector<std::vector<double>> R = {{15, -5, 0}, {-5, 20, -10}, {0, -10, 25}};
        std::vector<double> V = {10, 0, 5};
        std::vector<double> I = solver.lu_solve(R, V);
        demo::value("I1 (A)", I[0], 8);
        demo::value("I2 (A)", I[1], 8);
        demo::value("I3 (A)", I[2], 8);
    });

    demo::section("Parabola y = a + b x + c x^2 through (1, 4), (2, 3), (4, 9)", [&] {
        std::vector<std::vector<double>> V = {{1, 1, 1}, {1, 2, 4}, {1, 4, 16}};
        std::vector<double> y = {4, 3, 9};
        std::vector<double> c = solver.gaussian_elimination(V, y);
        demo::value("a", c[0], 8);
        demo::value("b", c[1], 8);
        demo::value("c", c[2], 8);
        demo::compare("Check: y(4)", c[0] + 4 * c[1] + 16 * c[2], 9.0);
    });

    demo::section("Mixing three alloys (% copper, zinc, tin) to make 100 L of 55/32/13", [&] {
        // Each column is one alloy's composition: A = 70/20/10, B = 50/40/10, C = 40/40/20.
        // Row i: total amount of metal i contributed by the three alloys.
        std::vector<std::vector<double>> A = {{0.70, 0.50, 0.40}, {0.20, 0.40, 0.40}, {0.10, 0.10, 0.20}};
        std::vector<double> target = {55, 32, 13};   // litres of copper, zinc, tin
        std::vector<double> litres = solver.gaussian_elimination(A, target);
        demo::value("Litres of alloy A", litres[0], 6);
        demo::value("Litres of alloy B", litres[1], 6);
        demo::value("Litres of alloy C", litres[2], 6);
        demo::value("Total (should be 100)", litres[0] + litres[1] + litres[2], 6);
    });

    return demo::finish();
}
