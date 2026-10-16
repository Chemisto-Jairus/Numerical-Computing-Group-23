#pragma once
#include <vector>
#include <functional>
#include <utility>

namespace numcomp 
{
    class BasicODESolver 
    {
    public:
        // Solves dy/dt = f(t, y) using the standard Euler Method
        // Returns a vector of (t, y) pairs representing the solution curve
        std::vector<std::pair<double, double>> euler(std::function<double(double, double)> f, double t0, double y0, double t_end, double h);
        
        // Solves dy/dt = f(t, y) using Heun's Method (Improved Euler)
        std::vector<std::pair<double, double>> heun(std::function<double(double, double)> f, double t0, double y0, double t_end, double h);
    };
}