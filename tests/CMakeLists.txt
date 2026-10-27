# ==========================================
# Group 23: Unit test programs
# Owner: Kyeyune Praise Pauline (Member 8)
# ==========================================
# Every test file has its own main(), so every test is its own executable,
# linked against the numcomp2 library and registered with ctest.
# The shared framework tests/test_helpers.hpp is header-only.
#
# if(EXISTS ...) means a test file that is not merged yet cannot break the build.
# To add a test, add its file name (without .cpp) to this list.
set(GROUP23_TESTS
    test_interpolator           # Derrick (also Chebyshev approximation)
    test_cubic_spline           # Derrick
    test_matrix_solver          # Pauline (independent tests of Derrick's module)
    test_basic_root_finder      # Jairus
    test_basic_integrator       # Sharon
    test_advanced_root_finder   # Valeria
    test_advanced_integrator    # George
    test_basic_ode_solver       # Ian
    test_advanced_ode_solver    # Samuel
    # Week 5 modules
    test_numerical_differentiator      # Jairus
    test_conjugate_gradient_solver     # Valeria
    test_adams_bashforth_solver        # Ian
    test_adams_moulton_solver          # Samuel
    test_runge_kutta_fehlberg_solver   # George
    test_finite_difference_solver      # Sharon
)

foreach(test_name ${GROUP23_TESTS})
    if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/${test_name}.cpp)
        add_executable(${test_name} ${test_name}.cpp)
        target_link_libraries(${test_name} numcomp2)
        add_test(NAME ${test_name} COMMAND ${test_name})
    endif()
endforeach()
