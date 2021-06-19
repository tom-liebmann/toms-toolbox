set( TTB_ROOT "${CMAKE_CURRENT_LIST_DIR}/.." )

cmake_policy( SET CMP0079 NEW )

list( APPEND CMAKE_MODULE_PATH "${TTB_ROOT}/cmake" )
list( APPEND CMAKE_MODULE_PATH "${TTB_ROOT}/project/platforms" )

include( ttb_general )

macro( ttb_init_project PROJECT_CMAKE_FILE )

    request_build_platform( BUILD_PLATFORM )

    request_build_type()

    include( ttb_platform_${BUILD_PLATFORM} )

    _ttb_init_project_impl( ${PROJECT_CMAKE_FILE} )

endmacro()
