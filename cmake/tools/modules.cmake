function( ttb_add_module MODULE_NAME )

    # add cache option for enabling/disabling module
    # ================================================================
    set( ACTIVATE_${MODULE_NAME} ON CACHE BOOL "Enable module ${MODULE_NAME}" )

    if( NOT ACTIVATE_${MODULE_NAME} )
        message( STATUS "Skipping ttb module: ${MODULE_NAME}" )
        return()
    endif()

    message( STATUS "Adding ttb module: ${MODULE_NAME}" )

    # create the module target
    # ================================================================
    add_library( ${MODULE_NAME} ${BUILD_LIBRARY_TYPE} EXCLUDE_FROM_ALL )

    target_sources( ${MODULE_NAME} PRIVATE "${CMAKE_CURRENT_FUNCTION_LIST_DIR}/../../src/dummy.cpp" )

    # add compiler definitions
    # ================================================================
    string( TOUPPER ${BUILD_PLATFORM} BUILD_PLATFORM_UPPER )
    target_compile_definitions( ${MODULE_NAME} PUBLIC PLATFORM_${BUILD_PLATFORM_UPPER} )

    # set compile options
    # ================================================================
    target_compile_options( ${MODULE_NAME} PRIVATE -Wall -Wextra -Wpedantic )

    target_compile_features( ${MODULE_NAME} PUBLIC cxx_std_17 )

    set_target_properties( ${MODULE_NAME} PROPERTIES POSITION_INDEPENDENT_CODE ON )

    # link module to main target
    # ================================================================
    target_link_libraries( ttb INTERFACE ${MODULE_NAME} )

    list( APPEND EXPORT_TARGETS ${MODULE_NAME} )
    set( EXPORT_TARGETS ${EXPORT_TARGETS} PARENT_SCOPE )

endfunction()
