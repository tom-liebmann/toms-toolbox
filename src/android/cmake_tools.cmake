set( TTB_ANDROID_TOOLS_DIR "${CMAKE_CURRENT_LIST_DIR}" )

function( ttb_create_android_target PLATFORM PROJECT_CMAKE_FILE )

    include( ExternalProject )

    set( LIBRARY_NAME "project_library_${PLATFORM}" )

    ExternalProject_Add(
        "${LIBRARY_NAME}"
        PREFIX "${LIBRARY_NAME}"
        SOURCE_DIR "${TTB_ANDROID_TOOLS_DIR}"
        CMAKE_ARGS
            -DANDROID_PLATFORM=android-30
            -DCMAKE_TOOLCHAIN_FILE=/home/tom/libraries/android_sdk/ndk/22.1.7171670/build/cmake/android.toolchain.cmake
            -DPROJECT_CMAKE_FILE=${PROJECT_CMAKE_FILE}
            -DCMAKE_INSTALL_PREFIX=${CMAKE_CURRENT_BINARY_DIR}/${LIBRARY_NAME}
    )

endfunction()

macro( ttb_init_android_project PROJECT_CMAKE_FILE )

    set( ANDROID_PACKAGE_NAME "" CACHE STRING "Name of the android package" )
    set( ANDROID_SDK_VERSION_MIN "" CACHE STRING "Lowest supported SDK version" )
    set( ANDROID_SDK_VERSION_MAX "" CACHE STRING "Highest supported SDK version" )
    set( ANDROID_SDK_VERSION_TARGET "" CACHE STRING "SDK version target" )

    if( NOT ANDROID_PACKAGE_NAME )
        message( FATAL_ERROR "ANDROID_PACKAGE_NAME not set" )
    endif()

    ttb_create_android_target( "test" "${PROJECT_CMAKE_FILE}" )

endmacro()

macro( ttb_init_project PROJECT_NAME )

    set( BUILD_PLATFORM "" CACHE STRING "Platform the project is built for" )
    set_property(
        CACHE BUILD_PLATFORM PROPERTY STRINGS
        "Linux"   # GLFW-based project using native OpenGL
        "Browser" # emscripten project using Weblog
        "Android" # NDK library to be used in an android application
    )

    set( CMAKE_BUILD_TYPE "Debug" CACHE STRING "Type of build" FORCE )
    set_property(
        CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS
        "Release"
        "Debug"
    )

    if( NOT BUILD_PLATFORM )
        message( FATAL_ERROR "Build platform must be specified" )
    endif()

    if( NOT CMAKE_BUILD_TYPE )
        message( FATAL_ERROR "Build type must be specified" )
    endif()

    if( "${BUILD_PLATFORM}" STREQUAL "Android" )
        ttb_init_android_project( ${PROJECT_NAME} )
    else()
        message( FATAL_ERROR "Build platform ${BUILD_PLATFORM} not fully supported yet" )
    endif()

endmacro()