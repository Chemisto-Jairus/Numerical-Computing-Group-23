#include "numerical-computing/adv_integration.hpp"
#include <stdexcept>

namespace numcomp 
{
    double simpsons_rule(std::function<double(double)> f, double a, double b, int n) {
        if (n <= 0 || n % 2 != 0) 
        {
            throw std::invalid_argument("Simpson's rule requires a strictly positive and even number of intervals.");
        }
        
        double h  = (b - a) / n;
        double sum = f(a) + f(b) ;
        
        for (int i = 1; i < n; ++i) {
            double x = a + i * h ;
            sum += (i %  2 == 0) ? 2.0 * f(x) : 4.0 * f(x);
        }
        
        return sum * h / 3.0;
    }
}