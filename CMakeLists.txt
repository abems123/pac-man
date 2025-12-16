cmake_minimum_required(VERSION 3.16)
project(PacManProject)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# SFML 2.6.1 detection
find_package(SFML 2.5 COMPONENTS graphics window system REQUIRED)

add_subdirectory(logic)
add_subdirectory(app)
enable_testing()
add_subdirectory(tests)
