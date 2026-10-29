/**
 * @file console_ui.cpp
 * @brief Implementation of the console helpers in app::ui.
 * @author Nabakooza Bridget Maria (Member 9)
 */

#include "console_ui.hpp"
#include <cmath>
#include <iomanip>
#include <iostream>
#include <sstream>

namespace app
{
    namespace ui
    {
        /**
         * @brief Reads one line from standard input after printing a prompt.
         * @param prompt  Text printed before reading.
         * @return The line without its trailing newline or carriage return.
         * @throws InputEnded If no more input is available.
         */
        static std::string read_line(const std::string& prompt)
        {
            std::cout << "  " << prompt << std::flush;
            std::string line;
            if (!std::getline(std::cin, line))
            {
                throw InputEnded{};
            }
            // Lines typed on Windows or piped from a Windows file may end in '\r'.
            if (!line.empty() && line.back() == '\r') line.pop_back();
            return line;
        }

        /**
         * @brief Removes leading and trailing spaces and tabs.
         * @param text  The text to trim.
         * @return The trimmed text.
         */
        static std::string trim(const std::string& text)
        {
            std::size_t first = text.find_first_not_of(" \t");
            if (first == std::string::npos) return "";
            std::size_t last = text.find_last_not_of(" \t");
            return text.substr(first, last - first + 1);
        }

        /**
         * @brief Centres text within the frame width.
         * @param text  The text to centre.
         * @return The text padded on the left.
         */
        static std::string centred(const std::string& text)
        {
            int pad = (WIDTH - static_cast<int>(text.size())) / 2;
            return std::string(pad > 0 ? pad : 0, ' ') + text;
        }

        void print_rule(char fill)
        {
            std::cout << std::string(WIDTH, fill) << "\n";
        }

        void print_banner()
        {
            // 5-row block letters, 6 columns wide each, drawn with '#'.
            // Order of glyphs: G R O U P (space) 2 3
            const char* glyphs[8][5] = {
                {" #####", "#     ", "#  ###", "#    #", " #### "},   // G
                {"##### ", "#    #", "##### ", "#   # ", "#    #"},   // R
                {" #### ", "#    #", "#    #", "#    #", " #### "},   // O
                {"#    #", "#    #", "#    #", "#    #", " #### "},   // U
                {"##### ", "#    #", "##### ", "#     ", "#     "},   // P
                {"   ",    "   ",    "   ",    "   ",    "   "},      // space
                {" #### ", "#    #", "   ## ", " ##   ", "######"},   // 2
                {"##### ", "     #", "  ### ", "     #", "##### "},   // 3
            };

            std::cout << "\n";
            print_rule('=');
            std::cout << "\n";
            for (int row = 0; row < 5; ++row)
            {
                std::string line;
                for (int g = 0; g < 8; ++g)
                {
                    line += glyphs[g][row];
                    if (g < 7) line += "  ";
                }
                std::cout << centred(line) << "\n";
            }
            std::cout << "\n";
            std::cout << centred("N U M E R I C A L   C O M P U T I N G   2") << "\n";
            std::cout << centred("C++17 library: roots, approximation, calculus, ODEs, BVPs, linear systems") << "\n";
            std::cout << "\n";
            print_rule('=');
        }

        void print_header(const std::string& title, const std::string& subtitle)
        {
            std::cout << "\n+" << std::string(WIDTH - 2, '-') << "+\n";
            std::string t = centred(title);
            std::cout << "|" << t.substr(1) << std::string(WIDTH - 1 - static_cast<int>(t.size()), ' ') << "|\n";
            if (!subtitle.empty())
            {
                std::string s = centred(subtitle);
                std::cout << "|" << s.substr(1) << std::string(WIDTH - 1 - static_cast<int>(s.size()), ' ') << "|\n";
            }
            std::cout << "+" << std::string(WIDTH - 2, '-') << "+\n";
        }

        int choose(const std::string& title, const std::vector<std::string>& options, const std::string& zero_label)
        {
            std::cout << "\n  " << title << "\n";
            std::cout << "  " << std::string(title.size(), '~') << "\n";
            for (std::size_t i = 0; i < options.size(); ++i)
            {
                std::cout << "   " << std::setw(2) << i + 1 << ")  " << options[i] << "\n";
            }
            std::cout << "    0)  " << zero_label << "\n";
            return read_int("Your choice: ", -1, 0, static_cast<int>(options.size()));
        }

