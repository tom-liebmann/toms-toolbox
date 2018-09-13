# find source files
file(
    GLOB_RECURSE
    SOURCE_FILES
    ABSOLUTE "${CMAKE_CURRENT_SOURCE_DIR}"
    src/*.cpp
)

if( ${COMPILATION_MODE} STREQUAL "GCC" )
    add_library(
        ttbMath
        SHARED
        ${SOURCE_FILES} )
elseif( ${COMPILATION_MODE} STREQUAL "CLANG" )
    add_library(
        ttbMath
        STATIC
        ${SOURCE_FILES}
        )
endif()

target_include_directories(
    ttbMath
    PUBLIC
    include
    PRIVATE
    src )

target_compile_features(
    ttbMath
    PUBLIC
    cxx_return_type_deduction
    )

set_property(
    TARGET
    ttbMath
    PROPERTY POSITION_INDEPENDENT_CODE ON)

list( APPEND VALID_TARGETS ttbMath )
set( VALID_TARGETS ${VALID_TARGETS} PARENT_SCOPE )
