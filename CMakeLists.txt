cmake_minimum_required(VERSION 3.10)

project(NumericalComputing2 VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include_directories(include)

add_library
(numcomp2
 
    src/basic_root_finder.cpp
    src/interpolator.cpp
    src/basic_integrator.cpp
    src/advanced_root_finder.cpp
    src/advanced_integrator.cpp
    src/basic_ode_solver.cpp
    src/advanced_ode_solver.cpp
)
add_subdirectory(tests)

add_executable(numcomp_app src/main.cpp)
target_link_libraries(numcomp_app numcomp2)