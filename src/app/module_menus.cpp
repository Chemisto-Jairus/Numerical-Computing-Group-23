/**
 * @file module_menus.cpp
 * @brief Implementation of the seven topic menus and the About screen.
 * @author Nabakooza Bridget Maria (Member 9)
 */

#include "module_menus.hpp"
#include "catalog.hpp"
#include "console_ui.hpp"
#include "numerical-computing/basic_root_finder.hpp"
#include "numerical-computing/advanced_root_finder.hpp"
#include "numerical-computing/interpolator.hpp"
#include "numerical-computing/cubic_spline.hpp"
#include "numerical-computing/basic_integrator.hpp"
#include "numerical-computing/advanced_integrator.hpp"
#include "numerical-computing/basic_ode_solver.hpp"
#include "numerical-computing/advanced_ode_solver.hpp"
#include "numerical-computing/matrix_solver.hpp"
#include "numerical-computing/numerical_differentiator.hpp"
#include "numerical-computing/conjugate_gradient_solver.hpp"
#include "numerical-computing/adams_bashforth_solver.hpp"
#include "numerical-computing/adams_moulton_solver.hpp"
#include "numerical-computing/runge_kutta_fehlberg_solver.hpp"
#include "numerical-computing/finite_difference_solver.hpp"
#include <algorithm>
#include <cmath>
#include <limits>
#include <stdexcept>
#include <string>
#include <vector>

namespace app
{
    using catalog::Function1D;
    using catalog::Function2D;

    /// Marks "no exact value known" in the result helpers.
    static const double UNKNOWN = std::numeric_limits<double>::quiet_NaN();

    /**
     * @brief Runs one calculation and turns any library exception into a message.
     *
     * The handlers are ordered from the most specific exception type to the
     * most general, and app::ui::InputEnded is not caught here, so ending
     * the input still leaves the app.
     *
     * @tparam Code  Any callable taking no arguments.
     * @param code  The calculation to run.
     */
    template <typename Code>
    static void run_safely(Code code)
    {
        try
        {
            code();
        }
        catch (const std::invalid_argument& e)
        {
            ui::print_error("Invalid input", e.what());
        }
        catch (const std::out_of_range& e)
        {
            ui::print_error("Out of range", e.what());
        }
        catch (const std::runtime_error& e)
        {
            ui::print_error("Calculation failed", e.what());
        }
        catch (const std::exception& e)
        {
            ui::print_error("Unexpected error", e.what());
        }
    }

    /**
     * @brief Prints a result, compared with the exact value when one is known.
     * @param label   Description of the result.
     * @param approx  Numerical result.
     * @param exact   Exact value, or UNKNOWN.
     */
    static void show_result(const std::string& label, double approx, double exact)
    {
        if (std::isnan(exact))
        {
            ui::print_value(label, approx, 12);
        }
        else
        {
            ui::print_compare(label, approx, exact);
        }
    }

    /**
     * @brief Wraps f so that every call increments a counter.
     * @param f      The function to wrap.
     * @param calls  Counter increased on every evaluation.
     * @return A function with the same values as f.
     */
    static Function1D counted(const Function1D& f, int& calls)
    {
        return [&calls, f](double x) { ++calls; return f(x); };
    }

    // =====================================================================
    // Root finding
    // =====================================================================

    /**
     * @brief Lets the user pick a preset equation f(x) = 0 or type a cubic.
     * @param problem  Receives the chosen equation. For a custom cubic the root is UNKNOWN.
     * @return false if the user chose "Back".
     */
    static bool choose_root_problem(catalog::RootProblem& problem)
    {
        std::vector<std::string> options = catalog::names(catalog::root_problems());
        options.push_back("Type your own cubic: a x^3 + b x^2 + c x + d = 0");
        int pick = ui::choose("Which equation?", options);
        if (pick == 0) return false;

        if (pick <= static_cast<int>(catalog::root_problems().size()))
        {
            problem = catalog::root_problems()[pick - 1];
            return true;
        }

        double a = ui::read_double("a", 1.0);
        double b = ui::read_double("b", 0.0);
        double c = ui::read_double("c", -1.0);
        double d = ui::read_double("d", -2.0);
        problem.name = "custom cubic";
        problem.f  = [=](double x) { return ((a * x + b) * x + c) * x + d; };
        problem.df = [=](double x) { return (3.0 * a * x + 2.0 * b) * x + c; };
        problem.a = 0.0;
        problem.b = 2.0;
        problem.x0 = 1.0;
        problem.root = UNKNOWN;
        return true;
    }

    /**
     * @brief Shows a root, its known value (if any) and the residual f(root).
     * @param label    Method name.
     * @param root     Root returned by the method.
     * @param problem  The equation that was solved.
     * @param calls    Number of evaluations of f (and f').
     */
    static void show_root(const std::string& label, double root, const catalog::RootProblem& problem, int calls)
    {
        show_result(label, root, problem.root);
        ui::print_value("Residual f(root)", problem.f(root), 16);
        ui::print_value("Function evaluations", calls, 0);
    }