        double read_double(const std::string& prompt, double default_value)
        {
            std::ostringstream shown;
            shown << prompt << " [" << default_value << "]: ";
            while (true)
            {
                std::string text = trim(read_line(shown.str()));
                if (text.empty())
                {
                    return default_value;
                }
                try
                {
                    std::size_t used = 0;
                    double value = std::stod(text, &used);
                    if (used == text.size() && std::isfinite(value))
                    {
                        return value;
                    }
                }
                catch (const std::exception&)
                {
                    // Not a number: fall through and ask again.
                }
                print_error("Invalid input", "please type a number such as 2, -0.5 or 1e-6.");
            }
        }

        int read_int(const std::string& prompt, int default_value, int min_value, int max_value)
        {
            // A default outside the range means "no default": an empty line is rejected.
            bool has_default = default_value >= min_value && default_value <= max_value;
            std::ostringstream shown;
            shown << prompt;
            if (has_default)
            {
                shown << "[" << default_value << "]: ";
            }
            while (true)
            {
                std::string text = trim(read_line(shown.str()));
                if (text.empty() && has_default)
                {
                    return default_value;
                }
                try
                {
                    std::size_t used = 0;
                    long value = std::stol(text, &used);
                    if (used == text.size() && value >= min_value && value <= max_value)
                    {
                        return static_cast<int>(value);
                    }
                }
                catch (const std::exception&)
                {
                    // Not a whole number: fall through and ask again.
                }
                print_error("Invalid input", "please type a whole number from " + std::to_string(min_value) + " to " + std::to_string(max_value) + ".");
            }
        }

        bool ask_yes_no(const std::string& prompt, bool default_value)
        {
            while (true)
            {
                std::string text = trim(read_line(prompt + (default_value ? " [Y/n]: " : " [y/N]: ")));
                if (text.empty()) return default_value;
                if (text == "y" || text == "Y" || text == "yes") return true;
                if (text == "n" || text == "N" || text == "no") return false;
                print_error("Invalid input", "please answer y or n.");
            }
        }

        void print_value(const std::string& label, double value, int precision)
        {
            std::string dots = label.size() < 36 ? std::string(36 - label.size(), '.') : "";
            std::cout << "  " << label << " " << dots << " " << std::fixed << std::setprecision(precision) << value << "\n";
        }

        void print_compare(const std::string& label, double approx, double exact)
        {
            std::cout << "  " << std::left << std::setw(20) << label << std::right
                      << std::fixed << std::setprecision(10) << std::setw(16) << approx
                      << "  exact" << std::setw(16) << exact
                      << "  error " << std::scientific << std::setprecision(2) << std::abs(approx - exact)
                      << std::fixed << "\n";
        }

        /**
         * @brief Width of one table column, so that a row never exceeds WIDTH characters.
         * @param columns  Number of columns in the table.
         * @return 18 for up to four columns, narrower for wider tables.
         */
        static int column_width(std::size_t columns)
        {
            return columns <= 4 ? 18 : (WIDTH - 2) / static_cast<int>(columns);
        }

        void print_table_header(const std::vector<std::string>& columns)
        {
            int width = column_width(columns.size());
            std::cout << "  ";
            for (const std::string& c : columns) std::cout << std::setw(width) << c;
            std::cout << "\n  " << std::string(width * columns.size(), '-') << "\n";
        }

        void print_table_row(const std::vector<double>& values)
        {
            int width = column_width(values.size());
            int digits = (width >= 18) ? 10 : 8;   // narrower columns show fewer significant digits
            std::cout << "  ";
            for (double v : values) std::cout << std::setw(width) << std::setprecision(digits) << std::defaultfloat << v;
            std::cout << std::fixed << "\n";
        }

        void print_info(const std::string& text)
        {
            std::cout << "  " << text << "\n";
        }

        void print_error(const std::string& kind, const std::string& message)
        {
            // Long messages are wrapped at word boundaries so no line exceeds WIDTH;
            // continuation lines are indented under the start of the message.
            std::string line = "  [!] " + kind + ": ";
            const std::string indent = "      ";
            std::istringstream words(message);
            std::string word;
            bool line_has_word = false;
            while (words >> word)
            {
                if (line_has_word && line.size() + 1 + word.size() > static_cast<std::size_t>(WIDTH))
                {
                    std::cout << line << "\n";
                    line = indent;
                    line_has_word = false;
                }
                if (line_has_word) line += " ";
                line += word;
                line_has_word = true;
            }
            std::cout << line << "\n";
        }
    }
}
