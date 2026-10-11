#pragma once
#include <functional>

namespace numcomp 
{
    class BasicIntegrator 
    {
    public:
        BasicIntegrator() = default;

        double trapezoidal_rule(std::function<double(double)> f, double a, double b, int n);
        double midpoint_rule(std::function<double(double)> f, double a, double b, int n);
    };
}