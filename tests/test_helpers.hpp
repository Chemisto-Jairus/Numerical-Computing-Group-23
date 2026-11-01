#pragma once
/**
 * @file test_helpers.hpp
 * @brief Minimal header-only test framework shared by every Group 23 test program.
 * @author Kyeyune Praise Pauline (Member 8)
 *
 * Each test program includes this header, records results with check() and
 * the expect_* helpers, and returns test::finish() from main(). The exit code
 * is 0 when every check passed and 1 otherwise, which is what ctest and
 * tests/run_all_tests.sh look at.
 *
 * Plain C++17 with no external dependencies. It does not rely on assert(),
 * so the checks run identically in Debug and Release builds.
 *
 * @code
 * #include "test_helpers.hpp"
 * int main()
 * {
 *     test::check("1 + 1 is 2", 1 + 1 == 2);
 *     test::expect_throw<std::invalid_argument>("bad n throws", [] { throw std::invalid_argument("n"); });
 *     return test::finish("Example");
 * }
 * @endcode
 */

#include <cmath>
#include <iostream>
#include <string>

/**
 * @namespace test
 * @brief Counters and assertion helpers used by the test programs.
 */
namespace test
{
    /// Number of checks that passed so far in this program.
    inline int passed = 0;

    /// Number of checks that failed so far in this program.
    inline int failed = 0;

    /**
     * @brief Records one test result and prints "[PASS] name" or "[FAIL] name".
     * @param name  Description of what is being checked.
     * @param ok    true if the check succeeded.
     */
    inline void check(const std::string& name, bool ok)
    {
        if (ok)
        {
            std::cout << "[PASS] " << name << std::endl;
            passed++;
        }
        else
        {
            std::cout << "[FAIL] " << name << std::endl;
            failed++;
        }
    }

    /**
     * @brief Compares two floating-point numbers with an absolute tolerance.
     * @param a    First value.
     * @param b    Second value.
     * @param tol  Largest allowed difference.
     * @return true if |a - b| < tol.
     */
    inline bool close(double a, double b, double tol)
    {
        return std::abs(a - b) < tol;
    }

    /**
     * @brief Passes only if running @p code throws an exception of type @p Expected.
     *
     * Any other exception type, or no exception at all, is recorded as a failure.
     *
     * @tparam Expected  Exception type that must be thrown, for example std::invalid_argument.
     * @tparam Code      Any callable taking no arguments (usually a lambda).
     * @param name  Description of what is being checked.
     * @param code  The code that should throw.
     */
    template <typename Expected, typename Code>
    void expect_throw(const std::string& name, Code code)
    {
        bool ok = false;
        try
        {
            code();
        }
        catch (const Expected&)
        {
            ok = true;
        }
        catch (...)
        {
        }
        check(name, ok);
    }

    /**
     * @brief Passes only if running @p code does not throw anything.
     * @tparam Code  Any callable taking no arguments (usually a lambda).
     * @param name  Description of what is being checked.
     * @param code  The code that must complete normally.
     */
    template <typename Code>
    void expect_no_throw(const std::string& name, Code code)
    {
        bool ok = true;
        try
        {
            code();
        }
        catch (...)
        {
            ok = false;
        }
        check(name, ok);
    }

    /**
     * @brief Prints the summary line for a test program.
     * @param module_name  Name shown in the summary, for example "Cubic Spline".
     * @return 0 if every check passed, 1 if any failed. Return this from main().
     */
    inline int finish(const std::string& module_name)
    {
        std::cout << std::endl << module_name << ": " << passed << " passed, " << failed << " failed" << std::endl;
        return failed > 0 ? 1 : 0;
    }
}