    void root_finding_menu()
    {
        numcomp::BasicRootFinder basic;
        numcomp::AdvancedRootFinder advanced;

        while (true)
        {
            ui::print_header("ROOT FINDING", "Solve f(x) = 0");
            int method = ui::choose("Choose a method", {
                "Bisection            - reliable, needs f(a) and f(b) of opposite sign",
                "Newton-Raphson       - fastest near the root, uses f'(x)",
                "Secant               - fast, needs two starting guesses, no derivative",
                "Fixed-point          - solves x = g(x)",
                "Compare Bisection, Newton-Raphson and Secant on one equation"}, "Back to main menu");
            if (method == 0) return;

            if (method == 4)
            {
                int pick = ui::choose("Which equation x = g(x)?", catalog::names(catalog::fixed_point_problems()));
                if (pick == 0) continue;
                const catalog::FixedPointProblem& p = catalog::fixed_point_problems()[pick - 1];
                double x0 = ui::read_double("Starting guess x0", p.x0);
                double tol = ui::read_double("Tolerance (> 0)", 1e-10);
                int max_iter = ui::read_int("Maximum iterations (1 to 100000) ", 200, 1, 100000);
                run_safely([&] {
                    double x = advanced.fixed_point_iteration(p.g, x0, tol, max_iter);
                    show_result("Fixed point", x, p.fixed_point);
                    ui::print_value("Check: g(x) - x", p.g(x) - x, 16);
                });
                continue;
            }

            catalog::RootProblem problem;
            if (!choose_root_problem(problem)) continue;
            double tol = ui::read_double("Tolerance (> 0)", 1e-10);
            int max_iter = ui::read_int("Maximum iterations (1 to 100000) ", 200, 1, 100000);

            if (method == 1 || method == 5)
            {
                double a = (method == 1) ? ui::read_double("Bracket left end a", problem.a) : problem.a;
                double b = (method == 1) ? ui::read_double("Bracket right end b", problem.b) : problem.b;
                run_safely([&] {
                    int calls = 0;
                    double r = basic.bisection(counted(problem.f, calls), a, b, tol, max_iter);
                    show_root("Bisection", r, problem, calls);
                });
            }
            if (method == 2 || method == 5)
            {
                double x0 = (method == 2) ? ui::read_double("Starting guess x0", problem.x0) : problem.x0;
                run_safely([&] {
                    int calls = 0;
                    double r = basic.newton_raphson(counted(problem.f, calls), counted(problem.df, calls), x0, tol, max_iter);
                    show_root("Newton-Raphson", r, problem, calls);
                });
            }
            if (method == 3 || method == 5)
            {
                double x0 = (method == 3) ? ui::read_double("First guess x0", problem.a) : problem.a;
                double x1 = (method == 3) ? ui::read_double("Second guess x1", problem.b) : problem.b;
                run_safely([&] {
                    int calls = 0;
                    double r = advanced.secant_method(counted(problem.f, calls), x0, x1, tol, max_iter);
                    show_root("Secant", r, problem, calls);
                });
            }
        }
    }

    // =====================================================================
    // Interpolation
    // =====================================================================

    /**
     * @brief Lets the user pick a preset data set or type their own points.
     * @param data  Receives the chosen data.
     * @return false if the user chose "Back".
     */
    static bool choose_data(catalog::DataSet& data)
    {
        std::vector<std::string> options = catalog::names(catalog::data_sets());
        options.push_back("Type your own data points");
        int pick = ui::choose("Which data?", options);
        if (pick == 0) return false;

        if (pick <= static_cast<int>(catalog::data_sets().size()))
        {
            data = catalog::data_sets()[pick - 1];
            return true;
        }

        data = catalog::DataSet{"your data", {}, {}, nullptr};
        int n = ui::read_int("How many points (2 to 20) ", 4, 2, 20);
        ui::print_info("Type the x values in increasing order.");
        for (int i = 0; i < n; ++i)
        {
            data.x.push_back(ui::read_double("x" + std::to_string(i + 1), i));
            data.y.push_back(ui::read_double("y" + std::to_string(i + 1), 0.0));
        }
        return true;
    }

