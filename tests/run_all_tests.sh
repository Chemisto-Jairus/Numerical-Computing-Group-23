#!/bin/bash
# ===================================================
# Group 23: automated test runner
# Owner: Kyeyune Praise Pauline (Member 8)
# ===================================================
# Run it from the REPO ROOT (the folder with README.md):
#
#     bash tests/run_all_tests.sh               (looks in build/tests)
#     bash tests/run_all_tests.sh other/tests   (looks in another build folder)
#
# Running it from inside build/tests also works:  bash ../../tests/run_all_tests.sh
# Works on Linux, macOS and Git Bash on Windows (.exe files and the
# Debug/ or Release/ sub-folders of Visual Studio builds are found too).
# A test that is not built yet is SKIPPED, not FAILED.
# Exit code: 0 if every test that ran passed, 1 otherwise.

# Pick the folder that holds the test programs
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
cd "$TEST_DIR" || exit 1

failed=0
ran=0
skipped=0

# run_test <program name> <owner>
run_test() {
    exe=""
    for candidate in "./$1" "./$1.exe" "./Debug/$1.exe" "./Release/$1.exe" "./Debug/$1" "./Release/$1"; do
        if [ -f "$candidate" ]; then
            exe="$candidate"
            break
        fi
    done

    if [ -z "$exe" ]; then
        echo ">>> SKIPPED $1 ($2): not built yet"
        echo ""
        skipped=$((skipped + 1))
        return
    fi

    echo ">>> Running $1 ($2)..."
    ran=$((ran + 1))
    if "$exe"; then
        echo "    PASSED"
    else
        echo "    FAILED"
        failed=$((failed + 1))
    fi
    echo ""
}

echo "==================================================="
echo "       GROUP 23: AUTOMATED TEST RUNNER             "
echo "==================================================="
echo ""

run_test test_interpolator "Derrick"
run_test test_cubic_spline "Derrick"
run_test test_matrix_solver "Pauline (Derrick's module)"
run_test test_basic_root_finder "Jairus"
run_test test_basic_integrator "Sharon"
run_test test_advanced_root_finder "Valeria"
run_test test_advanced_integrator "George"
run_test test_basic_ode_solver "Ian"
run_test test_advanced_ode_solver "Samuel"

echo "==================================================="
echo "  Ran: $ran   Failed: $failed   Skipped: $skipped"
if [ $ran -eq 0 ]; then
    echo "  NO TESTS FOUND in $TEST_DIR - did the build finish?"
    echo "==================================================="
    exit 1
elif [ $failed -eq 0 ]; then
    echo "  ALL TESTS PASSED"
    echo "==================================================="
    exit 0
else
    echo "  SOME TESTS FAILED - see the messages above"
    echo "==================================================="
    exit 1
fi
