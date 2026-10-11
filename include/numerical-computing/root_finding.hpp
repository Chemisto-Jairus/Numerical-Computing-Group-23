#pragma once
#include <functional>

namespace numcomp 
{
    class BasicRootFinder 
    {
    public:
        BasicRootFinder() = default;

        double bisection(std::function<double(double)> f, double a, double b, double tol, int max_iter);
        double newton_raphson(std::function<double(double)> f, std::function<double(double)> df, double x0, double tol, int max_iter);
    };
}