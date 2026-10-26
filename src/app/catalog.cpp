#include "catalog.hpp"
#include <cmath>

namespace app
{
    namespace catalog
    {
        static const double PI = std::acos(-1.0);

        const std::vector<RootProblem>& root_problems()
        {
            static const std::vector<RootProblem> list = {
                {"x^2 - 4 = 0                 (polynomial, root 2)",
                 [](double x) { return x * x - 4.0; }, [](double x) { return 2.0 * x; }, 0.0, 3.0, 3.0, 2.0},
                {"x^3 - x - 2 = 0             (cubic)",
                 [](double x) { return x * x * x - x - 2.0; }, [](double x) { return 3.0 * x * x - 1.0; }, 1.0, 2.0, 1.5, 1.5213797068045676},
                {"x^3 - 2x - 5 = 0            (Wallis' cubic)",
                 [](double x) { return x * x * x - 2.0 * x - 5.0; }, [](double x) { return 3.0 * x * x - 2.0; }, 2.0, 3.0, 2.0, 2.0945514815423265},
                {"cos(x) - x = 0              (trigonometric)",
                 [](double x) { return std::cos(x) - x; }, [](double x) { return -std::sin(x) - 1.0; }, 0.0, 1.0, 1.0, 0.7390851332151607},
                {"sin(x) - x/2 = 0            (trigonometric)",
                 [](double x) { return std::sin(x) - x / 2.0; }, [](double x) { return std::cos(x) - 0.5; }, 1.0, 3.0, 2.0, 1.8954942670339809},
                {"e^x - 3x = 0                (exponential)",
                 [](double x) { return std::exp(x) - 3.0 * x; }, [](double x) { return std::exp(x) - 3.0; }, 0.0, 1.0, 0.0, 0.6190612867359451},
                {"ln(x) + x - 2 = 0           (logarithmic)",
                 [](double x) { return std::log(x) + x - 2.0; }, [](double x) { return 1.0 / x + 1.0; }, 1.0, 2.0, 1.5, 1.5571455989976114},
            };
            return list;
        }

        const std::vector<FixedPointProblem>& fixed_point_problems()
        {
            static const std::vector<FixedPointProblem> list = {
                {"x = sqrt(x + 2)        (root of x^2 - x - 2)", [](double x) { return std::sqrt(x + 2.0); }, 1.0, 2.0},
                {"x = cos(x)",                                   [](double x) { return std::cos(x); }, 1.0, 0.7390851332151607},
                {"x = e^(-x)             (omega constant)",       [](double x) { return std::exp(-x); }, 0.5, 0.5671432904097838},
                {"x = (x + 2/x) / 2      (Babylonian sqrt(2))",   [](double x) { return 0.5 * (x + 2.0 / x); }, 1.0, std::sqrt(2.0)},
                {"x = 1 + 1/x            (golden ratio)",         [](double x) { return 1.0 + 1.0 / x; }, 1.0, (1.0 + std::sqrt(5.0)) / 2.0},
            };
            return list;
        }

        const std::vector<Integral>& integrals()
        {
            static const std::vector<Integral> list = {
                {"x^2",              [](double x) { return x * x; }, 0.0, 1.0, [](double x) { return x * x * x / 3.0; }},
                {"x^3",              [](double x) { return x * x * x; }, 0.0, 2.0, [](double x) { return x * x * x * x / 4.0; }},
                {"sin(x)",           [](double x) { return std::sin(x); }, 0.0, PI, [](double x) { return -std::cos(x); }},
                {"e^x",              [](double x) { return std::exp(x); }, 0.0, 1.0, [](double x) { return std::exp(x); }},
                {"1 / (1 + x^2)",    [](double x) { return 1.0 / (1.0 + x * x); }, 0.0, 1.0, [](double x) { return std::atan(x); }},
                {"sqrt(x)",          [](double x) { return std::sqrt(x); }, 0.0, 4.0, [](double x) { return 2.0 / 3.0 * x * std::sqrt(x); }},
                {"e^(-x^2)",         [](double x) { return std::exp(-x * x); }, 0.0, 1.0, [](double x) { return std::sqrt(PI) / 2.0 * std::erf(x); }},
                {"1 / x",            [](double x) { return 1.0 / x; }, 1.0, std::exp(1.0), [](double x) { return std::log(x); }},
            };
            return list;
        }

