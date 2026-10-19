#!/bin/bash
# Run this from the folder that holds the built test programs (build/tests).
# It runs every test and only reports success if ALL of them passed.

failed=0

run_test() {
    echo ">>> Running $1 ($2)..."
    if ./$1; then
        echo "    PASSED"
    else
        echo "    FAILED"
        failed=1
    fi
    echo ""
}

echo "==================================================="
echo "       GROUP 23: AUTOMATED TEST RUNNER             "
echo "==================================================="
echo ""

run_test test_interpolator "Derrick"
run_test test_basic_integrator "Sharon"
run_test test_advanced_integrator "George"
run_test test_advanced_root_finder "Valeria"
# Week 4: remove the # at the start of these lines when the tests exist
# run_test test_basic_root_finder "Jairus"
# run_test test_basic_ode_solver "Ian"
# run_test test_advanced_ode_solver "Samuel"

echo "==================================================="
if [ $failed -eq 0 ]; then
    echo "  ALL TESTS PASSED"
else
    echo "  SOME TESTS FAILED - see the messages above"
    exit 1
fi
echo "==================================================="
