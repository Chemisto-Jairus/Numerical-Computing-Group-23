#pragma once

#include <functional>
#include <string>
#include <vector>

namespace app
{
    namespace catalog
    {
        using Function1D = std::function<double(double)>;

        using Function2D = std::function<double(double, double)>;

        struct RootProblem
        {
            std::string name;
            Function1D f;
            Function1D df;
            double a;
            double b;
            double x0;
            double root;
        };

        struct FixedPointProblem
        {
            std::string name;
            Function1D g;
            double x0;
            double fixed_point;
        };

        struct Integral
        {
            std::string name;
            Function1D f;
            double a;
            double b;
            Function1D antiderivative;
        };

        struct ODEProblem
        {
            std::string name;
            Function2D f;
            double t0;
            double y0;
            double t_end;
            double h;
            Function1D exact;
        };

        struct DataSet
        {
            std::string name;
            std::vector<double> x;
            std::vector<double> y;
            Function1D exact;
        };

        struct LinearSystem
        {
            std::string name;
            std::vector<std::vector<double>> A;
            std::vector<double> b;
        };

        const std::vector<RootProblem>& root_problems();

        const std::vector<FixedPointProblem>& fixed_point_problems();

        const std::vector<Integral>& integrals();

        const std::vector<ODEProblem>& ode_problems();

        const std::vector<DataSet>& data_sets();

        const std::vector<LinearSystem>& linear_systems();

        template <typename Entry>
        std::vector<std::string> names(const std::vector<Entry>& entries)
        {
            std::vector<std::string> result;
            for (const Entry& e : entries) result.push_back(e.name);
            return result;
        }
    }
}
