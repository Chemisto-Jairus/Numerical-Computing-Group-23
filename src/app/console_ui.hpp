#pragma once

#include <string>
#include <utility>
#include <vector>

namespace app
{
    namespace ui
    {
        constexpr int WIDTH = 78;

        struct InputEnded {};

        void print_banner();

        void print_header(const std::string& title, const std::string& subtitle = "");

        void print_rule(char fill = '-');

        int choose(const std::string& title, const std::vector<std::string>& options, const std::string& zero_label = "Back");

        double read_double(const std::string& prompt, double default_value);

        int read_int(const std::string& prompt, int default_value, int min_value, int max_value);

        bool ask_yes_no(const std::string& prompt, bool default_value);

        void print_value(const std::string& label, double value, int precision = 10);

        void print_compare(const std::string& label, double approx, double exact);

        void print_table_header(const std::vector<std::string>& columns);

        void print_table_row(const std::vector<double>& values);

        void print_info(const std::string& text);

        void print_error(const std::string& kind, const std::string& message);
    }
}
