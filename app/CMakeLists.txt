# Build the application
add_executable(pacman_app
        src/main.cpp
)

# Link with the logic library and SFML
target_link_libraries(pacman_app
        PRIVATE
        paclogic
        sfml-graphics
        sfml-window
        sfml-system
)

# Include path for app headers (if you later add include/)
target_include_directories(pacman_app
        PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)
