function( ttb_add_test )

    set( BUILD_TESTS "OFF" CACHE BOOL "Whether to build tests" )

    if( ${BUILD_TESTS} )
        enable_testing()

        cmake_parse_arguments(
            ADD_TEST
            ""
            "TARGET_NAME"
            "SOURCES"
            ${ARGN}
        )

        # Create test target
        add_executable(
            ${ADD_TEST_TARGET_NAME}
            EXCLUDE_FROM_ALL
            ${ADD_TEST_SOURCES}
        )

        # Import and link catch2 test framework to target
        find_package( catch2 REQUIRED )
        target_link_libraries( ${ADD_TEST_TARGET_NAME} PRIVATE catch2 )

        # Propagate test using the catch cmake tools
        include( Catch )
        include( CTest )
        catch_discover_tests( ${ADD_TEST_TARGET_NAME} )
    endif()

endfunction()

function( ttb_add_module MODULE_NAME )

    # add cache option for enabling/disabling module
    # ================================================================
    set( ACTIVATE_${MODULE_NAME} OFF CACHE BOOL "Enable module ${MODULE_NAME}" )

    if( NOT ACTIVATE_${MODULE_NAME} )
        message( STATUS "Skipping ttb module: ${MODULE_NAME}" )
        return()
    endif()

    message( STATUS "Adding ttb module: ${MODULE_NAME}" )

    # create the module target
    # ================================================================
    add_library( ${MODULE_NAME} ${BUILD_LIBRARY_TYPE} )

    target_sources( ${MODULE_NAME} PRIVATE "${CMAKE_CURRENT_FUNCTION_LIST_DIR}/../src/dummy.cpp" )

    # add compiler definitions
    # ================================================================
    string( TOUPPER ${BUILD_PLATFORM} BUILD_PLATFORM_UPPER )
    target_compile_definitions( ${MODULE_NAME} PUBLIC PLATFORM_${BUILD_PLATFORM_UPPER} )

    # set compile options
    # ================================================================
    target_compile_options( ${MODULE_NAME} PRIVATE -Wall -Wextra -Wpedantic )

    target_compile_features( ${MODULE_NAME} PUBLIC cxx_std_20 )

    set_target_properties( ${MODULE_NAME} PROPERTIES POSITION_INDEPENDENT_CODE ON )

    # link module to main target
    # ================================================================
    list( APPEND EXPORT_TARGETS ${MODULE_NAME} )
    set( EXPORT_TARGETS ${EXPORT_TARGETS} PARENT_SCOPE )

endfunction()

function( ttb_add_module_dependency MODULE_NAME DEPENDENCY )
    # Find dependency
    find_package( ${DEPENDENCY} REQUIRED )

    # Link dependency to module
    target_link_libraries( ${MODULE_NAME} PUBLIC ${DEPENDENCY} )

    # Add dependency to export set
    list( APPEND EXPORT_DEPENDENCIES "${DEPENDENCY}" )
    set( EXPORT_DEPENDENCIES ${EXPORT_DEPENDENCIES} PARENT_SCOPE )
endfunction()
