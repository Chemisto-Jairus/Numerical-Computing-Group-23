#include "numerical-computing/basic_ode_solver.hpp"
#include <stdexcept>

namespace numcomp 
{
    std::vector<std::pair<double, double>> BasicODESolver::heun(std::function<double(double, double)> f, double t0, double y0, double t_end, double h) 
    {
        if (h <= 0 || t_end <= t0) {
            throw std::invalid_argument("Step size h must be strictly positive and t_end must be greater than t0.");
        }

        // Stop absurdly small steps: they would need millions of points of memory
        if ((t_end - t0) / h > 1000000) {
            throw std::invalid_argument("Too many steps: please use a bigger step size h.");
        }

        std::vector<std::pair<double, double>> result;
        result.push_back({t0, y0}); // Store initial condition

        double t = t0;
        double y = y0;
        double eps = 1e-9 * h;   // if less time than this is left, we are at the end

        while (t_end - t > eps) {
            // Prevent overshooting the target time on the final step
            if (t + h > t_end) {
                h = t_end - t; 
            }
            
            // Predictor step (Standard Euler)
            double k1 = f(t, y);
            
            // Corrector step
            double k2 = f(t + h, y + h * k1);
            
            y = y + (h / 2.0) * (k1 + k2);
            t = t + h;
            if (t_end - t <= eps) t = t_end;   // land exactly on t_end
            
            result.push_back({t, y});
        }
        
        return result;
    }
    
    std::vector<std::pair<double, double>> BasicODESolver::euler(std::function<double(double, double)> f, double t0, double y0, double t_end, double h) 
    {
        if (h <= 0 || t_end <= t0) {
            throw std::invalid_argument("Step size h must be strictly positive and t_end must be greater than t0.");
        }

        // Stop absurdly small steps: they would need millions of points of memory
        if ((t_end - t0) / h > 1000000) {
            throw std::invalid_argument("Too many steps: please use a bigger step size h.");
        }

        std::vector<std::pair<double, double>> result;
        result.push_back({t0, y0}); // Store initial condition

        double t = t0;
        double y = y0;
        double eps = 1e-9 * h;   // if less time than this is left, we are at the end

        while (t_end - t > eps) {
            // Prevent overshooting the target time on the final step
            if (t + h > t_end) {
                h = t_end - t;
            }
            
            y = y + h * f(t, y);
            t = t + h;
            if (t_end - t <= eps) t = t_end;   // land exactly on t_end
            
            result.push_back({t, y});
        }
        
        return result;
    }
}