        const std::vector<ODEProblem>& ode_problems()
        {
            static const std::vector<ODEProblem> list = {
                {"Growth:   dy/dt = y,            y(0) = 1",
                 [](double, double y) { return y; }, 0.0, 1.0, 1.0, 0.1, [](double t) { return std::exp(t); }},
                {"Decay:    dy/dt = -2y,          y(0) = 1",
                 [](double, double y) { return -2.0 * y; }, 0.0, 1.0, 1.0, 0.1, [](double t) { return std::exp(-2.0 * t); }},
                {"Gaussian: dy/dt = t y,          y(0) = 1",
                 [](double t, double y) { return t * y; }, 0.0, 1.0, 1.0, 0.1, [](double t) { return std::exp(t * t / 2.0); }},
                {"Linear:   dy/dt = y - t,        y(0) = 2",
                 [](double t, double y) { return y - t; }, 0.0, 2.0, 1.0, 0.1, [](double t) { return std::exp(t) + t + 1.0; }},
                {"Forcing:  dy/dt = cos(t),       y(0) = 0",
                 [](double t, double) { return std::cos(t); }, 0.0, 0.0, PI, 0.1, [](double t) { return std::sin(t); }},
                {"Cooling:  dT/dt = -0.08(T - 22), T(0) = 90",
                 [](double, double T) { return -0.08 * (T - 22.0); }, 0.0, 90.0, 30.0, 0.5, [](double t) { return 22.0 + 68.0 * std::exp(-0.08 * t); }},
                {"Logistic: dP/dt = 0.4 P (1 - P/5000), P(0) = 100",
                 [](double, double P) { return 0.4 * P * (1.0 - P / 5000.0); }, 0.0, 100.0, 20.0, 0.1,
                 [](double t) { return 5000.0 / (1.0 + 49.0 * std::exp(-0.4 * t)); }},
            };
            return list;
        }

        const std::vector<DataSet>& data_sets()
        {
            static const std::vector<DataSet> list = {
                {"y = x^2 at x = 0, 1, 2, 3, 4",
                 {0, 1, 2, 3, 4}, {0, 1, 4, 9, 16}, [](double x) { return x * x; }},
                {"y = sin(x), 7 points on [0, pi]",
                 {0, PI / 6, PI / 3, PI / 2, 2 * PI / 3, 5 * PI / 6, PI},
                 {0, 0.5, std::sqrt(3.0) / 2, 1, std::sqrt(3.0) / 2, 0.5, 0}, [](double x) { return std::sin(x); }},
                {"y = e^x at x = 0, 0.5, 1, 1.5, 2",
                 {0, 0.5, 1, 1.5, 2}, {1, std::exp(0.5), std::exp(1.0), std::exp(1.5), std::exp(2.0)}, [](double x) { return std::exp(x); }},
                {"Runge function 1/(1 + 25x^2), 9 points on [-1, 1]",
                 {-1, -0.75, -0.5, -0.25, 0, 0.25, 0.5, 0.75, 1},
                 {1 / 26.0, 1 / 15.0625, 1 / 7.25, 1 / 2.5625, 1, 1 / 2.5625, 1 / 7.25, 1 / 15.0625, 1 / 26.0},
                 [](double x) { return 1.0 / (1.0 + 25.0 * x * x); }},
                {"Measured temperatures (deg C) at 6, 8, ..., 18 h",
                 {6, 8, 10, 12, 14, 16, 18}, {14.2, 17.8, 22.5, 26.1, 27.4, 24.9, 20.3}, nullptr},
            };
            return list;
        }

        const std::vector<LinearSystem>& linear_systems()
        {
            static const std::vector<LinearSystem> list = {
                {"2 x 2:  2x + y = 5,  x + 3y = 10            (answer 1, 3)", {{2, 1}, {1, 3}}, {5, 10}},
                {"3 x 3:  textbook system                      (answer 2, 3, -1)", {{2, 1, -1}, {-3, -1, 2}, {-2, 1, 2}}, {8, -11, -3}},
                {"3 x 3:  zero first pivot, needs a row swap   (answer 1, 2, 3)", {{0, 2, 1}, {1, 1, 1}, {2, 1, 0}}, {7, 6, 4}},
                {"3 x 3:  resistor circuit loop currents", {{15, -5, 0}, {-5, 20, -10}, {0, -10, 25}}, {10, 0, 5}},
                {"4 x 4:  diagonally dominant                  (answer 1, -2, 3, 0.5)",
                 {{4, -1, 0, 2}, {1, 5, -2, 0}, {0, 2, 6, -1}, {3, 0, 1, 7}}, {7, -15, 13.5, 9.5}},
                {"2 x 2:  singular (rows are multiples)        (shows the error)", {{1, 2}, {2, 4}}, {3, 6}},
            };
            return list;
        }
    }
}
