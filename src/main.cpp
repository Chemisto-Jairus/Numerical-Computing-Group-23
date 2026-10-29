/**
 * @file main.cpp
 * @brief Entry point of numcomp_app, the interactive Group 23 command-line application.
 * @author Nabakooza Bridget Maria (Member 9) and Chemisto Jairus (Member 1)
 *
 * Shows the GROUP 23 banner and the main menu. Each menu entry opens the
 * menu for one topic of the library (see src/app/module_menus.hpp). The
 * app runs until the user chooses Exit or the input ends.
 *
 * Build it together with the library:
 * @code
 * cmake -S . -B build
 * cmake --build build
 * ./build/numcomp_app            (Windows: build\numcomp_app.exe)
 * @endcode
 */

#include "app/console_ui.hpp"
#include "app/module_menus.hpp"
#include <iostream>

/**
 * @brief Shows the main menu until the user exits.
 * @return 0 on a normal exit.
 */
int main()
{
    try
    {
        app::ui::print_banner();

        while (true)
        {
            app::ui::print_header("MAIN MENU", "Choose a topic, then a method and a problem");
            int choice = app::ui::choose("Topics", {
                "Root finding           Bisection, Newton, Secant, Fixed-point",
                "Interpolation          Linear, Spline, Chebyshev nodes & approx.",
                "Differentiation        Forward, Backward, Central, Richardson",
                "Integration            Trapezoidal, Midpoint, Simpson, Romberg",
                "Initial value ODEs     Euler, RK, Adams-Bashforth/Moulton, RKF45",
                "Boundary value ODEs    Finite difference method",
                "Linear systems         Gaussian, LU, Conjugate gradient",
                "About this library"}, "Exit");

            switch (choice)
            {
                case 1: app::root_finding_menu(); break;
                case 2: app::interpolation_menu(); break;
                case 3: app::differentiation_menu(); break;
                case 4: app::integration_menu(); break;
                case 5: app::ode_menu(); break;
                case 6: app::bvp_menu(); break;
                case 7: app::linear_systems_menu(); break;
                case 8: app::about_screen(); break;
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
