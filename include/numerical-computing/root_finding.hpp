#ifndef ROOT_FINDING_HPP
#define ROOT_FINDING_HPP

#include <functional>

namespace numcomp {

    // core root finding methods

    double bisection(std::function<double(double)> func, double a, double b, double tol, int max_iter);
    double newtonRaphson(std::function<double(double)> func, std::function<double(double)> deriv, double initial_guess, double tol, int max_iter);

} 
#endif 