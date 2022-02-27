cmake_minimum_required( VERSION 3.15 )

list( APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake" )

include( ${CMAKE_BINARY_DIR}/conan_paths.cmake )
include( ttb_general )
include( ttb_module_functions )

request_build_platform( BUILD_PLATFORM )

request_build_type()

ttb_init_testing()

if( PROJECT_HAS_PARENT )

    message( "Building toolbox as submodule" )

    set( TTB_PROJECT_TOOLS "${CMAKE_CURRENT_SOURCE_DIR}/project" )
    set( TTB_PROJECT_TOOLS ${TTB_PROJECT_TOOLS} PARENT_SCOPE )

    set( BUILD_LIBRARY_TYPE "STATIC" )

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

    request_library_type( BUILD_LIBRARY_TYPE )

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

set( EXPORT_TARGETS "" )
set( EXPORT_DEPENDENCIES "" )

add_subdirectory( src/math )
add_subdirectory( src/core )
add_subdirectory( src/utils )
add_subdirectory( src/ui )

export(
    TARGETS ${EXPORT_TARGETS}
    FILE "${PROJECT_BINARY_DIR}/ttbTargets.cmake"
)

configure_file(
    ttbConfig.cmake.in
    "${CMAKE_CURRENT_BINARY_DIR}/ttbConfig.cmake"
    @ONLY
)
