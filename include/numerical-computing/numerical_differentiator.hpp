#pragma once

#include <functional>

namespace numcomp
{
    class NumericalDifferentiator
    {
    public:
        NumericalDifferentiator() = default;

        double forward_difference(std::function<double(double)> f, double x, double h);

        double backward_difference(std::function<double(double)> f, double x, double h);

        double central_difference(std::function<double(double)> f, double x, double h);

        double second_derivative(std::function<double(double)> f, double x, double h);

        double richardson_extrapolation(std::function<double(double)> f, double x, double h);
    };
}
