file(GLOB_RECURSE LOGIC_SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
)

add_library(paclogic STATIC
        ${LOGIC_SOURCES}
)

target_include_directories(paclogic PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)