    /**
     * @brief Approximates a chosen function by a Chebyshev series and tabulates the error.
     * @param interp  The interpolator that computes and evaluates the series.
     */
    static void chebyshev_approximation_screen(numcomp::Interpolator& interp)
    {
        std::vector<std::string> options;
        for (const catalog::Integral& i : catalog::integrals()) options.push_back("f(x) = " + i.name);
        int pick = ui::choose("Which function?", options);
        if (pick == 0) return;
        const catalog::Integral& fn = catalog::integrals()[pick - 1];

        double a = ui::read_double("Left end a", fn.a);
        double b = ui::read_double("Right end b", fn.b);
        int n = ui::read_int("Number of terms n (1 to 1000) ", 10, 1, 1000);
        run_safely([&] {
            std::vector<double> c = interp.chebyshev_coefficients(fn.f, a, b, n);
            ui::print_table_header({"x", "series", "f(x)", "error"});
            double worst = 0.0;
            for (int k = 0; k <= 8; ++k)
            {
                double x = a + (b - a) * k / 8.0;
                double approx = interp.chebyshev_approximate(c, a, b, x);
                double exact = fn.f(x);
                worst = std::max(worst, std::abs(approx - exact));
                ui::print_table_row({x, approx, exact, std::abs(approx - exact)});
            }
            ui::print_value("Largest error in the table", worst, 16);
            ui::print_value("Last coefficient c_(n-1)", c.back(), 16);
        });
    }

    void interpolation_menu()
    {
        numcomp::Interpolator interp;
        numcomp::CubicSpline spline;

        while (true)
        {
            ui::print_header("INTERPOLATION", "Estimate values between known data points");
            int method = ui::choose("Choose a method", {
                "Linear interpolation   - straight lines between neighbouring points",
                "Natural cubic spline   - smooth curve through every point",
                "Compare linear and spline at several points",
                "Chebyshev nodes        - well-placed sample points on [a, b]",
                "Chebyshev approx.      - replace f(x) by a short Chebyshev series"}, "Back to main menu");
            if (method == 0) return;

            if (method == 5)
            {
                chebyshev_approximation_screen(interp);
                continue;
            }

            if (method == 4)
            {
                double a = ui::read_double("Left end a", -1.0);
                double b = ui::read_double("Right end b", 1.0);
                int n = ui::read_int("Number of nodes (1 to 1000000) ", 8, 1, 1000000);
                run_safely([&] {
                    std::vector<double> nodes = interp.chebyshev_nodes(a, b, n);
                    ui::print_table_header({"k", "node x_k"});
                    for (std::size_t k = 0; k < nodes.size() && k < 20; ++k) ui::print_table_row({static_cast<double>(k + 1), nodes[k]});
                    if (nodes.size() > 20) ui::print_info("(first 20 of " + std::to_string(nodes.size()) + " nodes shown)");
                });
                continue;
            }

            catalog::DataSet data;
            if (!choose_data(data)) continue;

            if (method == 3)
            {
                run_safely([&] {
                    double lo = data.x.front(), hi = data.x.back();
                    // The exact column is shown only when the data came from a known function.
                    ui::print_table_header(data.exact ? std::vector<std::string>{"x", "linear", "spline", "exact"}
                                                      : std::vector<std::string>{"x", "linear", "spline"});
                    for (int k = 0; k <= 8; ++k)
                    {
                        double x = lo + (hi - lo) * (k + 0.5) / 9.0;
                        std::vector<double> row = {x, interp.linear_interpolate(x, data.x, data.y), spline.interpolate(x, data.x, data.y)};
                        if (data.exact) row.push_back(data.exact(x));
                        ui::print_table_row(row);
                    }
                });
                continue;
            }

            do
            {
                double x = ui::read_double("Estimate y at x", 0.5 * (data.x[0] + data.x[1]));
                run_safely([&] {
                    double exact = data.exact ? data.exact(x) : UNKNOWN;
                    if (method == 1) show_result("Linear interpolation", interp.linear_interpolate(x, data.x, data.y), exact);
                    else             show_result("Cubic spline", spline.interpolate(x, data.x, data.y), exact);
                });
            } while (ui::ask_yes_no("Estimate at another x?", true));
        }
    }

    // =====================================================================
    // Integration
    // =====================================================================

    /**
     * @brief Lets the user pick a preset integrand or type a cubic polynomial.
     * @param integral  Receives the chosen integrand and its antiderivative.
     * @return false if the user chose "Back".
     */
    static bool choose_integral(catalog::Integral& integral)
    {
        std::vector<std::string> options;
        for (const catalog::Integral& i : catalog::integrals()) options.push_back("f(x) = " + i.name);
        options.push_back("Type your own cubic: f(x) = a x^3 + b x^2 + c x + d");
        int pick = ui::choose("Which function?", options);
        if (pick == 0) return false;

        if (pick <= static_cast<int>(catalog::integrals().size()))
        {
            integral = catalog::integrals()[pick - 1];
            return true;
        }

        double a = ui::read_double("a", 1.0);
        double b = ui::read_double("b", 0.0);
        double c = ui::read_double("c", 0.0);
        double d = ui::read_double("d", 0.0);
        integral.name = "custom cubic";
        integral.f = [=](double x) { return ((a * x + b) * x + c) * x + d; };
        integral.antiderivative = [=](double x) { return (((a / 4.0 * x + b / 3.0) * x + c / 2.0) * x + d) * x; };
        integral.a = 0.0;
        integral.b = 1.0;
        return true;
    }

