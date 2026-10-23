#pragma once
/**
 * @file example_helpers.hpp
 * @brief Shared output and error-handling helpers for the example programs in examples/.
 * @author Muwanguzi Samuel (Member 7)
 *
 * Every example prints a title, groups its work into sections, and shows
 * results next to the exact answer where one is known. Each section runs
 * inside demo::section(), which catches any exception, so one failing
 * section cannot stop the rest of the demo. demo::finish() returns the
 * exit code: 0 when every section completed, 1 otherwise.
 *
 * @code
 * #include "example_helpers.hpp"
 * int main()
 * {
 *     demo::title("Bisection", "Root of x^2 - 2");
 *     demo::section("Square root of 2", [] {
 *         demo::compare("root", 1.41421356, std::sqrt(2.0));
 *     });
 *     return demo::finish();
 * }
 * @endcode
 */

#include <cmath>
#include <exception>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

/**
 * @namespace demo
 * @brief Formatting and error-handling helpers used only by the example programs.
 */
namespace demo
{
    /// Number of sections that ended with an unexpected exception.
    inline int failed_sections = 0;

    /// Width of the decorative rules printed by title() and section().
    constexpr int WIDTH = 72;

    /**
     * @brief Prints the program title framed by two rules.
     * @param name      Main title, for example the module name.
     * @param subtitle  One line saying what the example shows.
     */
    inline void title(const std::string& name, const std::string& subtitle)
    {
        std::cout << std::string(WIDTH, '=') << "\n";
        std::cout << "  GROUP 23 | " << name << "\n";
        std::cout << "  " << subtitle << "\n";
        std::cout << std::string(WIDTH, '=') << "\n";
    }

    /**
     * @brief Runs one titled section of an example inside try/catch.
     *
     * An exception is reported and counted, and the program continues with
     * the next section.
     *
     * @tparam Code  Any callable taking no arguments (usually a lambda).
     * @param name  Section heading.
     * @param code  The work to run.
     */
    template <typename Code>
    void section(const std::string& name, Code code)
    {
        std::cout << "\n--- " << name << " " << std::string(name.size() < 60 ? 60 - name.size() : 0, '-') << "\n";
        try
        {
            code();
        }
        catch (const std::exception& e)
        {
            std::cout << "  [!] Unexpected error: " << e.what() << "\n";
            failed_sections++;
        }
    }

    /**
     * @brief Prints a labelled number.
     * @param label      Text printed before the value.
     * @param value      The number to print.
     * @param precision  Number of digits after the decimal point.
     */
    inline void value(const std::string& label, double value, int precision = 10)
    {
        std::cout << "  " << std::left << std::setw(34) << label << std::right
                  << std::fixed << std::setprecision(precision) << value << "\n";
    }

    /**
     * @brief Prints an approximation, the exact value and the absolute error on one line.
     * @param label   What was computed.
     * @param approx  The numerical result.
     * @param exact   The known exact value.
     */
    inline void compare(const std::string& label, double approx, double exact)
    {
        std::cout << "  " << std::left << std::setw(26) << label << std::right
                  << std::fixed << std::setprecision(10) << std::setw(16) << approx
                  << "   exact " << std::setw(16) << exact
                  << "   error " << std::scientific << std::setprecision(2) << std::abs(approx - exact)
                  << std::fixed << "\n";
    }

    /**
     * @brief Prints a table header followed by a rule.
     * @param columns  Column titles, each printed 16 characters wide.
     */
    inline void table_header(const std::vector<std::string>& columns)
    {
        std::cout << "  ";
        for (const std::string& c : columns)
        {
            std::cout << std::setw(16) << c;
        }
        std::cout << "\n  " << std::string(16 * columns.size(), '-') << "\n";
    }

    /**
     * @brief Prints one table row of numbers aligned under table_header().
     * @param values     The numbers in this row.
     * @param precision  Significant digits used for each number.
     */
    inline void table_row(const std::vector<double>& values, int precision = 8)
    {
        std::cout << "  ";
        for (double v : values)
        {
            std::cout << std::setw(16) << std::setprecision(precision) << std::defaultfloat << v;
        }
        std::cout << std::fixed << "\n";
    }

    /**
     * @brief Shows that the library rejects an invalid call.
     *
     * Runs @p code, expects an exception, and prints its message. This is
     * part of the demonstration, so a caught exception is not a failure.
     * If nothing is thrown, the section is counted as failed.
     *
     * @tparam Code  Any callable taking no arguments.
     * @param label  Description of the invalid call.
     * @param code   The call that should be rejected.
     */
    template <typename Code>
    void show_rejected(const std::string& label, Code code)
    {
        try
        {
            code();
            std::cout << "  " << label << ": NOT rejected (unexpected)\n";
            failed_sections++;
        }
        catch (const std::exception& e)
        {
            std::cout << "  " << label << ": rejected -> " << e.what() << "\n";
        }
    }

    /**
     * @brief Prints the closing rule and returns the program's exit code.
     * @return 0 if every section completed, 1 otherwise.
     */
    inline int finish()
    {
        std::cout << "\n" << std::string(WIDTH, '=') << "\n";
        std::cout << (failed_sections == 0 ? "  Example completed successfully.\n" : "  Example finished with errors.\n");
        std::cout << std::string(WIDTH, '=') << "\n";
        return failed_sections == 0 ? 0 : 1;
    }
}
