# Check if the project has a parent.
# If it does, it means that this was included as a subproject via
# add_subdirectory(...).

get_directory_property( PROJECT_HAS_PARENT PARENT_DIRECTORY )



macro( request_build_platform PARAMETER_NAME )

    set( ${PARAMETER_NAME} "" CACHE STRING "Platform the project is built for" )

    set_property(
        CACHE ${PARAMETER_NAME} PROPERTY STRINGS
        "Linux"   # GLFW-based project using native OpenGL
        "Browser" # emscripten project using Weblog
        "Android" # NDK library to be used in an android application
    )

    if( NOT ${PARAMETER_NAME} )
        message( FATAL_ERROR "Build platform must be specified" )
    endif()

endmacro()


macro( request_build_type )

    if( NOT CMAKE_BUILD_TYPE )

        set( CMAKE_BUILD_TYPE "Debug" CACHE STRING "Type of build" FORCE )

        set_property(
            CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS
            "Release"
            "Debug"
        )

    endif()

endmacro()


macro( request_library_type PARAMETER_NAME )

    set( ${PARAMETER_NAME} "" CACHE STRING "Type of library that is built" )

    set_property(
        CACHE ${PARAMETER_NAME} PROPERTY STRINGS
        "SHARED"
        "STATIC"
    )

    if( NOT ${PARAMETER_NAME} )
        message( FATAL_ERROR "Library type must be specified" )
    endif()

endmacro()