    void integration_menu()
    {
        numcomp::BasicIntegrator basic;
        numcomp::AdvancedIntegrator advanced;

        while (true)
        {
            ui::print_header("NUMERICAL INTEGRATION", "Approximate the area under f(x) from a to b");
            int method = ui::choose("Choose a method", {
                "Trapezoidal rule     - straight lines, error ~ h^2",
                "Midpoint rule        - centre of each strip, error ~ h^2",
                "Simpson's 1/3 rule   - parabolas, error ~ h^4 (n must be even)",
                "Romberg integration  - extrapolated trapezoids, very accurate",
                "Compare all four methods on one integral"}, "Back to main menu");
            if (method == 0) return;

            catalog::Integral integral;
            if (!choose_integral(integral)) continue;
            double a = ui::read_double("Lower limit a", integral.a);
            double b = ui::read_double("Upper limit b", integral.b);
            int n = (method != 4) ? ui::read_int("Number of strips n (1 to 10000000) ", 100, 1, 10000000) : 100;
            int rows = (method == 4 || method == 5) ? ui::read_int("Romberg rows (1 to 25) ", 6, 1, 25) : 6;

            run_safely([&] {
                double exact = integral.antiderivative(b) - integral.antiderivative(a);
                if (std::isnan(exact) || std::isinf(exact)) exact = UNKNOWN;
                if (method == 1 || method == 5) show_result("Trapezoidal", basic.trapezoidal_rule(integral.f, a, b, n), exact);
                if (method == 2 || method == 5) show_result("Midpoint", basic.midpoint_rule(integral.f, a, b, n), exact);
                if (method == 3 || method == 5) show_result("Simpson's 1/3", advanced.simpsons_rule(integral.f, a, b, n % 2 == 0 || method == 3 ? n : n + 1), exact);
                if (method == 4 || method == 5) show_result("Romberg", advanced.romberg_integration(integral.f, a, b, rows), exact);
                if (method == 5 && n % 2 != 0) ui::print_info("(Simpson used n + 1 strips because n must be even.)");
            });
        }
    }

    // =====================================================================
    // Ordinary differential equations
    // =====================================================================

    /**
     * @brief Lets the user pick a preset model or type a linear ODE.
     * @param problem  Receives the chosen initial value problem.
     * @return false if the user chose "Back".
     */
    static bool choose_ode(catalog::ODEProblem& problem)
    {
        std::vector<std::string> options = catalog::names(catalog::ode_problems());
        options.push_back("Type your own: dy/dt = p y + q t + r, y(t0) = y0");
        int pick = ui::choose("Which equation?", options);
        if (pick == 0) return false;

        if (pick <= static_cast<int>(catalog::ode_problems().size()))
        {
            problem = catalog::ode_problems()[pick - 1];
            return true;
        }

        double p = ui::read_double("p", -1.0);
        double q = ui::read_double("q", 1.0);
        double r = ui::read_double("r", 0.0);
        problem.name = "custom linear ODE";
        problem.f = [=](double t, double y) { return p * y + q * t + r; };
        problem.t0 = ui::read_double("Start time t0", 0.0);
        problem.y0 = ui::read_double("Initial value y0", 1.0);
        problem.t_end = problem.t0 + 1.0;
        problem.h = 0.1;
        problem.exact = nullptr;
        return true;
    }

    /// Names of the fixed-step ODE methods, in the order used by solve_ode().
    static const std::vector<std::string> ODE_METHODS = {"Euler", "Heun", "RK3", "RK4", "AB2", "AB4", "AM2", "AM4"};

    /**
     * @brief Solves an initial value problem with one of the eight fixed-step methods.
     * @param method   1 = Euler, 2 = Heun, 3 = RK3, 4 = RK4, 5 = AB2, 6 = AB4, 7 = AM2, 8 = AM4.
     * @param problem  The problem; t_end and h are taken from it.
     * @return The solution curve.
     */
    static std::vector<std::pair<double, double>> solve_ode(int method, const catalog::ODEProblem& problem)
    {
        numcomp::BasicODESolver basic;
        numcomp::AdvancedODESolver advanced;
        numcomp::AdamsBashforthSolver bashforth;
        numcomp::AdamsMoultonSolver moulton;
        const catalog::ODEProblem& p = problem;
        switch (method)
        {
            case 1:  return basic.euler(p.f, p.t0, p.y0, p.t_end, p.h);
            case 2:  return basic.heun(p.f, p.t0, p.y0, p.t_end, p.h);
            case 3:  return advanced.rk3(p.f, p.t0, p.y0, p.t_end, p.h);
            case 4:  return advanced.rk4(p.f, p.t0, p.y0, p.t_end, p.h);
            case 5:  return bashforth.ab2(p.f, p.t0, p.y0, p.t_end, p.h);
            case 6:  return bashforth.ab4(p.f, p.t0, p.y0, p.t_end, p.h);
            case 7:  return moulton.am2(p.f, p.t0, p.y0, p.t_end, p.h);
            default: return moulton.am4(p.f, p.t0, p.y0, p.t_end, p.h);
        }
    }

