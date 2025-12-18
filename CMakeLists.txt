cmake_minimum_required(VERSION 3.16)
project(PacManProject)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(logic)
add_subdirectory(app)
enable_testing()
add_subdirectory(tests)
