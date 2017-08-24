cmake_minimum_required( VERSION 3.0 )

project( toms_toolbox )

set( CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH};${CMAKE_CURRENT_SOURCE_DIR}/cmake" )

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
