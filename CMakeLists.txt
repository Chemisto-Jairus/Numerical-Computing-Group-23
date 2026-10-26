cmake_minimum_required(VERSION 3.10)

project(NumericalComputing2 VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include_directories(include)

# The library. Every module's .cpp file goes in this list (not main.cpp).
add_library(numcomp2
    src/basic_root_finder.cpp
    src/interpolator.cpp
    src/basic_integrator.cpp
    src/advanced_root_finder.cpp
    src/advanced_integrator.cpp
    src/basic_ode_solver.cpp
    src/advanced_ode_solver.cpp
    src/cubic_spline.cpp
    src/matrix_solver.cpp
    # Shared internal helpers (not public: they live in src/internal/)
    src/internal/input_checks.cpp
    src/internal/ode_stepper.cpp
)

# Tests: "ctest" in the build folder runs every test registered in tests/
enable_testing()
add_subdirectory(tests)

# The command-line app
add_executable(numcomp_app
    src/main.cpp
    src/app/console_ui.cpp
    src/app/catalog.cpp
    src/app/module_menus.cpp
)
target_link_libraries(numcomp_app numcomp2)

# Install rules 
#   cmake --install build --prefix <folder>
# copies the library to <folder>/lib, the headers to <folder>/include
# and the app to <folder>/bin.
install(TARGETS numcomp2 ARCHIVE DESTINATION lib LIBRARY DESTINATION lib)
install(DIRECTORY include/ DESTINATION include)
install(TARGETS numcomp_app RUNTIME DESTINATION bin)
