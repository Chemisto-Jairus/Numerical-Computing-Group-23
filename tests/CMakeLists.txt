# ==========================================
# Group 23: Unit Test Build Configuration
# Owner: Kyeyune Praise Pauline (Member 8)
# ==========================================
# Every test file has its own main(), so every test gets its own executable.
# The library # ==========================================
# Group 23: Unit Test Build Configuration
# Owner: Kyeyune Praise Pauline (Member 8)
# ==========================================
# Every test file has its own main(), so every test gets its own executable.
# The library is called numcomp2 (see the top-level CMakeLists.txt).
# Each block is wrapped in if(EXISTS ...) so a test file that is not
# merged yet cannot break the build.

# 1. Derrick's tests
if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/test_interpolator.cpp)
    add_executable(test_interpolator test_interpolator.cpp)
    target_link_libraries(test_interpolator numcomp2)
    add_test(NAME test_interpolator COMMAND test_interpolator)
endif()

if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/test_cubic_spline.cpp)
    add_executable(test_cubic_spline test_cubic_spline.cpp)
    target_link_libraries(test_cubic_spline numcomp2)
    add_test(NAME test_cubic_spline COMMAND test_cubic_spline)
endif()

# 2. Pauline's independent tests for Derrick's Matrix Solver
if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/test_matrix_solver.cpp)
    add_executable(test_matrix_solver test_matrix_solver.cpp)
    target_link_libraries(test_matrix_solver numcomp2)
    add_test(NAME test_matrix_solver COMMAND test_matrix_solver)
endif()

# 3. Jairus's tests
if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/test_basic_root_finder.cpp)
    add_executable(test_basic_root_finder test_basic_root_finder.cpp)
    target_link_libraries(test_basic_root_finder numcomp2)
    add_test(NAME test_basic_root_finder COMMAND test_basic_root_finder)
endif()

# 4. Sharon's tests
if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/test_basic_integrator.cpp)
    add_executable(test_basic_integrator test_basic_integrator.cpp)
    target_link_libraries(test_basic_integrator numcomp2)
    add_test(NAME test_basic_integrator COMMAND test_basic_integrator)
endif()

# 5. Valeria's tests
if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/test_advanced_root_finder.cpp)
    add_executable(test_advanced_root_finder test_advanced_root_finder.cpp)
    target_link_libraries(test_advanced_root_finder numcomp2)
    add_test(NAME test_advanced_root_finder COMMAND test_advanced_root_finder)
endif()

# 6. George's tests
if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/test_advanced_integrator.cpp)
    add_executable(test_advanced_integrator test_advanced_integrator.cpp)
    target_link_libraries(test_advanced_integrator numcomp2)
    add_test(NAME test_advanced_integrator COMMAND test_advanced_integrator)
endif()

# 7. Ian's tests
if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/test_basic_ode_solver.cpp)
    add_executable(test_basic_ode_solver test_basic_ode_solver.cpp)
    target_link_libraries(test_basic_ode_solver numcomp2)
    add_test(NAME test_basic_ode_solver COMMAND test_basic_ode_solver)
endif()

# 8. Samuel's tests
if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/test_advanced_ode_solver.cpp)
    add_executable(test_advanced_ode_solver test_advanced_ode_solver.cpp)
    target_link_libraries(test_advanced_ode_solver numcomp2)
    add_test(NAME test_advanced_ode_solver COMMAND test_advanced_ode_solver)
endif()
is called numcomp2 (see the top-level CMakeLists.txt).
# Each block is wrapped in if(EXISTS ...) so a test file that is not
# merged yet cannot break the build.

# 1. Derrick's tests
if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/test_interpolator.cpp)
    add_executable(test_interpolator test_interpolator.cpp)
    target_link_libraries(test_interpolator numcomp2)
    add_test(NAME test_interpolator COMMAND test_interpolator)
endif()

if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/test_cubic_spline.cpp)
    add_executable(test_cubic_spline test_cubic_spline.cpp)
    target_link_libraries(test_cubic_spline numcomp2)
    add_test(NAME test_cubic_spline COMMAND test_cubic_spline)
endif()

# 2. Pauline's independent tests for Derrick's Matrix Solver
if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/test_matrix_solver.cpp)
    add_executable(test_matrix_solver test_matrix_solver.cpp)
    target_link_libraries(test_matrix_solver numcomp2)
    add_test(NAME test_matrix_solver COMMAND test_matrix_solver)
endif()

# 3. Jairus's tests
if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/test_basic_root_finder.cpp)
    add_executable(test_basic_root_finder test_basic_root_finder.cpp)
    target_link_libraries(test_basic_root_finder numcomp2)
    add_test(NAME test_basic_root_finder COMMAND test_basic_root_finder)
endif()

# 4. Sharon's tests
if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/test_basic_integrator.cpp)
    add_executable(test_basic_integrator test_basic_integrator.cpp)
    target_link_libraries(test_basic_integrator numcomp2)
    add_test(NAME test_basic_integrator COMMAND test_basic_integrator)
endif()

# 5. Valeria's tests
if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/test_advanced_root_finder.cpp)
    add_executable(test_advanced_root_finder test_advanced_root_finder.cpp)
    target_link_libraries(test_advanced_root_finder numcomp2)
    add_test(NAME test_advanced_root_finder COMMAND test_advanced_root_finder)
endif()

# 6. George's tests
if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/test_advanced_integrator.cpp)
    add_executable(test_advanced_integrator test_advanced_integrator.cpp)
    target_link_libraries(test_advanced_integrator numcomp2)
    add_test(NAME test_advanced_integrator COMMAND test_advanced_integrator)
endif()

# 7. Ian's tests
if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/test_basic_ode_solver.cpp)
    add_executable(test_basic_ode_solver test_basic_ode_solver.cpp)
    target_link_libraries(test_basic_ode_solver numcomp2)
    add_test(NAME test_basic_ode_solver COMMAND test_basic_ode_solver)
endif()

# 8. Samuel's tests
if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/test_advanced_ode_solver.cpp)
    add_executable(test_advanced_ode_solver test_advanced_ode_solver.cpp)
    target_link_libraries(test_advanced_ode_solver numcomp2)
    add_test(NAME test_advanced_ode_solver COMMAND test_advanced_ode_solver)
endif()
