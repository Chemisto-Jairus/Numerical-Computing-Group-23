# ==========================================
# Group 23: Unit Test Build Configuration
# ==========================================

# 1. Sharon's Basic Integration Tests
add_executable(test_basic_integration test_basic_integration.cpp)
target_link_libraries(test_basic_integration numcomp2)

# 2. George's Advanced Integration Tests
add_executable(test_advanced_integration test_advanced_integration.cpp)
target_link_libraries(test_advanced_integration numcomp2)

# 3. Derrick's Interpolation Tests
add_executable(test_basic_interpolation test_basic_interpolation.cpp)
target_link_libraries(test_basic_interpolation numcomp2)

# 4. Valeria's Advanced Root Finder Tests
add_executable(test_advanced_root_finder test_advanced_root_finder.cpp)
target_link_libraries(test_advanced_root_finder numcomp2)