# ==========================================
# Group 23: Unit Test Build Configuration
# Owner: Kyeyune Praise Pauline (Member 8)
# ==========================================
# Every test file has its own main(), so every test gets its own executable.
# The library is called numcomp2 (see the top-level CMakeLists.txt).
#
# HELPER: one loop instead of 9 copy-pasted blocks.
# To add a new test, add its name to this list. That is all.
set(GROUP23_TESTS
    test_interpolator           # Derrick
    test_cubic_spline           # Derrick
    test_matrix_solver          # Pauline (tests Derrick's Matrix Solver)
    test_basic_root_finder      # Jairus
    test_basic_integrator       # Sharon
    test_advanced_root_finder   # Valeria
    test_advanced_integrator    # George
    test_basic_ode_solver       # Ian
    test_advanced_ode_solver    # Samuel
)

foreach(test_name ${GROUP23_TESTS})
    # if(EXISTS ...) so a test file that is not merged yet cannot break the build
    if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/${test_name}.cpp)
        add_executable(${test_name} ${test_name}.cpp)
        target_link_libraries(${test_name} numcomp2)
        add_test(NAME ${test_name} COMMAND ${test_name})
    endif()
endforeach()
