cmake_minimum_required( VERSION 3.0 )

project( toms_toolbox )

set( CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH};${CMAKE_CURRENT_SOURCE_DIR}/cmake" )

add_subdirectory( src/math )
add_subdirectory( src/core )
add_subdirectory( src/net )
add_subdirectory( src/utils )

# Export build tree to Targets.cmake file.
export(
    TARGETS ttbCore ttbMath ttbNet ttbUtils
    FILE "${PROJECT_BINARY_DIR}/TTBTargets.cmake" )

configure_file(
    TTBConfig.cmake.in
    "${CMAKE_CURRENT_BINARY_DIR}/TTBConfig.cmake" )
