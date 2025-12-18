# SFML 2.6.1 detection
find_package(SFML 2.6.1 COMPONENTS graphics window system REQUIRED)


# Collect all .cpp files in the app/src directory
file(GLOB_RECURSE APP_SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
)

# Build the application
add_executable(pacman_app
        ${APP_SOURCES}
)

# Link with the logic library and SFML
target_link_libraries(pacman_app
        PRIVATE
        paclogic
        sfml-graphics
        sfml-window
        sfml-system
)

# Include path for app headers
target_include_directories(pacman_app
        PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)