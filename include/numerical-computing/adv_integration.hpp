#pragma once
#include <functional>


namespace numcomp {
    class AdvancedIntegrator 
    {
    public:
        AdvancedIntegrator()  = default;

        double  simpsons_rule(std::function<double(double)> f, double a, double b, int n);
        double romberg_integration(std::function<double(double)> f, double a, double b, int max_iter);
    };
}