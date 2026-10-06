#pragma once
#include <functional>

namespace numcomp 
{
    double trapezoidal_rule(std::function<double(double)> f, double a, double b, int n);
}