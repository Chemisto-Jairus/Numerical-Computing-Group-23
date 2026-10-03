#include "numerical-computing/root_finding.hpp"
#include <iostream>
#include <cmath>
#include <stdexcept>

using namespace std;

namespace numcomp {

    double bisection(function<double(double)> func, double a, double b, double tol, int max_iter) {
        // check if signs are opposite, otherwise it won't converge
        if (func(a) * func(b) >= 0.0) {
            throw invalid_argument("Error: Function values at interval endpoints must have opposite signs");
        }

        double c = a;
        for (int i = 0; i < max_iter; i++) {
            c = (a + b) / 2.0; 

            
            if (abs(func(c)) < tol || (b - a) / 2.0 < tol) {
                return c;
            }

            
            if (func(c) * func(a) < 0.0) {
                b = c;
            } else {
                a = c;
            }
        }
        
        cout << "warning (bisection): max iterations reached." << endl;
        return c;
    }

    double newtonRaphson(function<double(double)> func, function<double(double)> deriv, double initial_guess, double tol, int max_iter) {
        double x = initial_guess;
        
        for (int i = 0; i < max_iter; i++) {
            double fx = func(x);
            double fprime = deriv(x);
            
            if (abs(fprime) < 1e-10) {
                throw runtime_error("error (newton-raphson): derivative is too close to zero");
            }
            
            double x_next = x - (fx / fprime);
            
            if (abs(x_next - x) < tol) {
                return x_next;
            }
            
            x = x_next;
        }
        
        cout << "warning (newton-raphson): max iterations reached." << endl;
        return x;
    }

} 