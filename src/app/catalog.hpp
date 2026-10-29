#pragma once
/**
 * @file catalog.hpp
 * @brief Ready-made equations, data sets and linear systems offered by the app.
 * @author Nabakooza Bridget Maria (Member 9)
 *
 * Each entry carries the information the menus need: a readable name, the
 * function(s), suggested starting values, and the exact answer where one is
 * known, so every result can be shown next to its true value and error.
 */

#include <functional>
#include <string>
#include <vector>

namespace app
{
    /**
     * @namespace app::catalog
     * @brief Preset problems for every module of the library.
     */
    namespace catalog
    {
        /// A function of one variable, f(x).
        using Function1D = std::function<double(double)>;

        /// The right-hand side f(t, y) of an ODE dy/dt = f(t, y).
        using Function2D = std::function<double(double, double)>;

        /// An equation f(x) = 0 for the root-finding methods.
        struct RootProblem
        {
            std::string name;    ///< Equation as shown to the user.
            Function1D f;        ///< f(x).
            Function1D df;       ///< f'(x), used by Newton-Raphson.
            double a;            ///< Suggested left end of a bracket.
            double b;            ///< Suggested right end of a bracket.
            double x0;           ///< Suggested starting guess.
            double root;         ///< Root inside [a, b], accurate to about 1e-15.
        };

        /// An equation x = g(x) for Fixed-Point Iteration.
        struct FixedPointProblem
        {
            std::string name;    ///< Equation as shown to the user.
            Function1D g;        ///< g(x).
            double x0;           ///< Suggested starting guess.
            double fixed_point;  ///< The fixed point the iteration converges to.
        };

        /// A definite integral of f from a to b.
        struct Integral
        {
            std::string name;    ///< Integral as shown to the user.
            Function1D f;        ///< Integrand.
            double a;            ///< Suggested lower limit.
            double b;            ///< Suggested upper limit.
            Function1D antiderivative;  ///< F with F' = f, used for the exact value on any [a, b].
        };

        /// An initial value problem dy/dt = f(t, y), y(t0) = y0.
        struct ODEProblem
        {
            std::string name;    ///< Model as shown to the user.
            Function2D f;        ///< Right-hand side f(t, y).
            double t0;           ///< Start time.
            double y0;           ///< Initial value.
            double t_end;        ///< Suggested end time.
            double h;            ///< Suggested step size.
            Function1D exact;    ///< Exact solution y(t) for this t0 and y0.
        };

        /// A set of data points for interpolation.
        struct DataSet
        {
            std::string name;          ///< Description shown to the user.
            std::vector<double> x;     ///< Strictly increasing x values.
            std::vector<double> y;     ///< Matching y values.
            Function1D exact;          ///< Function the data was sampled from (empty if measured data).
        };

        /// A function with known first and second derivatives, for numerical differentiation.
        struct DerivativeProblem
        {
            std::string name;    ///< Function as shown to the user.
            Function1D f;        ///< f(x).
            Function1D df;       ///< Exact f'(x).
            Function1D d2f;      ///< Exact f''(x).
            double x;            ///< Suggested point of evaluation.
        };

        /// A linear boundary value problem y'' = p(x) y' + q(x) y + r(x), y(a) = alpha, y(b) = beta.
        struct BVPProblem
        {
            std::string name;    ///< Problem as shown to the user.
            Function1D p;        ///< Coefficient of y'.
            Function1D q;        ///< Coefficient of y.
            Function1D r;        ///< Source term.
            double a;            ///< Left end.
            double b;            ///< Right end.
            double alpha;        ///< y(a).
            double beta;         ///< y(b).
            Function1D exact;    ///< Exact solution y(x).
        };

        /// A square linear system A x = b.
        struct LinearSystem
        {
            std::string name;                         ///< Description shown to the user.
            std::vector<std::vector<double>> A;       ///< Coefficient matrix.
            std::vector<double> b;                    ///< Right-hand side.
        };

        /// @return The equations offered for Bisection, Newton-Raphson and the Secant method.
        const std::vector<RootProblem>& root_problems();

        /// @return The equations offered for Fixed-Point Iteration.
        const std::vector<FixedPointProblem>& fixed_point_problems();

        /// @return The integrals offered by the four integration methods (their integrands
        ///         are also offered for Chebyshev approximation).
        const std::vector<Integral>& integrals();

        /// @return The initial value problems offered by the nine ODE solvers.
        const std::vector<ODEProblem>& ode_problems();

        /// @return The data sets offered by linear interpolation and the cubic spline.
        const std::vector<DataSet>& data_sets();

        /// @return The linear systems offered by Gaussian elimination, LU decomposition and conjugate gradient.
        const std::vector<LinearSystem>& linear_systems();

        /// @return The functions offered by the numerical differentiation formulas.
        const std::vector<DerivativeProblem>& derivative_problems();

        /// @return The boundary value problems offered by the finite difference method.
        const std::vector<BVPProblem>& bvp_problems();

        /**
         * @brief Collects the names of any list of catalog entries, for a menu.
         * @tparam Entry  Any catalog struct with a @c name member.
         * @param entries  The catalog list.
         * @return The names, in the same order.
         */
        template <typename Entry>
        std::vector<std::string> names(const std::vector<Entry>& entries)
        {
            std::vector<std::string> result;
            for (const Entry& e : entries) result.push_back(e.name);
            return result;
        }
    }
}
