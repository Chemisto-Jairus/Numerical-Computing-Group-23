#pragma once

#include <functional>
#include <utility>
#include <vector>

namespace numcomp
{
    class FiniteDifferenceSolver
    {
    public:
        FiniteDifferenceSolver() = default;

        std::vector<std::pair<double, double>> linear_bvp(std::function<double(double)> p, std::function<double(double)> q,
                                                          std::function<double(double)> r, double a, double b,
                                                          double alpha, double beta, int n);

        std::vector<std::pair<double, double>> poisson(std::function<double(double)> r, double a, double b,
                                                       double alpha, double beta, int n);
    };
}
