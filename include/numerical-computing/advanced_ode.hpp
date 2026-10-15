#pragma once
#include <vector>
#include <functional>
#include <utility>

namespace numcomp 
{
    class AdvancedODESolver 
    {
    public:
        // Solves dy/dt = f(t, y) using the 4th-Order Runge-Kutta Method (RK4)
        // Returns a vector of (t, y) pairs representing the solution curve
        std::vector<std::pair<double, double>> rk4(std::function<double(double, double)> f, double t0, double y0, double t_end, double h);
        // Solves dy/dt = f(t, y) using the 3rd-Order Runge-Kutta Method (RK3)
        std::vector<std::pair<double, double>> rk3(std::function<double(double, double)> f, double t0, double y0, double t_end, double h);
    };
}