set( TTB_ROOT_DIR "${CMAKE_CURRENT_LIST_DIR}/../.." )

macro( _ttb_init_project_impl PROJECT_CMAKE_FILE )

    cmake_policy( SET CMP0076 NEW )

    # TODO: pass in as parameter
    set( PROJECT_NAME "linux_project" )

    set( TARGET_NAME ${PROJECT_NAME} )

    enable_language( C )
    enable_language( CXX )

    add_executable( ${TARGET_NAME} )

    add_subdirectory( ${PROJECT_CMAKE_FILE} _target_${PROJECT_NAME} )

    add_subdirectory( "${TTB_ROOT}/project/src" ttb_project )

    add_subdirectory( "${TTB_ROOT_DIR}" toms-toolbox )

    target_link_libraries(
        ${TARGET_NAME}
        PRIVATE
            ttb
    )

endmacro()