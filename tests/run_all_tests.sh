#!/bin/bash
# =====================================================================
# Group 23: automated test runner
# Owner: Kyeyune Praise Pauline (Member 8)
# =====================================================================
# Runs every unit-test program, then every example program, and prints a
# summary. Run it from the repository root (the folder with README.md):
#
#     bash tests/run_all_tests.sh                (uses build/tests and build/examples)
#     bash tests/run_all_tests.sh build-gpp      (programs built with g++ into one folder)
#
# Running it from inside build/tests also works:  bash ../../tests/run_all_tests.sh
#
# Works on Linux, macOS and Git Bash on Windows: .exe files and the
# Debug/ or Release/ sub-folders of Visual Studio builds are found too.
# A program that has not been built is reported as SKIPPED, not FAILED.
# Exit code: 0 if every program that ran passed, 1 otherwise.

# ---- Locate the folder(s) holding the built programs ----------------
if [ -n "$1" ]; then
    TEST_DIR="$1"
elif ls ./test_* >/dev/null 2>&1; then
    TEST_DIR="."
else
    TEST_DIR="build/tests"
fi

if [ ! -d "$TEST_DIR" ]; then
    echo "[!] Folder '$TEST_DIR' not found."
    echo "    Build first:  cmake -S . -B build  &&  cmake --build build"
    exit 1
fi

# CMake puts the tests in <build>/tests and the examples in <build>/examples.
# A g++ build usually puts every program in one folder. So: if the test folder
# is called "tests", look for examples in its sibling "examples"; otherwise
# look in the same folder.
TEST_DIR_ABS="$(cd "$TEST_DIR" && pwd)"
if [ "$(basename "$TEST_DIR_ABS")" = "tests" ] && [ -d "$TEST_DIR_ABS/../examples" ]; then
    EXAMPLE_DIR="$TEST_DIR_ABS/../examples"
else
    EXAMPLE_DIR="$TEST_DIR"
fi

failed=0
ran=0
skipped=0

# run_program <folder> <program name> <owner>
# Finds the program (plain, .exe, or inside Debug/ or Release/), runs it,
# and records PASSED (exit code 0), FAILED (any other code) or SKIPPED.
run_program() {
    local exe=""
    for candidate in "$1/$2" "$1/$2.exe" "$1/Debug/$2.exe" "$1/Release/$2.exe" "$1/Debug/$2" "$1/Release/$2"; do
        if [ -f "$candidate" ]; then
            exe="$candidate"
            break
        fi
    done

    if [ -z "$exe" ]; then
        echo ">>> SKIPPED $2 ($3): not built"
        skipped=$((skipped + 1))
        return
    fi

    ran=$((ran + 1))
    if [ "$QUIET" = "1" ]; then
        # Examples print a lot; only the verdict is shown.
        if "$exe" >/dev/null 2>&1; then
            echo ">>> PASSED  $2 ($3)"
        else
            echo ">>> FAILED  $2 ($3)"
            failed=$((failed + 1))
        fi
    else
        echo ">>> Running $2 ($3)..."
        if "$exe"; then
            echo "    PASSED"
        else
            echo "    FAILED"
            failed=$((failed + 1))
        fi
        echo ""
    fi
}

# "program:owner" pairs. To add a program, add one line.
TESTS=(
    "test_interpolator:Derrick"
    "test_cubic_spline:Derrick"
    "test_matrix_solver:Pauline (Derrick's module)"
    "test_basic_root_finder:Jairus"
    "test_basic_integrator:Sharon"
    "test_advanced_root_finder:Valeria"
    "test_advanced_integrator:George"
    "test_basic_ode_solver:Ian"
    "test_advanced_ode_solver:Samuel"
    "test_numerical_differentiator:Jairus"
    "test_conjugate_gradient_solver:Valeria"
    "test_adams_bashforth_solver:Ian"
    "test_adams_moulton_solver:Samuel"
    "test_runge_kutta_fehlberg_solver:George"
    "test_finite_difference_solver:Sharon"
)

EXAMPLES=(
    "example_basic_root_finder:Jairus"
    "example_basic_root_finder_applied:Jairus"
    "example_interpolator:Derrick"
    "example_interpolator_applied:Derrick"
    "example_cubic_spline:Derrick"
    "example_cubic_spline_applied:Derrick"
    "example_matrix_solver:Derrick"
    "example_matrix_solver_applied:Derrick"
    "example_basic_integrator:Sharon"
    "example_basic_integrator_applied:Sharon"
    "example_advanced_root_finder:Valeria"
    "example_advanced_root_finder_applied:Valeria"
    "example_advanced_integrator:George"
    "example_advanced_integrator_applied:George"
    "example_basic_ode_solver:Ian"
    "example_basic_ode_solver_applied:Ian"
    "example_advanced_ode_solver:Samuel"
    "example_advanced_ode_solver_applied:Samuel"
    "example_numerical_differentiator:Jairus"
    "example_numerical_differentiator_applied:Jairus"
    "example_conjugate_gradient_solver:Valeria"
    "example_conjugate_gradient_solver_applied:Valeria"
    "example_adams_bashforth_solver:Ian"
    "example_adams_bashforth_solver_applied:Ian"
    "example_adams_moulton_solver:Samuel"
    "example_adams_moulton_solver_applied:Samuel"
    "example_runge_kutta_fehlberg_solver:George"
    "example_runge_kutta_fehlberg_solver_applied:George"
    "example_finite_difference_solver:Sharon"
    "example_finite_difference_solver_applied:Sharon"
)

echo "==================================================="
echo "       GROUP 23: AUTOMATED TEST RUNNER             "
echo "==================================================="
echo ""
echo "---- Unit tests ($TEST_DIR) ----"
echo ""
QUIET=0
for entry in "${TESTS[@]}"; do
    run_program "$TEST_DIR" "${entry%%:*}" "${entry#*:}"   # before ":" = program, after = owner
done

echo "---- Example programs ($EXAMPLE_DIR) ----"
QUIET=1
for entry in "${EXAMPLES[@]}"; do
    run_program "$EXAMPLE_DIR" "${entry%%:*}" "${entry#*:}"
done

echo ""
echo "==================================================="
echo "  Ran: $ran   Failed: $failed   Skipped: $skipped"
if [ $ran -eq 0 ]; then
    echo "  NO PROGRAMS FOUND in $TEST_DIR - did the build finish?"
    echo "==================================================="
    exit 1
elif [ $failed -eq 0 ]; then
    echo "  ALL PROGRAMS PASSED"
    echo "==================================================="
    exit 0
else
    echo "  SOME PROGRAMS FAILED - see the messages above"
    echo "==================================================="
    exit 1
fi
