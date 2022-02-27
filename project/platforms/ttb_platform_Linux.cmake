include_guard( GLOBAL )

set( TTB_ROOT_DIR "${CMAKE_CURRENT_LIST_DIR}/../.." )

macro( _ttb_add_project_impl PROJECT_NAME PROJECT_CMAKE_FILE )

    cmake_policy( SET CMP0076 NEW )

    set( PROJECT_NAME ${PROJECT_NAME} )

    set( TARGET_NAME ${PROJECT_NAME} )

    enable_language( C )
    enable_language( CXX )

    add_executable( ${TARGET_NAME} )

    find_package( ttb REQUIRED )
    target_link_libraries( ${TARGET_NAME} PRIVATE ttb::core ttb::utils ttb::ui ttb::math )

    string( TOUPPER ${BUILD_PLATFORM} BUILD_PLATFORM_UPPER )
    target_compile_definitions( ${TARGET_NAME} PUBLIC PLATFORM_${BUILD_PLATFORM_UPPER} )

    add_subdirectory( ${PROJECT_CMAKE_FILE} _target_${PROJECT_NAME} )

    add_subdirectory( "${TTB_ROOT}/project/src" ttb_project )

endmacro()

macro( _ttb_project_assets_impl ASSET_DIR )

    # Do nothing for now. Later, this macro could be used to bundle assets into a data blob

endmacro()
