list( APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/platforms" )

include( ttb_tools )

macro( ttb_init_project PROJECT_CMAKE_FILE )

    request_build_platform( BUILD_PLATFORM )

    request_build_type()

    include( ttb_platform_${BUILD_PLATFORM} )

    _ttb_init_project_impl( ${PROJECT_CMAKE_FILE} )

endmacro()