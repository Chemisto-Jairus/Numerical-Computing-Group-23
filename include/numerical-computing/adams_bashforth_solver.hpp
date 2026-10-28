#pragma once

#include <functional>
#include <utility>
#include <vector>

namespace numcomp
{
    class AdamsBashforthSolver
    {
    public:
        AdamsBashforthSolver() = default;

        std::vector<std::pair<double, double>> ab2(std::function<double(double, double)> f, double t0, double y0, double t_end, double h);

        std::vector<std::pair<double, double>> ab4(std::function<double(double, double)> f, double t0, double y0, double t_end, double h);
    };
}