    /**
     * @brief Prints about ten evenly spread points of a solution curve, always including the last.
     * @param curve     The (t, y) points.
     * @param y_column  Heading of the y column.
     * @param exact     Exact solution, or empty if unknown.
     * @param show_h    Also print the step that led to each point (for the adaptive method).
     */
    static void print_curve(const std::vector<std::pair<double, double>>& curve, const std::string& y_column,
                            const Function1D& exact, bool show_h)
    {
        // The exact and error columns are shown only when the exact solution is known.
        std::vector<std::string> columns = {"t", y_column};
        if (exact) { columns.push_back("exact y"); columns.push_back("error"); }
        if (show_h) columns.push_back("step h");
        ui::print_table_header(columns);

        std::size_t stride = std::max<std::size_t>(1, curve.size() / 10);
        for (std::size_t i = 0; i < curve.size(); i += stride)
        {
            if (i + stride >= curve.size()) i = curve.size() - 1;
            std::vector<double> row = {curve[i].first, curve[i].second};
            if (exact)
            {
                double e = exact(curve[i].first);
                row.push_back(e);
                row.push_back(std::abs(curve[i].second - e));
            }
            if (show_h) row.push_back(i == 0 ? 0.0 : curve[i].first - curve[i-1].first);
            ui::print_table_row(row);
        }
        ui::print_value("Points computed", static_cast<double>(curve.size()), 0);
    }

    void ode_menu()
    {
        numcomp::RungeKuttaFehlbergSolver rkf;

        while (true)
        {
            ui::print_header("ORDINARY DIFFERENTIAL EQUATIONS", "Solve dy/dt = f(t, y) from t0 to t_end");
            int family = ui::choose("Choose a family of methods", {
                "One-step methods     - Euler, Heun, RK3, RK4 (fixed step h)",
                "Multistep methods    - Adams-Bashforth 2 and 4, Adams-Moulton 2 and 4",
                "Adaptive method      - Runge-Kutta-Fehlberg 4(5), picks its own steps",
                "Compare all nine methods on one equation"}, "Back to main menu");
            if (family == 0) return;

            int method = 0;   // index into ODE_METHODS (1-based); 9 = RKF45; 10 = compare all
            if (family == 1)
            {
                method = ui::choose("Choose a one-step method", {
                    "Euler    - 1 slope per step, error ~ h",
                    "Heun     - 2 slopes per step, error ~ h^2",
                    "RK3      - 3 slopes per step, error ~ h^3",
                    "RK4      - 4 slopes per step, error ~ h^4"});
                if (method == 0) continue;
            }
            else if (family == 2)
            {
                int pick = ui::choose("Choose a multistep method", {
                    "Adams-Bashforth 2  - explicit, reuses 2 slopes, error ~ h^2",
                    "Adams-Bashforth 4  - explicit, reuses 4 slopes, error ~ h^4",
                    "Adams-Moulton 2    - implicit trapezoidal rule, error ~ h^2",
                    "Adams-Moulton 4    - implicit, 3 old slopes + the new one, error ~ h^4"});
                if (pick == 0) continue;
                method = pick + 4;
            }
            else
            {
                method = (family == 3) ? 9 : 10;
            }

            catalog::ODEProblem problem;
            if (!choose_ode(problem)) continue;
            problem.t_end = ui::read_double("End time t_end", problem.t_end);
            problem.h = ui::read_double(method == 9 ? "First step size h" : "Step size h (at most 1,000,000 steps)", problem.h);
            double tol = (method >= 9) ? ui::read_double("RKF45 tolerance (error per unit time, > 0)", 1e-8) : 1e-8;

            run_safely([&] {
                if (method == 10)
                {
                    double exact = problem.exact ? problem.exact(problem.t_end) : UNKNOWN;
                    for (int m = 1; m <= 8; ++m)
                    {
                        // Each method runs in its own handler, so one failure does not hide the others.
                        run_safely([&] { show_result(ODE_METHODS[m - 1] + " y(t_end)", solve_ode(m, problem).back().second, exact); });
                    }
                    show_result("RKF45 y(t_end)", rkf.rkf45(problem.f, problem.t0, problem.y0, problem.t_end, problem.h, tol).back().second, exact);
                    return;
                }
                if (method == 9)
                {
                    print_curve(rkf.rkf45(problem.f, problem.t0, problem.y0, problem.t_end, problem.h, tol), "RKF45 y", problem.exact, true);
                    return;
                }
                print_curve(solve_ode(method, problem), ODE_METHODS[method - 1] + " y", problem.exact, false);
            });
        }
    }

