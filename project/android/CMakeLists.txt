cmake_minimum_required( VERSION 3.10 )

set( TTB_ROOT "${CMAKE_CURRENT_LIST_DIR}/../.." )

cmake_policy( SET CMP0076 NEW ) # allow relative target_sources
cmake_policy( SET CMP0079 NEW ) # allow out-of-tree linkage

project( ${PROJECT_NAME} )

set( TARGET_NAME ${PROJECT_NAME} )

add_library( ${TARGET_NAME} SHARED )

add_subdirectory( ${PROJECT_CMAKE_FILE} ${PROJECT_NAME} )

add_subdirectory( "${TTB_ROOT}" toms-toolbox )

add_subdirectory( "${TTB_ROOT}/project/src" ttb_project )

target_link_libraries(
    ${TARGET_NAME}
    PRIVATE
        android
        log
        ttb
)

install( TARGETS ${TARGET_NAME} )
