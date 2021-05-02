cmake_minimum_required( VERSION 3.10 )

cmake_policy( SET CMP0076 NEW ) # allow relative target_sources

project( bla )

message( "TEST" )

set( TARGET_NAME "project_library" )

add_library( ${TARGET_NAME} SHARED )

message( STATUS "Including ${PROJECT_CMAKE_FILE}" )
add_subdirectory( ${PROJECT_CMAKE_FILE} "project_library" )

install( TARGETS ${TARGET_NAME} )
