#pragma once
#include <functional>

namespace numcomp {
    double simpsons_rule(std::function<double(double)> f, double a, double b, int n);
}