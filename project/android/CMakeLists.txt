cmake_minimum_required( VERSION 3.24 )

set( TTB_ROOT "${CMAKE_CURRENT_LIST_DIR}/../.." )

list( APPEND CMAKE_MODULE_PATH "${PROJECT_CONAN_DIR}" )
list( APPEND CMAKE_PREFIX_PATH "${PROJECT_CONAN_DIR}" )

cmake_policy( SET CMP0076 NEW ) # allow relative target_sources
cmake_policy( SET CMP0079 NEW ) # allow out-of-tree linkage

# To locate and collect all library files for packaging, packages added in
# subdirectories have to be put into the global scope. Otherwise, the library
# targets would not be available in this file's scope.
set( CMAKE_FIND_PACKAGE_TARGETS_GLOBAL TRUE )

project( ${PROJECT_NAME} )

set( TARGET_NAME "project_library" )

add_library( ${TARGET_NAME} SHARED )

target_compile_features( ${TARGET_NAME} PUBLIC cxx_std_20 )
target_compile_definitions( ${TARGET_NAME} PUBLIC PLATFORM_ANDROID )

find_package( ttb REQUIRED )

target_link_libraries( ${TARGET_NAME} PRIVATE
    ttb::ttb
    GLESv3
    android
    log
)

add_subdirectory( ${PROJECT_CMAKE_FILE} ${PROJECT_NAME} )

add_subdirectory( "${TTB_ROOT}/project/src" ttb_project )

string( REPLACE ";" "\n" _LIBRARY_PATH_LINES "${CMAKE_LIBRARY_PATH}" )
file( WRITE ${PROJECT_LIB_PATH_FILE} "${_LIBRARY_PATH_LINES}" )

install( TARGETS ${TARGET_NAME} DESTINATION "." )
