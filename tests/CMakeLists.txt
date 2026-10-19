# ==========================================
# Group 23: Unit Test Build Configuration
# ==========================================
# Every test file has its own main(), so every test gets its own executable.
# The library is called numcomp2 (see the top-level CMakeLists.txt).

# 1. Derrick's tests
if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/test_interpolator.cpp)
add_executable(test_interpolator test_interpolator.cpp)
target_link_libraries(test_interpolator numcomp2)
add_test(NAME test_interpolator COMMAND test_interpolator)
endif()

# 2. Sharon's tests
if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/test_basic_integrator.cpp)
add_executable(test_basic_integrator test_basic_integrator.cpp)
target_link_libraries(test_basic_integrator numcomp2)
add_test(NAME test_basic_integrator COMMAND test_basic_integrator)
endif()

# 3. George's tests
if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/test_advanced_integrator.cpp)
add_executable(test_advanced_integrator test_advanced_integrator.cpp)
target_link_libraries(test_advanced_integrator numcomp2)
add_test(NAME test_advanced_integrator COMMAND test_advanced_integrator)
endif()

# 4. Valeria's tests
if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/test_advanced_root_finder.cpp)
add_executable(test_advanced_root_finder test_advanced_root_finder.cpp)
target_link_libraries(test_advanced_root_finder numcomp2)
add_test(NAME test_advanced_root_finder COMMAND test_advanced_root_finder)
endif()

# 4b. Jairus's tests (still to be written). Only built once the file exists.
if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/test_basic_root_finder.cpp)
    add_executable(test_basic_root_finder test_basic_root_finder.cpp)
    target_link_libraries(test_basic_root_finder numcomp2)
    add_test(NAME test_basic_root_finder COMMAND test_basic_root_finder)
endif()

# 5. Ian's tests
if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/test_basic_ode_solver.cpp)
    add_executable(test_basic_ode_solver test_basic_ode_solver.cpp)
    target_link_libraries(test_basic_ode_solver numcomp2)
    add_test(NAME test_basic_ode_solver COMMAND test_basic_ode_solver)
endif()

# 6. Samuel's tests 
if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/test_advanced_ode_solver.cpp)
    add_executable(test_advanced_ode_solver test_advanced_ode_solver.cpp)
    target_link_libraries(test_advanced_ode_solver numcomp2)
    add_test(NAME test_advanced_ode_solver COMMAND test_advanced_ode_solver)
endif()
