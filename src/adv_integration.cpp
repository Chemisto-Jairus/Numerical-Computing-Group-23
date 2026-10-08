#include "numerical-computing/adv_integration.hpp"
#include <stdexcept>
#include <vector>
#include <cmath>

namespace numcomp
 {
    double AdvancedIntegrator::simpsons_rule(std::function<double(double)> f, double a, double b, int n) {
        if (n <= 0 || n % 2 != 0) throw std::invalid_argument("Requires positive even number of intervals.");
        
        double h = (b - a) / n;
        double sum = f(a) + f(b);
        
        for (int i = 1; i < n; ++i) {
            double x = a + i * h;
            sum += (i % 2 == 0) ? 2.0 * f(x) : 4.0 * f(x);
        }
        return sum * h / 3.0;
    }

    double AdvancedIntegrator::romberg_integration(std::function<double(double)> f, double a, double b, int max_iter) {
        if (max_iter <= 0) throw std::invalid_argument("max_iter must be positive.");
        
        std::vector<std::vector<double>> R(max_iter, std::vector<double>(max_iter, 0.0));
        
        for (int i = 0; i < max_iter; ++i) 
        {
            int n = 1 << i; 
            double h = (b - a) / n;
            double sum = 0.5 * (f(a) + f(b));
            for (int k = 1; k < n; ++k) sum += f(a + k * h);
            R[i][0] = sum * h;
        }

        for (int j = 1; j < max_iter; ++j) {
            for (int i = j; i < max_iter; ++i) {
                double factor = std::pow(4, j);
                R[i][j] = (factor * R[i][j-1] - R[i-1][j-1]) / (factor - 1.0);
            }
        }
        return R[max_iter - 1][max_iter - 1];
    }
}