#include "numerical-computing/advanced_ode_solver.hpp"
#include <stdexcept>

namespace numcomp 
{
    std::vector<std::pair<double, double>> AdvancedODESolver::rk4(std::function<double(double, double)> f, double t0, double y0, double t_end, double h) 
    {
        if (h <= 0 || t_end <= t0) {
            throw std::invalid_argument("Step size h must be strictly positive and t_end must be greater than t0.");
        }

        // Stop absurdly small steps: they would need millions of points of memory
        if ((t_end - t0) / h > 1000000) {
            throw std::invalid_argument("Too many steps: please use a bigger step size h.");
        }

        std::vector<std::pair<double, double>> result;
        result.push_back({t0, y0});

        double t = t0;
        double y = y0;
        double eps = 1e-9 * h;   // if less time than this is left, we are at the end

        while (t_end - t > eps) {
            if (t + h > t_end) {
                h = t_end - t; // Prevent overshooting the final time step
            }
            
            double k1 = f(t, y);
            double k2 = f(t + h / 2.0, y + (h / 2.0) * k1);
            double k3 = f(t + h / 2.0, y + (h / 2.0) * k2);
            double k4 = f(t + h, y + h * k3);
            
            y = y + (h / 6.0) * (k1 + 2.0 * k2 + 2.0 * k3 + k4);
            t = t + h;
            if (t_end - t <= eps) t = t_end;   // land exactly on t_end
            
            result.push_back({t, y});
        }
        
        return result;
    }
    std::vector<std::pair<double, double>> AdvancedODESolver::rk3(std::function<double(double, double)> f, double t0, double y0, double t_end, double h) 
    {
        if (h <= 0 || t_end <= t0) {
            throw std::invalid_argument("Step size h must be strictly positive and t_end must be greater than t0.");
        }

        // Stop absurdly small steps: they would need millions of points of memory
        if ((t_end - t0) / h > 1000000) {
            throw std::invalid_argument("Too many steps: please use a bigger step size h.");
        }

        std::vector<std::pair<double, double>> result;
        result.push_back({t0, y0});

        double t = t0;
        double y = y0;
        double eps = 1e-9 * h;   // if less time than this is left, we are at the end

        while (t_end - t > eps) {
            if (t + h > t_end) {
                h = t_end - t;
            }
            
            double k1 = f(t, y);
            double k2 = f(t + h / 2.0, y + (h / 2.0) * k1);
            double k3 = f(t + h, y - h * k1 + 2.0 * h * k2);
            
            y = y + (h / 6.0) * (k1 + 4.0 * k2 + k3);
            t = t + h;
            if (t_end - t <= eps) t = t_end;   // land exactly on t_end
            
            result.push_back({t, y});
        }
        
        return result;
    }
}