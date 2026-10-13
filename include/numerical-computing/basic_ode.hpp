#pragma once
#include <vector>
#include <functional>
#include <utility>

namespace numcomp 
{
    class BasicODESolver 
    {
    public:
        std::vector<std::pair<double, double>> euler(std::function<double(double, double)> f, double t0, double y0, double t_end, double h);
    };
}