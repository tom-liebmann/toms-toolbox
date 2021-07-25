set( TTB_ROOT_DIR "${CMAKE_CURRENT_LIST_DIR}/../.." )

macro( _ttb_add_project_impl PROJECT_NAME PROJECT_CMAKE_FILE )

    cmake_policy( SET CMP0076 NEW )

    set( PROJECT_NAME ${PROJECT_NAME} )

    set( TARGET_NAME ${PROJECT_NAME} )

    enable_language( C )
    enable_language( CXX )

    add_executable( ${TARGET_NAME} )

    find_package( ttb REQUIRED )
    target_link_libraries( ${TARGET_NAME} PRIVATE ttbCore ttbUtils ttbUi ttbMath )

    add_subdirectory( ${PROJECT_CMAKE_FILE} _target_${PROJECT_NAME} )

    add_subdirectory( "${TTB_ROOT}/project/src" ttb_project )

endmacro()
