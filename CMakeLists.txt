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

add_subdirectory( src/math )
add_subdirectory( src/core )
add_subdirectory( src/utils )
#add_subdirectory( src/net )
