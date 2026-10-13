# ==========================================
# Group 23: Unit Test Build Configuration
# ==========================================

# 1. Sharon's Basic Integration Tests
add_executable(test_basic_integration test_basic_integration.cpp)
# Link it to our main library (assuming Jairus named the main library 'numcomp')
target_link_libraries(test_basic_integration numcomp)

# 2. George's Advanced Integration Tests
add_executable(test_advanced_integration test_advanced_integration.cpp)
target_link_libraries(test_advanced_integration numcomp)

# 3. Valeria's Advanced Root Finder Tests
add_executable(test_advanced_root_finder test_advanced_root_finder.cpp)
target_link_libraries(test_advanced_root_finder numcomp)

# (Derrick's tests will be added here next week when he pushes them!)