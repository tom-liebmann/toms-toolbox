cmake_minimum_required( VERSION 3.10 )

cmake_policy( SET CMP0076 NEW ) # allow relative target_sources

project( ${PROJECT_NAME} )

set( TARGET_NAME ${PROJECT_NAME} )

add_library( ${TARGET_NAME} SHARED )

set( BUILD_PLATFORM "Android" CACHE PATH "" FORCE )
set( LIBRARY_TYPE "Object" CACHE STRING "" FORCE )
add_subdirectory( "${CMAKE_CURRENT_LIST_DIR}/../.." toms-toolbox )

add_subdirectory( ${PROJECT_CMAKE_FILE} ${PROJECT_NAME} )

target_link_libraries(
    ${TARGET_NAME}
    PRIVATE
        android
        log
        ttb
)

install( TARGETS ${TARGET_NAME} )
