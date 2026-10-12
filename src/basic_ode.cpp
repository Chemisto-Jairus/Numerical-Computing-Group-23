#include "numerical-computing/basic_ode.hpp"
#include <stdexcept>

namespace numcomp 
{
    std::vector<std::pair<double, double>> BasicODESolver::euler(std::function<double(double, double)> f, double t0, double y0, double t_end, double h) 
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
                h = t_end - t;
            }
            
            y = y + h * f(t, y);
            t = t + h;
            
            result.push_back({t, y});
        }
        
        return result;
    }
}