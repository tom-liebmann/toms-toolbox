set( TTB_ROOT "${CMAKE_CURRENT_LIST_DIR}/.." )

cmake_policy( SET CMP0079 NEW )

list( APPEND CMAKE_MODULE_PATH "${TTB_ROOT}/cmake" )
list( APPEND CMAKE_MODULE_PATH "${TTB_ROOT}/project/platforms" )

include( ttb_general )
include( ttb_utils )

function( ttb_project_create PROJECT_NAME )
    add_custom_target( ${PROJECT_NAME} ALL )
endfunction()

function( ttb_project_finish PROJECT_NAME )
    _ttb_project_check_required_property( ${PROJECT_NAME} TTB_CMAKE_FILE )
    _ttb_project_check_required_property( ${PROJECT_NAME} TTB_CONAN_FILE )

    request_build_platform( BUILD_PLATFORM )

    include( ttb_platform_${BUILD_PLATFORM} )

    _ttb_project_finish( ${PROJECT_NAME} )
endfunction()

function( ttb_project_set_conan_file PROJECT_NAME CONAN_FILE )
    set_property(
        TARGET ${PROJECT_NAME}
        PROPERTY TTB_CONAN_FILE ${CONAN_FILE}
    )
endfunction()

function( ttb_project_set_cmake_file PROJECT_NAME CMAKE_FILE )
    set_property(
        TARGET ${PROJECT_NAME}
        PROPERTY TTB_CMAKE_FILE ${CMAKE_FILE}
    )
endfunction()

function( ttb_project_add_android_abi PROJECT_NAME ABI )
    set_property(
        TARGET ${PROJECT_NAME}
        APPEND
        PROPERTY TTB_ANDROID_ABI ${ABI}
    )
endfunction()

function( ttb_project_add_assets PROJECT_NAME ASSET_DIR )
    set_property(
        TARGET ${PROJECT_NAME}
        APPEND
        PROPERTY TTB_ASSET_DIRECTORIES ${ASSERT_DIR}
    )
endfunction()

function( _ttb_project_check_required_property TARGET PROPERTY )
    get_property(
        _PROPERTY_VALUE
        TARGET ${TARGET}
        PROPERTY ${PROPERTY}
    )

    message( STATUS "${_PROPERTY_VALUE}" )

    if( "${_PROPERTY_VALUE}" STREQUAL "" )
        message( FATAL_ERROR "Property ${PROPERTY} missing for target ${TARGET}" )
    endif()
endfunction()
