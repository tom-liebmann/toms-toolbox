# find source files
file(
    GLOB_RECURSE
    SOURCE_FILES
    ABSOLUTE "${CMAKE_CURRENT_SOURCE_DIR}"
    src/*.cpp
)

add_library(
    ttbMath
    STATIC
    ${SOURCE_FILES} )

target_include_directories(
    ttbMath
    PUBLIC
    include
    PRIVATE
    src )

target_compile_options(
    ttbMath
    PRIVATE
    -std=gnu++14
    -fPIC )

if( WIN32 )

    target_compile_definitions(
        ttbMath
        PRIVATE
        WIN32 )

endif()
