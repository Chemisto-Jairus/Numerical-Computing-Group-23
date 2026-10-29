#pragma once
/**
 * @file console_ui.hpp
 * @brief Console input and output helpers for the Group 23 command-line app.
 * @author Nabakooza Bridget Maria (Member 9)
 *
 * Everything the app prints or reads goes through these functions, so the
 * layout is consistent and every number typed by the user is validated
 * before it reaches the library. Input is read one whole line at a time;
 * pressing Enter on an empty line accepts the default shown in brackets.
 * Output uses plain ASCII only, so it displays correctly in every terminal,
 * including the Windows console.
 */

#include <string>
#include <utility>
#include <vector>

namespace app
{
    /**
     * @namespace app::ui
     * @brief Screen layout, menus and validated keyboard input.
     */
    namespace ui
    {
        /// Width of every frame and rule drawn by the app, in characters.
        constexpr int WIDTH = 78;

        /**
         * @brief Thrown when standard input ends (Ctrl+D / Ctrl+Z or end of a piped file).
         *
         * Deliberately not derived from std::exception, so the per-calculation
         * error handlers in the menus do not catch it; main() catches it and
         * closes the app cleanly.
         */
        struct InputEnded {};

        /// Prints the large centred "GROUP 23" banner and the library title.
        void print_banner();

        /**
         * @brief Prints a framed screen title.
         * @param title     Main heading, centred.
         * @param subtitle  Optional second line, centred. Empty for none.
         */
        void print_header(const std::string& title, const std::string& subtitle = "");

        /**
         * @brief Prints a horizontal rule made of one character.
         * @param fill  The character to repeat across the full width.
         */
        void print_rule(char fill = '-');

        /**
         * @brief Shows a numbered menu and returns the user's valid choice.
         *
         * Options are numbered from 1. Option 0 is always shown with @p zero_label.
         *
         * @param title       Heading printed above the options.
         * @param options     Text of options 1..n.
         * @param zero_label  Text for option 0, for example "Back" or "Exit".
         * @return A number from 0 to options.size().
         * @throws InputEnded If input ends before a valid choice is made.
         */
        int choose(const std::string& title, const std::vector<std::string>& options, const std::string& zero_label = "Back");

        /**
         * @brief Reads a real number, offering a default value.
         *
         * Keeps asking until the line is a valid number (the whole line must parse)
         * or is empty, in which case @p default_value is returned.
         *
         * @param prompt         Question shown to the user.
         * @param default_value  Value used when the user just presses Enter.
         * @return The number typed, or the default.
         * @throws InputEnded If input ends.
         */
        double read_double(const std::string& prompt, double default_value);

        /**
         * @brief Reads a whole number within [min_value, max_value], offering a default.
         * @param prompt         Question shown to the user.
         * @param default_value  Value used when the user just presses Enter.
         * @param min_value      Smallest accepted value.
         * @param max_value      Largest accepted value.
         * @return The number typed, or the default.
         * @throws InputEnded If input ends.
         */
        int read_int(const std::string& prompt, int default_value, int min_value, int max_value);

        /**
         * @brief Asks a yes/no question.
         * @param prompt         Question shown to the user.
         * @param default_value  Answer used when the user just presses Enter.
         * @return true for yes.
         * @throws InputEnded If input ends.
         */
        bool ask_yes_no(const std::string& prompt, bool default_value);

        /**
         * @brief Prints "label .......... value".
         * @param label      Description of the value.
         * @param value      The number.
         * @param precision  Digits after the decimal point.
         */
        void print_value(const std::string& label, double value, int precision = 10);

        /**
         * @brief Prints a result, the exact value and the absolute error.
         * @param label   Description of the result.
         * @param approx  Numerical result.
         * @param exact   Known exact value.
         */
        void print_compare(const std::string& label, double approx, double exact);

        /**
         * @brief Prints a table header with fixed-width columns.
         *
         * Columns are 18 characters wide; tables with five or more columns use
         * narrower columns so that every line stays within WIDTH characters.
         * @param columns  Column titles.
         */
        void print_table_header(const std::vector<std::string>& columns);

        /**
         * @brief Prints one row of numbers under print_table_header().
         * @param values  The numbers in this row.
         */
        void print_table_row(const std::vector<double>& values);

        /**
         * @brief Prints an informational line, indented to match the layout.
         * @param text  The message.
         */
        void print_info(const std::string& text);

        /**
         * @brief Prints an error message starting with "[!]".
         *
         * A long message is wrapped at word boundaries so that no line is wider than WIDTH.
         *
         * @param kind     Short category, for example "Invalid input".
         * @param message  Details, usually the exception's what().
         */
        void print_error(const std::string& kind, const std::string& message);
    }
}
