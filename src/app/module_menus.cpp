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

    static const double UNKNOWN = std::numeric_limits<double>::quiet_NaN();

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

    static Function1D counted(const Function1D& f, int& calls)
    {
        return [&calls, f](double x) { ++calls; return f(x); };
    }

    // =====================================================================
    // Root finding
    // =====================================================================

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
                "Chebyshev nodes        - well-placed sample points on [a, b]"}, "Back to main menu");
            if (method == 0) return;

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

    static std::vector<std::pair<double, double>> solve_ode(int method, const catalog::ODEProblem& problem)
    {
        numcomp::BasicODESolver basic;
        numcomp::AdvancedODESolver advanced;
        switch (method)
        {
            case 1:  return basic.euler(problem.f, problem.t0, problem.y0, problem.t_end, problem.h);
            case 2:  return basic.heun(problem.f, problem.t0, problem.y0, problem.t_end, problem.h);
            case 3:  return advanced.rk3(problem.f, problem.t0, problem.y0, problem.t_end, problem.h);
            default: return advanced.rk4(problem.f, problem.t0, problem.y0, problem.t_end, problem.h);
        }
    }

    void ode_menu()
    {
        const std::vector<std::string> method_names = {"Euler", "Heun", "RK3", "RK4"};

        while (true)
        {
            ui::print_header("ORDINARY DIFFERENTIAL EQUATIONS", "Solve dy/dt = f(t, y) from t0 to t_end");
            int method = ui::choose("Choose a method", {
                "Euler    - 1 slope per step, error ~ h",
                "Heun     - 2 slopes per step, error ~ h^2",
                "RK3      - 3 slopes per step, error ~ h^3",
                "RK4      - 4 slopes per step, error ~ h^4",
                "Compare all four methods on one equation"}, "Back to main menu");
            if (method == 0) return;

            catalog::ODEProblem problem;
            if (!choose_ode(problem)) continue;
            problem.t_end = ui::read_double("End time t_end", problem.t_end);
            problem.h = ui::read_double("Step size h (at most 1,000,000 steps)", problem.h);

            run_safely([&] {
                if (method == 5)
                {
                    double exact = problem.exact ? problem.exact(problem.t_end) : UNKNOWN;
                    for (int m = 1; m <= 4; ++m)
                    {
                        show_result(method_names[m - 1] + " y(t_end)", solve_ode(m, problem).back().second, exact);
                    }
                    return;
                }

                std::vector<std::pair<double, double>> curve = solve_ode(method, problem);
                // The exact and error columns are shown only when the exact solution is known.
                std::string y_col = method_names[method - 1] + " y";
                ui::print_table_header(problem.exact ? std::vector<std::string>{"t", y_col, "exact y", "error"}
                                                     : std::vector<std::string>{"t", y_col});

                // Show about ten evenly spread points, always including the last one.
                std::size_t stride = std::max<std::size_t>(1, curve.size() / 10);
                for (std::size_t i = 0; i < curve.size(); i += stride)
                {
                    if (i + stride >= curve.size()) i = curve.size() - 1;
                    std::vector<double> row = {curve[i].first, curve[i].second};
                    if (problem.exact)
                    {
                        double e = problem.exact(curve[i].first);
                        row.push_back(e);
                        row.push_back(std::abs(curve[i].second - e));
                    }
                    ui::print_table_row(row);
                }
                ui::print_value("Points computed", static_cast<double>(curve.size()), 0);
            });
        }
    }

    // =====================================================================
    // Linear systems
    // =====================================================================

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

        while (true)
        {
            ui::print_header("LINEAR SYSTEMS", "Solve A x = b");
            int method = ui::choose("Choose a method", {
                "Gaussian elimination with partial pivoting",
                "LU decomposition with partial pivoting",
                "Compare both methods on one system"}, "Back to main menu");
            if (method == 0) return;

            catalog::LinearSystem system;
            if (!choose_system(system)) continue;

            run_safely([&] {
                if (method == 1 || method == 3) show_solution("Gaussian elimination", system, solver.gaussian_elimination(system.A, system.b));
                if (method == 2 || method == 3) show_solution("LU decomposition", system, solver.lu_solve(system.A, system.b));
            });
        }
    }

    // =====================================================================
    // About
    // =====================================================================

    void about_screen()
    {
        ui::print_header("ABOUT THIS LIBRARY", "Group 23 - Numerical Computing 2");
        ui::print_info("Nine stateless classes in namespace numcomp:");
        ui::print_info("  BasicRootFinder     bisection, newton_raphson");
        ui::print_info("  AdvancedRootFinder  secant_method, fixed_point_iteration");
        ui::print_info("  Interpolator        linear_interpolate, chebyshev_nodes");
        ui::print_info("  CubicSpline         second_derivatives, interpolate");
        ui::print_info("  BasicIntegrator     trapezoidal_rule, midpoint_rule");
        ui::print_info("  AdvancedIntegrator  simpsons_rule, romberg_integration");
        ui::print_info("  BasicODESolver      euler, heun");
        ui::print_info("  AdvancedODESolver   rk3, rk4");
        ui::print_info("  MatrixSolver        gaussian_elimination, lu_solve");
        ui::print_info("");
        ui::print_info("Errors are reported with standard exceptions:");
        ui::print_info("  invalid_argument  a parameter is outside its allowed range");
        ui::print_info("  out_of_range      an interpolation point lies outside the data");
        ui::print_info("  runtime_error     the method failed (no convergence, singular matrix)");
        ui::print_info("");
        ui::print_info("Limits: at most 1,000,000 ODE steps or Chebyshev nodes; Romberg up to 25 rows.");
    }
}
