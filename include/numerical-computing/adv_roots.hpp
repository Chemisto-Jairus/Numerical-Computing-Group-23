#pragma once
#include <functional>

namespace numcomp 
{
    double secant_method(std::function<double(double)> f, double x0, double x1, double tol, int max_iter);
}