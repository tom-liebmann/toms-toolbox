cmake_minimum_required( VERSION 3.10 )

set( CMAKE_TOOLCHAIN_FILE "${CMAKE_CURRENT_BINARY_DIR}/conan_toolchain.cmake" )

cmake_policy( SET CMP0076 NEW )
cmake_policy( SET CMP0079 NEW )

set( CMAKE_EXPORT_COMPILE_COMMANDS ON )
set( PROJECT_NAME "ttb-project" )

project( ${PROJECT_NAME} )

set( TARGET_NAME ${PROJECT_NAME} )
add_library( ${PROJECT_NAME} SHARED )

find_package( ttb REQUIRED )
target_link_libraries( ${TARGET_NAME} PRIVATE ttb::core ttb::utils ttb::math )
target_compile_options( ${TARGET_NAME} PRIVATE -fcoroutines )

add_subdirectory( src )
