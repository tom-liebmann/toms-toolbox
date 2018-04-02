cmake_minimum_required( VERSION 3.2 )

set( CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH};${CMAKE_CURRENT_SOURCE_DIR}/cmake" )

set( COMPILATION_MODE "UNKNOWN" CACHE STRING "Mode to compile the libraries in" )
set_property( CACHE COMPILATION_MODE PROPERTY STRINGS GCC CLANG )

if( ${COMPILATION_MODE} STREQUAL "GCC" )

    message( "Using gcc" )
    set( CMAKE_C_COMPILER "gcc" )
    set( CMAKE_CXX_COMPILER "g++" )

elseif( ${COMPILATION_MODE} STREQUAL "CLANG" )

    message( "Using clang" )
    set( CMAKE_TOOLCHAIN_FILE $ENV{EMSCRIPTEN}/cmake/Modules/Platform/Emscripten.cmake CACHE PATH "Emscripten toolchain file" )

else()

    message( FATAL_ERROR "Unknown compilation mode" )

endif()

project( toms_toolbox )

# Generate documentation
find_package( Doxygen )

if( DOXYGEN_FOUND )

    configure_file(
        ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in
        ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
        @ONLY )

    add_custom_target(
        doc
        ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        COMMENT "Generating API documentation with Doxygen" VERBATIM )

endif( DOXYGEN_FOUND )

set( VALID_TARGETS )

add_subdirectory( src/math )
add_subdirectory( src/core )
add_subdirectory( src/net )
add_subdirectory( src/utils )

# Export build tree to Targets.cmake file.
export(
    TARGETS ${VALID_TARGETS}
    FILE "${PROJECT_BINARY_DIR}/TTBTargets.cmake" )

configure_file(
    TTBConfig.cmake.in
    "${CMAKE_CURRENT_BINARY_DIR}/TTBConfig.cmake" )
