# find source files
file(
    GLOB_RECURSE
    SOURCE_FILES
    ABSOLUTE "${CMAKE_CURRENT_SOURCE_DIR}"
    src/*.cpp
)

add_library(
    ttbMath
    SHARED
    ${SOURCE_FILES}
)

target_include_directories(
    ttbMath
    PUBLIC
        include
    PRIVATE
        src
    )

target_compile_options(
    ttbMath
    PUBLIC
        -Wall
        -Wextra
        -Wpedantic
    )

set_target_properties(
    ttbMath
    PROPERTIES
        POSITION_INDEPENDENT_CODE ON
        CXX_STANDARD 17
    )

target_compile_features(
    ttbMath
    PUBLIC
        cxx_std_17
    )

list( APPEND VALID_TARGETS ttbMath )
set( VALID_TARGETS ${VALID_TARGETS} PARENT_SCOPE )