    // =====================================================================
    // Linear systems
    // =====================================================================

    /**
     * @brief Lets the user pick a preset system or type one of size 2 to 6.
     * @param system  Receives the chosen system.
     * @return false if the user chose "Back".
     */
    static bool choose_system(catalog::LinearSystem& system)
    {
        std::vector<std::string> options = catalog::names(catalog::linear_systems());
        options.push_back("Type your own system (2 to 6 unknowns)");
        int pick = ui::choose("Which system?", options);
        if (pick == 0) return false;

        if (pick <= static_cast<int>(catalog::linear_systems().size()))
        {
            system = catalog::linear_systems()[pick - 1];
            return true;
        }

        int n = ui::read_int("Number of unknowns (2 to 6) ", 3, 2, 6);
        system = catalog::LinearSystem{"your system", std::vector<std::vector<double>>(n, std::vector<double>(n, 0.0)), std::vector<double>(n, 0.0)};
        for (int i = 0; i < n; ++i)
        {
            ui::print_info("Row " + std::to_string(i + 1) + ":");
            for (int j = 0; j < n; ++j)
            {
                system.A[i][j] = ui::read_double("  A[" + std::to_string(i + 1) + "][" + std::to_string(j + 1) + "]", i == j ? 1.0 : 0.0);
            }
            system.b[i] = ui::read_double("  b[" + std::to_string(i + 1) + "]", 0.0);
        }
        return true;
    }

    /**
     * @brief Prints a solution vector and the size of the residual A x - b.
     * @param label   Method name.
     * @param system  The system that was solved.
     * @param x       The computed solution.
     */
    static void show_solution(const std::string& label, const catalog::LinearSystem& system, const std::vector<double>& x)
    {
        ui::print_info(label + ":");
        double worst = 0.0;
        for (std::size_t i = 0; i < x.size(); ++i)
        {
            ui::print_value("  x" + std::to_string(i + 1), x[i], 12);
            double r = -system.b[i];
            for (std::size_t j = 0; j < x.size(); ++j) r += system.A[i][j] * x[j];
            worst = std::max(worst, std::abs(r));
        }
        ui::print_value("  Largest residual |A x - b|", worst, 16);
    }

    void linear_systems_menu()
    {
        numcomp::MatrixSolver solver;
        numcomp::ConjugateGradientSolver cg;

        while (true)
        {
            ui::print_header("LINEAR SYSTEMS", "Solve A x = b");
            int method = ui::choose("Choose a method", {
                "Gaussian elimination with partial pivoting   (direct, any matrix)",
                "LU decomposition with partial pivoting       (direct, any matrix)",
                "Conjugate gradient   (iterative, symmetric positive definite only)",
                "Compare all three methods on one system"}, "Back to main menu");
            if (method == 0) return;

            catalog::LinearSystem system;
            if (!choose_system(system)) continue;
            double tol = 1e-12;
            int max_iter = 1000;
            if (method == 3)
            {
                tol = ui::read_double("CG tolerance (relative residual, > 0)", tol);
                max_iter = ui::read_int("CG maximum iterations (1 to 100000) ", max_iter, 1, 100000);
            }

            // Separate handlers: in "compare", CG can reject a matrix the direct methods accept.
            if (method == 1 || method == 4) run_safely([&] { show_solution("Gaussian elimination", system, solver.gaussian_elimination(system.A, system.b)); });
            if (method == 2 || method == 4) run_safely([&] { show_solution("LU decomposition", system, solver.lu_solve(system.A, system.b)); });
            if (method == 3 || method == 4) run_safely([&] { show_solution("Conjugate gradient", system, cg.solve(system.A, system.b, tol, max_iter)); });
        }
    }

    // =====================================================================
    // Numerical differentiation
    // =====================================================================

    /**
     * @brief Lets the user pick a preset function or type a cubic polynomial.
     * @param problem  Receives the chosen function and its exact derivatives.
     * @return false if the user chose "Back".
     */
    static bool choose_derivative_problem(catalog::DerivativeProblem& problem)
    {
        std::vector<std::string> options;
        for (const catalog::DerivativeProblem& d : catalog::derivative_problems()) options.push_back("f(x) = " + d.name);
        options.push_back("Type your own cubic: f(x) = a x^3 + b x^2 + c x + d");
        int pick = ui::choose("Which function?", options);
        if (pick == 0) return false;

        if (pick <= static_cast<int>(catalog::derivative_problems().size()))
        {
            problem = catalog::derivative_problems()[pick - 1];
            return true;
        }

        double a = ui::read_double("a", 1.0);
        double b = ui::read_double("b", 0.0);
        double c = ui::read_double("c", 0.0);
        double d = ui::read_double("d", 0.0);
        problem.name = "custom cubic";
        problem.f   = [=](double x) { return ((a * x + b) * x + c) * x + d; };
        problem.df  = [=](double x) { return (3.0 * a * x + 2.0 * b) * x + c; };
        problem.d2f = [=](double x) { return 6.0 * a * x + 2.0 * b; };
        problem.x = 1.0;
        return true;
    }

