include_guard( GLOBAL )

set( TTB_MODULE_FUNCTIONS_DIR ${CMAKE_CURRENT_LIST_DIR} )

macro( ttb_init_testing )
    set( BUILD_TESTS "OFF" CACHE BOOL "Whether to build tests" )

    if( ${BUILD_TESTS} )
        enable_testing()
    endif()
endmacro()

function( ttb_add_test )

    if( NOT ${BUILD_TESTS} )
        return()
    endif()

    if( NOT TARGET check )
        add_custom_target( check ${CMAKE_CTEST_COMMAND} )
    endif()

    find_package( Catch2 REQUIRED )

    if( NOT TARGET test_main )
        add_library(
            test_main
            OBJECT
            ${TTB_MODULE_FUNCTIONS_DIR}/src/test_main.cpp
            )

        target_link_libraries( test_main PRIVATE Catch2::Catch2 )
    endif()

    cmake_parse_arguments(
        ADD_TEST
        ""
        "TARGET_NAME"
        "SOURCES;DEPENDENCIES;INCLUDES"
        ${ARGN}
    )

    # Create test target
    add_executable(
        ${ADD_TEST_TARGET_NAME}
        ${ADD_TEST_SOURCES}
    )

    target_link_libraries( ${ADD_TEST_TARGET_NAME} PRIVATE test_main Catch2::Catch2 ${ADD_TEST_DEPENDENCIES} )

    target_include_directories( ${ADD_TEST_TARGET_NAME} PRIVATE ${ADD_TEST_INCLUDES} )

    target_compile_definitions( ${ADD_TEST_TARGET_NAME} PRIVATE -DTEST )

    add_test( NAME ${ADD_TEST_TARGET_NAME} COMMAND $<TARGET_FILE:${ADD_TEST_TARGET_NAME}> )

    add_dependencies( check ${ADD_TEST_TARGET_NAME} )

    set( TARGET_NAME ${ADD_TEST_TARGET_NAME} PARENT_SCOPE )

endfunction()

function( ttb_add_module MODULE_NAME MODULE_ALIAS )

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
    add_library( ${MODULE_ALIAS} ALIAS ${MODULE_NAME} )

    target_sources( ${MODULE_NAME} PRIVATE "${TTB_MODULE_FUNCTIONS_DIR}/src/dummy.cpp" )

    # add compiler definitions
    # ================================================================
    string( TOUPPER ${BUILD_PLATFORM} BUILD_PLATFORM_UPPER )
    target_compile_definitions( ${MODULE_NAME} PUBLIC PLATFORM_${BUILD_PLATFORM_UPPER} )

    # set compile options
    # ================================================================
    target_compile_options( ${MODULE_NAME} PRIVATE -Wall -Wextra -Wpedantic )

    target_compile_features( ${MODULE_NAME} PUBLIC cxx_std_20 )

    set_target_properties(
        ${MODULE_NAME} PROPERTIES
        POSITION_INDEPENDENT_CODE ON
        EXPORT_NAME ${MODULE_ALIAS}
    )

    # link module to main target
    # ================================================================
    list( APPEND EXPORT_TARGETS ${MODULE_NAME} )
    set( EXPORT_TARGETS ${EXPORT_TARGETS} PARENT_SCOPE )

endfunction()

macro( ttb_add_module_dependency MODULE_NAME DEP_MODULE DEP_TARGET )
    # Find dependency
    find_package( ${DEP_MODULE} CONFIG REQUIRED GLOBAL )

    # Link dependency to module
    target_link_libraries( ${MODULE_NAME} PUBLIC ${DEP_TARGET} )

    # Add dependency to export set
    list( APPEND EXPORT_DEPENDENCIES "${DEP_MODULE}" )
    set( EXPORT_DEPENDENCIES ${EXPORT_DEPENDENCIES} PARENT_SCOPE )
endmacro()
