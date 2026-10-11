cmake_minimum_required(VERSION 3.10)

project(NumericalComputing2 VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include_directories(include)

add_library(numcomp2 
    src/root_finding.cpp
    src/adv_integration.cpp
)
