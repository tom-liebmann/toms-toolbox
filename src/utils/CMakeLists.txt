# find source files
file(
    GLOB_RECURSE
    SOURCE_FILES
    ABSOLUTE ${CMAKE_CURRENT_SOURCE_DIR}
    *.cpp )

if( ${COMPILATION_MODE} STREQUAL "GCC" )
    add_library(
        ttbUtils
        SHARED
        ${SOURCE_FILES} )
elseif( ${COMPILATION_MODE} STREQUAL "CLANG" )
    add_library(
        ttbUtils
        STATIC
        ${SOURCE_FILES}
        )
endif()

target_compile_options(
    ttbUtils
    PUBLIC
    -std=c++14
    -Wall
    -Wextra
    -Wpedantic
    -Wconversion
    )

target_include_directories(
    ttbUtils
    PUBLIC
    include )

list( APPEND VALID_TARGETS ttbUtils )
set( VALID_TARGETS ${VALID_TARGETS} PARENT_SCOPE )
