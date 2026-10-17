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

echo "==================================================="
if [ $failed -eq 0 ]; then
    echo "  ALL TESTS PASSED"
else
    echo "  SOME TESTS FAILED - see the messages above"
    exit 1
fi
echo "==================================================="