    void differentiation_menu()
    {
        numcomp::NumericalDifferentiator diff;

        while (true)
        {
            ui::print_header("NUMERICAL DIFFERENTIATION", "Approximate f'(x) and f''(x) from values of f");
            int method = ui::choose("Choose a formula", {
                "Forward difference    (f(x+h) - f(x)) / h,          error ~ h",
                "Backward difference   (f(x) - f(x-h)) / h,          error ~ h",
                "Central difference    (f(x+h) - f(x-h)) / 2h,       error ~ h^2",
                "Second derivative     (f(x+h) - 2f(x) + f(x-h)) / h^2",
                "Richardson extrapolation of the central difference, error ~ h^4",
                "Compare all formulas, and watch the error as h shrinks"}, "Back to main menu");
            if (method == 0) return;

            catalog::DerivativeProblem problem;
            if (!choose_derivative_problem(problem)) continue;
            double x = ui::read_double("Point x", problem.x);

            if (method == 6)
            {
                run_safely([&] {
                    double d1 = problem.df(x), d2 = problem.d2f(x);
                    ui::print_table_header({"h", "forward", "central", "Richardson", "second"});
                    for (double h : {1e-1, 1e-2, 1e-3, 1e-4, 1e-5, 1e-6})
                    {
                        ui::print_table_row({h, std::abs(diff.forward_difference(problem.f, x, h) - d1),
                                                std::abs(diff.central_difference(problem.f, x, h) - d1),
                                                std::abs(diff.richardson_extrapolation(problem.f, x, h) - d1),
                                                std::abs(diff.second_derivative(problem.f, x, h) - d2)});
                    }
                    ui::print_info("Absolute errors. Each error first falls with h, then rounding takes over.");
                });
                continue;
            }

            double h = ui::read_double("Step size h (> 0)", method == 4 ? 1e-3 : 1e-4);
            run_safely([&] {
                switch (method)
                {
                    case 1:  show_result("Forward f'(x)", diff.forward_difference(problem.f, x, h), problem.df(x)); break;
                    case 2:  show_result("Backward f'(x)", diff.backward_difference(problem.f, x, h), problem.df(x)); break;
                    case 3:  show_result("Central f'(x)", diff.central_difference(problem.f, x, h), problem.df(x)); break;
                    case 4:  show_result("Second f''(x)", diff.second_derivative(problem.f, x, h), problem.d2f(x)); break;
                    default: show_result("Richardson f'(x)", diff.richardson_extrapolation(problem.f, x, h), problem.df(x)); break;
                }
            });
        }
    }

    // =====================================================================
    // Boundary value problems
    // =====================================================================

    /**
     * @brief Lets the user pick a preset problem or type one with constant coefficients.
     * @param problem  Receives the chosen problem. For a custom problem the exact solution is empty.
     * @return false if the user chose "Back".
     */
    static bool choose_bvp(catalog::BVPProblem& problem)
    {
        std::vector<std::string> options = catalog::names(catalog::bvp_problems());
        options.push_back("Type your own: y'' = p y' + q y + r (constants), y(a), y(b)");
        int pick = ui::choose("Which problem?", options);
        if (pick == 0) return false;

        if (pick <= static_cast<int>(catalog::bvp_problems().size()))
        {
            problem = catalog::bvp_problems()[pick - 1];
            return true;
        }

        double p = ui::read_double("p", 0.0);
        double q = ui::read_double("q (q >= 0 keeps the system solvable)", 1.0);
        double r = ui::read_double("r", 0.0);
        problem.name = "custom problem";
        problem.p = [=](double) { return p; };
        problem.q = [=](double) { return q; };
        problem.r = [=](double) { return r; };
        problem.a = ui::read_double("Left end a", 0.0);
        problem.b = ui::read_double("Right end b", 1.0);
        problem.alpha = ui::read_double("y(a)", 0.0);
        problem.beta = ui::read_double("y(b)", 1.0);
        problem.exact = nullptr;
        return true;
    }

    /**
     * @brief Largest difference between a grid solution and the exact solution.
     * @param grid   Grid points (x_i, y_i).
     * @param exact  Exact solution.
     * @return max_i |y_i - exact(x_i)|.
     */
    static double max_grid_error(const std::vector<std::pair<double, double>>& grid, const Function1D& exact)
    {
        double worst = 0.0;
        for (const std::pair<double, double>& point : grid) worst = std::max(worst, std::abs(point.second - exact(point.first)));
        return worst;
    }

