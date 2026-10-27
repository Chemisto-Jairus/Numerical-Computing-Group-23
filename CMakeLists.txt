# ==========================================
# Group 23: Numerical Computing 2 library
# Top-level build file. Owner: Chemisto Jairus (Member 1)
# ==========================================
# Builds:
#   numcomp2     the static library (everything in include/ and src/ except the app)
#   numcomp_app  the interactive command-line app (src/main.cpp + src/app/)
#   tests/       one test program per module, run with ctest
#   examples/    two demo programs per module, also run by ctest
#
# Typical use, from the repository root:
#   cmake -S . -B build
#   cmake --build build
#   cd build && ctest --output-on-failure
cmake_minimum_required(VERSION 3.10)

project(NumericalComputing2 VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Optimised build unless another type is chosen (for example -DCMAKE_BUILD_TYPE=Debug).
# Visual Studio picks the type at build time instead (cmake --build build --config Release).
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
endif()

# Public headers are included as "numerical-computing/<module>.hpp".
include_directories(include)

# ---- Library ------------------------------------------------------------
set(NUMCOMP_SOURCES
    src/basic_root_finder.cpp
    src/interpolator.cpp
    src/basic_integrator.cpp
    src/advanced_root_finder.cpp
    src/advanced_integrator.cpp
    src/basic_ode_solver.cpp
    src/advanced_ode_solver.cpp
    src/cubic_spline.cpp
    src/matrix_solver.cpp
    src/numerical_differentiator.cpp
    # Internal helpers shared by the modules (not part of the public API)
    src/internal/input_checks.cpp
    src/internal/ode_stepper.cpp
    src/internal/multistep.cpp
    src/internal/tridiagonal.cpp
)

# Modules added in Week 5 by different members. Each is compiled only when
# its file exists, so a pull request that is not merged yet cannot break the build.
set(NUMCOMP_WEEK5_MODULES
    src/conjugate_gradient_solver.cpp       # Valeria
    src/adams_bashforth_solver.cpp          # Ian
    src/adams_moulton_solver.cpp            # Samuel
    src/runge_kutta_fehlberg_solver.cpp     # George
    src/finite_difference_solver.cpp        # Sharon
)
foreach(source ${NUMCOMP_WEEK5_MODULES})
    if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/${source})
        list(APPEND NUMCOMP_SOURCES ${source})
    endif()
endforeach()

add_library(numcomp2 ${NUMCOMP_SOURCES})

# ---- Tests and examples (registered with ctest) ------------------------
enable_testing()
add_subdirectory(tests)
# Built only when the examples folder has its own CMakeLists.txt.
if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/examples/CMakeLists.txt)
    add_subdirectory(examples)
endif()

# ---- Command-line app ---------------------------------------------------
add_executable(numcomp_app
    src/main.cpp
    src/app/console_ui.cpp
    src/app/catalog.cpp
    src/app/module_menus.cpp
)
target_link_libraries(numcomp_app numcomp2)

# ---- Installation -------------------------------------------------------
#   cmake --install build --prefix <folder>
# installs the library to <folder>/lib, the public headers to
# <folder>/include/numerical-computing and the app to <folder>/bin.
install(TARGETS numcomp2 ARCHIVE DESTINATION lib LIBRARY DESTINATION lib)
install(DIRECTORY include/ DESTINATION include)
install(TARGETS numcomp_app RUNTIME DESTINATION bin)
