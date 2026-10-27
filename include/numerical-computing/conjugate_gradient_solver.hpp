#pragma once

#include <vector>

namespace numcomp
{
    class ConjugateGradientSolver
    {
    public:
        ConjugateGradientSolver() = default;

        std::vector<double> solve(const std::vector<std::vector<double>>& A, const std::vector<double>& b, double tol, int max_iter);

        std::vector<double> solve_from(const std::vector<std::vector<double>>& A, const std::vector<double>& b,
                                       const std::vector<double>& x0, double tol, int max_iter);
    };
}
