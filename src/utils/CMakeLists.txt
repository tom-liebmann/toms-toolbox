# find source files
file(
    GLOB_RECURSE
    SOURCE_FILES
    ABSOLUTE ${CMAKE_CURRENT_SOURCE_DIR}
    *.cpp )

add_library(
    ttbUtils
    STATIC
    ${SOURCE_FILES} )

target_include_directories(
    ttbUtils
    PUBLIC
    include )

list( APPEND VALID_TARGETS ttbUtils )
set( VALID_TARGETS ${VALID_TARGETS} PARENT_SCOPE )
