# find source files
file(
    GLOB_RECURSE
    SOURCE_FILES
    ABSOLUTE ${CMAKE_CURRENT_SOURCE_DIR}
    *.cpp
)

add_library(
    ttbUtils
    SHARED
    ${SOURCE_FILES}
)

target_include_directories(
    ttbUtils
    PUBLIC
        include
)

target_compile_options(
    ttbUtils
    PUBLIC
        -Wall
        -Wextra
        -Wpedantic
)

set_target_properties(
    ttbUtils
    PROPERTIES
        POSITION_INDEPENDENT_CODE ON
        CXX_STANDARD 17
)

target_compile_features(
    ttbUtils
    PUBLIC
        cxx_std_17
)

list( APPEND VALID_TARGETS ttbUtils )
set( VALID_TARGETS ${VALID_TARGETS} PARENT_SCOPE )
