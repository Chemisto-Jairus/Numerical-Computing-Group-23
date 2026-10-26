#include "console_ui.hpp"
#include <cmath>
#include <iomanip>
#include <iostream>
#include <sstream>

namespace app
{
    namespace ui
    {
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

        static std::string trim(const std::string& text)
        {
            std::size_t first = text.find_first_not_of(" \t");
            if (first == std::string::npos) return "";
            std::size_t last = text.find_last_not_of(" \t");
            return text.substr(first, last - first + 1);
        }

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
            std::cout << "\n";
            print_rule('=');
            std::cout << centred("GROUP 23: NUMERICAL COMPUTING 2") << "\n";
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

        void print_table_header(const std::vector<std::string>& columns)
        {
            std::cout << "  ";
            for (const std::string& c : columns) std::cout << std::setw(18) << c;
            std::cout << "\n  " << std::string(18 * columns.size(), '-') << "\n";
        }

        void print_table_row(const std::vector<double>& values)
        {
            std::cout << "  ";
            for (double v : values) std::cout << std::setw(18) << std::setprecision(10) << std::defaultfloat << v;
            std::cout << std::fixed << "\n";
        }

        void print_info(const std::string& text)
        {
            std::cout << "  " << text << "\n";
        }

        void print_error(const std::string& kind, const std::string& message)
        {
            std::cout << "  [!] " << kind << ": " << message << "\n";
        }
    }
}
