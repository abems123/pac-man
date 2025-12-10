# Build the logic library (currently empty)
add_library(paclogic STATIC)

# Public include directory (even if empty now)
target_include_directories(paclogic
        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)