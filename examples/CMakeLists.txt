# ==========================================
# Group 23: Example programs
# Owner: Nabakooza Bridget Maria (Member 9)
# ==========================================
# Every example has its own main(), so every example is its own executable.
# Each one is also registered with ctest, so "ctest" checks that every demo
# runs to the end with exit code 0.
#
# if(EXISTS ...) means a demo that is not merged yet cannot break the build.
# To add a demo, add its file name (without .cpp) to this list.
set(GROUP23_EXAMPLES
    example_basic_root_finder                 # Jairus
    example_basic_root_finder_applied         # Jairus
    example_interpolator                      # Derrick
    example_interpolator_applied              # Derrick
    example_cubic_spline                      # Derrick
    example_cubic_spline_applied              # Derrick
    example_matrix_solver                     # Derrick
    example_matrix_solver_applied             # Derrick
    example_basic_integrator                  # Sharon
    example_basic_integrator_applied          # Sharon
    example_advanced_root_finder              # Valeria
    example_advanced_root_finder_applied      # Valeria
    example_advanced_integrator               # George
    example_advanced_integrator_applied       # George
    example_basic_ode_solver                  # Ian
    example_basic_ode_solver_applied          # Ian
    example_advanced_ode_solver               # Samuel
    example_advanced_ode_solver_applied       # Samuel
    # Week 5 modules
    example_numerical_differentiator          # Jairus
    example_numerical_differentiator_applied  # Jairus
    example_conjugate_gradient_solver         # Valeria
    example_conjugate_gradient_solver_applied # Valeria
    example_adams_bashforth_solver            # Ian
    example_adams_bashforth_solver_applied    # Ian
    example_adams_moulton_solver              # Samuel
    example_adams_moulton_solver_applied      # Samuel
    example_runge_kutta_fehlberg_solver       # George
    example_runge_kutta_fehlberg_solver_applied # George
    example_finite_difference_solver          # Sharon
    example_finite_difference_solver_applied  # Sharon
)

foreach(example_name ${GROUP23_EXAMPLES})
    if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/${example_name}.cpp)
        add_executable(${example_name} ${example_name}.cpp)
        target_link_libraries(${example_name} numcomp2)
        add_test(NAME ${example_name} COMMAND ${example_name})
    endif()
endforeach()
