#pragma once
// =====================================================================
// Group 23: shared mini test framework (tests only, not part of the library)
// Owner: Batte Derrick Calvin (Member 2)
//
// Before: every test file had its own copy of report(), close(), the
// pass/fail counters and a 15-line try/catch block for every "should
// throw" test. Now all test files include this one header.
//
// No outside test framework and no assert(), so it works the same in
// Debug and Release builds and needs nothing to be downloaded.
// =====================================================================
#include <cmath>
#include <iostream>
#include <string>

namespace test
{
    // "inline" (C++17) lets these counters live in a header safely
    inline int passed = 0;
    inline int failed = 0;

    /// Records one test result and prints [PASS] or [FAIL].
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

    /// True if a and b are equal within tol.
    inline bool close(double a, double b, double tol)
    {
        return std::abs(a - b) < tol;
    }

    /// Passes only if calling "code" throws the exception type Expected.
    /// Example: test::expect_throw<std::invalid_argument>("n = 0 throws", [&] { solver.f(0); });
    /// Any other exception type, or no exception, is a FAIL.
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

    /// Passes only if calling "code" does not throw anything.
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

    /// Prints the summary line and gives main() its return value:
    /// 0 if everything passed, 1 if anything failed (ctest and
    /// run_all_tests.sh read this exit code).
    inline int finish(const std::string& module_name)
    {
        std::cout << std::endl << module_name << ": " << passed << " passed, " << failed << " failed" << std::endl;
        return failed > 0 ? 1 : 0;
    }
}
