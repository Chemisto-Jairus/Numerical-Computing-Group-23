#pragma once

#include <functional>
#include <utility>
#include <vector>

namespace numcomp
{
    class RungeKuttaFehlbergSolver
    {
    public:
        RungeKuttaFehlbergSolver() = default;

        std::vector<std::pair<double, double>> rkf45(std::function<double(double, double)> f, double t0, double y0,
                                                     double t_end, double h_initial, double tol);

        double rkf45_value(std::function<double(double, double)> f, double t0, double y0, double t_end, double tol);
    };
}
