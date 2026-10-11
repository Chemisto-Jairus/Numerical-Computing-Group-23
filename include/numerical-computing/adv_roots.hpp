#pragma once
#include <functional>

namespace numcomp 
{
    class AdvancedRootFinder 
    {
    public:
        AdvancedRootFinder() = default;

        double secant_method(std::function<double(double)> f, double x0, double x1, double tol, int max_iter);
        double fixed_point_iteration(std::function<double(double)> g, double x0, double tol, int max_iter);
    };
}