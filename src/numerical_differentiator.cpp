#include "numerical-computing/numerical_differentiator.hpp"
#include "internal/input_checks.hpp"

namespace numcomp
{
    double NumericalDifferentiator::forward_difference(std::function<double(double)> f, double x, double h)
    {
        checks::check_difference_inputs(x, h);
        double result = (f(x + h) - f(x)) / h;
        checks::check_finite_result(result);
        return result;
    }

    double NumericalDifferentiator::backward_difference(std::function<double(double)> f, double x, double h)
    {
        checks::check_difference_inputs(x, h);
        double result = (f(x) - f(x - h)) / h;
        checks::check_finite_result(result);
        return result;
    }

    double NumericalDifferentiator::central_difference(std::function<double(double)> f, double x, double h)
    {
        checks::check_difference_inputs(x, h);
        double result = (f(x + h) - f(x - h)) / (2.0 * h);
        checks::check_finite_result(result);
        return result;
    }

    double NumericalDifferentiator::second_derivative(std::function<double(double)> f, double x, double h)
    {
        checks::check_difference_inputs(x, h);
        double result = (f(x + h) - 2.0 * f(x) + f(x - h)) / (h * h);
        checks::check_finite_result(result);
        return result;
    }

    double NumericalDifferentiator::richardson_extrapolation(std::function<double(double)> f, double x, double h)
    {
        // Both calls validate x and h and check their own results.
        double coarse = central_difference(f, x, h);
        double fine   = central_difference(f, x, h / 2.0);

        // The h^2 error terms cancel: fine has a quarter of coarse's error.
        return (4.0 * fine - coarse) / 3.0;
    }
}
