set( TTB_ROOT "${CMAKE_CURRENT_LIST_DIR}/.." )

cmake_policy( SET CMP0079 NEW )

list( APPEND CMAKE_MODULE_PATH "${TTB_ROOT}/cmake" )
list( APPEND CMAKE_MODULE_PATH "${TTB_ROOT}/project/platforms" )

include( ttb_general )

macro( ttb_add_project PROJECT_NAME PROJECT_CMAKE_FILE )

    request_build_platform( BUILD_PLATFORM )

    request_build_type()

    include( ttb_platform_${BUILD_PLATFORM} )

    _ttb_add_project_impl( ${PROJECT_NAME} ${PROJECT_CMAKE_FILE} )

endmacro()

macro( ttb_project_assets ASSET_DIR )

    request_build_platform( BUILD_PLATFORM )

    include( ttb_platform_${BUILD_PLATFORM} )

    _ttb_project_assets_impl( ${ASSET_DIR} )

endmacro()