    void bvp_menu()
    {
        numcomp::FiniteDifferenceSolver fd;

        while (true)
        {
            ui::print_header("BOUNDARY VALUE PROBLEMS", "Finite differences for y'' = p y' + q y + r");
            int method = ui::choose("Choose what to do", {
                "Solve on a grid and show the solution",
                "Convergence study: n = 10, 20, 40, 80 (error ~ h^2)"}, "Back to main menu");
            if (method == 0) return;

            catalog::BVPProblem problem;
            if (!choose_bvp(problem)) continue;

            if (method == 2)
            {
                if (!problem.exact)
                {
                    ui::print_info("The convergence study needs a problem with a known exact solution.");
                    continue;
                }
                run_safely([&] {
                    ui::print_table_header({"n", "max error", "ratio"});
                    double previous = UNKNOWN;
                    for (int n : {10, 20, 40, 80})
                    {
                        double e = max_grid_error(fd.linear_bvp(problem.p, problem.q, problem.r, problem.a, problem.b, problem.alpha, problem.beta, n), problem.exact);
                        ui::print_table_row({static_cast<double>(n), e, std::isnan(previous) ? 0.0 : previous / e});
                        previous = e;
                    }
                    ui::print_info("A ratio near 4 confirms second-order accuracy.");
                });
                continue;
            }

            int n = ui::read_int("Number of grid intervals n (2 to 1000000) ", 20, 2, 1000000);
            run_safely([&] {
                std::vector<std::pair<double, double>> grid = fd.linear_bvp(problem.p, problem.q, problem.r, problem.a, problem.b, problem.alpha, problem.beta, n);
                std::vector<std::string> columns = {"x", "y (grid)"};
                if (problem.exact) { columns.push_back("exact y"); columns.push_back("error"); }
                ui::print_table_header(columns);
                std::size_t stride = std::max<std::size_t>(1, grid.size() / 10);
                for (std::size_t i = 0; i < grid.size(); i += stride)
                {
                    if (i + stride >= grid.size()) i = grid.size() - 1;
                    std::vector<double> row = {grid[i].first, grid[i].second};
                    if (problem.exact)
                    {
                        row.push_back(problem.exact(grid[i].first));
                        row.push_back(std::abs(grid[i].second - problem.exact(grid[i].first)));
                    }
                    ui::print_table_row(row);
                }
                if (problem.exact) ui::print_value("Largest error on the grid", max_grid_error(grid, problem.exact), 16);
            });
        }
    }

    // =====================================================================
    // About
    // =====================================================================

    void about_screen()
    {
        ui::print_header("ABOUT THIS LIBRARY", "Group 23 - Numerical Computing 2");
        ui::print_info("Fifteen stateless classes in namespace numcomp:");
        ui::print_info("  BasicRootFinder          bisection, newton_raphson");
        ui::print_info("  AdvancedRootFinder       secant_method, fixed_point_iteration");
        ui::print_info("  Interpolator             linear_interpolate, chebyshev_nodes,");
        ui::print_info("                           chebyshev_coefficients, chebyshev_approximate");
        ui::print_info("  CubicSpline              second_derivatives, interpolate");
        ui::print_info("  NumericalDifferentiator  forward/backward/central_difference,");
        ui::print_info("                           second_derivative, richardson_extrapolation");
        ui::print_info("  BasicIntegrator          trapezoidal_rule, midpoint_rule");
        ui::print_info("  AdvancedIntegrator       simpsons_rule, romberg_integration");
        ui::print_info("  BasicODESolver           euler, heun");
        ui::print_info("  AdvancedODESolver        rk3, rk4");
        ui::print_info("  AdamsBashforthSolver     ab2, ab4");
        ui::print_info("  AdamsMoultonSolver       am2, am4");
        ui::print_info("  RungeKuttaFehlbergSolver rkf45, rkf45_value");
        ui::print_info("  FiniteDifferenceSolver   linear_bvp, poisson");
        ui::print_info("  MatrixSolver             gaussian_elimination, lu_solve");
        ui::print_info("  ConjugateGradientSolver  solve, solve_from");
        ui::print_info("");
        ui::print_info("Errors are reported with standard exceptions:");
        ui::print_info("  invalid_argument  a parameter is outside its allowed range");
        ui::print_info("  out_of_range      a point lies outside the data or interval");
        ui::print_info("  runtime_error     the method failed (no convergence, singular matrix, ...)");
        ui::print_info("");
        ui::print_info("Limits: at most 1,000,000 ODE steps, grid intervals or Chebyshev nodes;");
        ui::print_info("1,000 Chebyshev coefficients; Romberg up to 25 rows.");
    }
}
