cmake_minimum_required( VERSION 3.15 )

list( APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake" )

set( BUILD_PLATFORM "" CACHE STRING "Platform the project is built for" )
set_property(
    CACHE BUILD_PLATFORM PROPERTY STRINGS
    "Linux"   # GLFW-based project using native OpenGL
    "Browser" # emscripten project using Weblog
    "Android" # NDK library to be used in an android application
)

if( NOT BUILD_PLATFORM )
    message( FATAL_ERROR "Build platform must be specified" )
endif()

if( NOT CMAKE_BUILD_TYPE )
    set( CMAKE_BUILD_TYPE "Debug" CACHE STRING "Type of build" FORCE )
    set_property(
        CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS
        "Release"
        "Debug"
    )
endif()

if( ${BUILD_SUBMODULE} )

    message( "Building toolbox as submodule" )

else()

    if( "${BUILD_PLATFORM}" STREQUAL "Linux" )

        project( toms_toolbox )

        if( "${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")

            set( REQUIRED_GCC_VERSION "9.0.0" )

            if( CMAKE_CXX_COMPILER_VERSION VERSION_LESS ${REQUIRED_GCC_VERSION})

                message(FATAL_ERROR "Insufficient gcc version (found ${CMAKE_CXX_COMPILER_VERSION}, need ${REQUIRED_GCC_VERSION})")

            endif()

            message( "Using GCC version ${CMAKE_CXX_COMPILER_VERSION}" )

        endif()

    elseif( "${BUILD_PLATFORM}" STREQUAL "Browser" )

        set( CMAKE_TOOLCHAIN_FILE $ENV{EMSCRIPTEN}/cmake/Modules/Platform/Emscripten.cmake CACHE PATH "Emscripten toolchain file" )

        project( toms_toolbox )

    elseif( "${BUILD_PLATFORM}" STREQUAL "Android" )

        project( toms_toolbox )

    endif()

endif()

# Generate documentation
find_package( Doxygen )

if( DOXYGEN_FOUND )

    configure_file(
        ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in
        ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
        @ONLY
    )

    add_custom_target(
        doc
        ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        COMMENT "Generating API documentation with Doxygen" VERBATIM
    )

endif( DOXYGEN_FOUND )

set( LIBRARY_TYPE "" CACHE STRING "Type of library that is built" )
set_property(
    CACHE LIBRARY_TYPE PROPERTY STRINGS
    "SHARED"
    "STATIC"
    "OBJECT"
)

if( NOT LIBRARY_TYPE )
    message( FATAL_ERROR "Library type must be specified" )
endif()

add_library( ttb ${LIBRARY_TYPE} )

set(EXPORT_TARGETS "ttb")

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
    add_library( ${MODULE_NAME} OBJECT EXCLUDE_FROM_ALL )

    target_sources( ${MODULE_NAME} PRIVATE "${CMAKE_CURRENT_FUNCTION_LIST_DIR}/src/dummy.cpp" )

    # add compiler definitions
    # ================================================================
    string( TOUPPER ${BUILD_PLATFORM} BUILD_PLATFORM_UPPER )
    target_compile_definitions( ${MODULE_NAME} PRIVATE PLATFORM_${BUILD_PLATFORM_UPPER} )

    # set compile options
    # ================================================================
    target_compile_options( ${MODULE_NAME} PRIVATE -Wall -Wextra -Wpedantic )

    target_compile_features( ${MODULE_NAME} PUBLIC cxx_std_17 )

    set_target_properties( ${MODULE_NAME} PROPERTIES POSITION_INDEPENDENT_CODE ON )

    # link module to main target
    # ================================================================
    target_link_libraries( ttb PUBLIC ${MODULE_NAME} )

    list( APPEND EXPORT_TARGETS ${MODULE_NAME} )
    set( EXPORT_TARGETS ${EXPORT_TARGETS} PARENT_SCOPE )

endfunction()

add_subdirectory( src/math )
add_subdirectory( src/core )
add_subdirectory( src/utils )
add_subdirectory( src/net )

export(
    TARGETS ${EXPORT_TARGETS}
    FILE "${PROJECT_BINARY_DIR}/TTBTargets.cmake"
)

configure_file(
    TTBConfig.cmake.in
    "${CMAKE_CURRENT_BINARY_DIR}/TTBConfig.cmake"
)
