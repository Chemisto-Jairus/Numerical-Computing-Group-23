#include "numerical-computing/advanced_ode.hpp"
#include <stdexcept>

namespace numcomp 
{
    std::vector<std::pair<double, double>> AdvancedODESolver::rk4(std::function<double(double, double)> f, double t0, double y0, double t_end, double h) 
    {
        if (h <= 0 || t_end <= t0) {
            throw std::invalid_argument("Step size h must be strictly positive and t_end must be greater than t0.");
        }

        std::vector<std::pair<double, double>> result;
        result.push_back({t0, y0});

        double t = t0;
        double y = y0;

        while (t < t_end) {
            if (t + h > t_end) {
                h = t_end - t; // Prevent overshooting the final time step
            }
            
            double k1 = f(t, y);
            double k2 = f(t + h / 2.0, y + (h / 2.0) * k1);
            double k3 = f(t + h / 2.0, y + (h / 2.0) * k2);
            double k4 = f(t + h, y + h * k3);
            
            y = y + (h / 6.0) * (k1 + 2.0 * k2 + 2.0 * k3 + k4);
            t = t + h;
            
            result.push_back({t, y});
        }
        
        return result;
    }
}