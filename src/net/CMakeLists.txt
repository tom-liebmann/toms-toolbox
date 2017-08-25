# find source files
file(
    GLOB_RECURSE
    SOURCE_FILES
    ABSOLUTE "${CMAKE_CURRENT_SOURCE_DIR}"
    src/*.cpp )

add_library(
    ttbNet
    STATIC
    ${SOURCE_FILES} )

target_include_directories(
    ttbNet
    PUBLIC
    include
    PRIVATE
    src )

target_compile_options(
    ttbNet
    PRIVATE
    -std=gnu++14
    -fPIC )

target_link_libraries(
    ttbNet
    PUBLIC
    ttbUtils )

if( WIN32 )

    target_link_libraries(
        ttbNet
        PUBLIC
        wsock32
        ws2_32 )

    target_compile_definitions(
        ttbNet
        PUBLIC
        WIN32 )

endif()

list( APPEND VALID_TARGETS ttbNet )
set( VALID_TARGETS ${VALID_TARGETS} PARENT_SCOPE )
