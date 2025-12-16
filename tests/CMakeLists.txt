# tests/CMakeLists.txt

cmake_minimum_required(VERSION 3.16)

# Download GoogleTest
include(FetchContent)

FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
)

# Prevent GoogleTest from overriding compiler flags
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# Create test executable
add_executable(pacman_tests
        TestStopwatch.cpp
)

# Link logic library + GoogleTest
target_link_libraries(pacman_tests
        PRIVATE
        paclogic
        gtest_main
)

# Add tests to CTest
include(GoogleTest)
gtest_discover_tests(pacman_tests)
