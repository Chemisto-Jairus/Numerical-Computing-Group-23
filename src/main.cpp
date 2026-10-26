// Group 23: Numerical Computing command-line app
// Owners: Nabakooza Bridget Maria (Member 9) with Chemisto Jairus (Member 1)
//
// Every module of the library is reachable from the menu.
// The user types the inputs. Every number is checked before it is used,
// the input limits are shown, and every library call is inside try/catch.
//
// HELPERS: the input reading and result printing that every menu option
// needs are written once (read_number, read_root_settings, read_limits,
// print_integral, read_ode_settings, print_ode_result, read_system,
// print_solution). Each menu option is now only a few lines.

#include "app/console_ui.hpp"
#include "app/module_menus.hpp"
#include <iostream>

int main()
{
    try
    {
        app::ui::print_banner();

        while (true)
        {
            app::ui::print_header("MAIN MENU", "Choose a topic, then a method and a problem");
            int choice = app::ui::choose("Topics", {
                "Root finding            Bisection, Newton-Raphson, Secant, Fixed-point",
                "Interpolation           Linear, Cubic spline, Chebyshev nodes",
                "Numerical integration   Trapezoidal, Midpoint, Simpson, Romberg",
                "Differential equations  Euler, Heun, RK3, RK4",
                "Linear systems          Gaussian elimination, LU decomposition",
                "About this library"}, "Exit");

            switch (choice)
            {
                case 1: app::root_finding_menu(); break;
                case 2: app::interpolation_menu(); break;
                case 3: app::integration_menu(); break;
                case 4: app::ode_menu(); break;
                case 5: app::linear_systems_menu(); break;
                case 6: app::about_screen(); break;
                default:
                    std::cout << "\n  Thank you for using the Group 23 numerical computing library. Goodbye!\n";
                    return 0;
            }
        }
    }
    catch (const app::ui::InputEnded&)
    {
        std::cout << "\n  Input ended. Goodbye!\n";
    }
    return 0;
}
