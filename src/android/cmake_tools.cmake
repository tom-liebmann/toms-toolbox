set( CURRENT_FUNCTION_DIR "${CMAKE_CURRENT_LIST_DIR}" )

function( ttb_create_android_target ANDROID_ABI PROJECT_CMAKE_FILE OUTPUT_LIB_DIR )

    include( ExternalProject )

    set( LIBRARY_NAME "project_library_${ANDROID_ABI}" )
    set( OUTPUT_FILE "${CMAKE_CURRENT_BINARY_DIR}/${LIBRARY_NAME}/lib/lib${LIBRARY_NAME}.so" )

    ExternalProject_Add(
        "${LIBRARY_NAME}"
        PREFIX "${LIBRARY_NAME}"
        SOURCE_DIR "${CURRENT_FUNCTION_DIR}"
        BUILD_ALWAYS TRUE
        BUILD_BYPRODUCTS "${OUTPUT_FILE}"
        CMAKE_ARGS
            -DANDROID_PLATFORM=android-30
            -DANDROID_ABI=${ANDROID_ABI}
            -DCMAKE_TOOLCHAIN_FILE=/home/tom/libraries/android_sdk/ndk/22.1.7171670/build/cmake/android.toolchain.cmake
            -DPROJECT_NAME=${LIBRARY_NAME}
            -DPROJECT_CMAKE_FILE=${PROJECT_CMAKE_FILE}
            -DCMAKE_INSTALL_PREFIX=${CMAKE_CURRENT_BINARY_DIR}/${LIBRARY_NAME}
    )

    add_custom_target(
       ${LIBRARY_NAME}_copy
       DEPENDS ${LIBRARY_NAME}
       COMMAND ${CMAKE_COMMAND}
           -E copy
           "${OUTPUT_FILE}"
           "${OUTPUT_LIB_DIR}/${ANDROID_ABI}/libproject_library.so"
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

    configure_file(
        "${CURRENT_FUNCTION_DIR}/templates/AppActivity.java.in"
        "${CMAKE_CURRENT_BINARY_DIR}/android/tmp/AppActivity.java"
        @ONLY
    )

    configure_file(
        "${CURRENT_FUNCTION_DIR}/templates/AndroidManifest.xml.in"
        "${CMAKE_CURRENT_BINARY_DIR}/android/tmp/AndroidManifest.xml"
        @ONLY
    )

    set( ANDROID_SDK "/home/tom/libraries/android_sdk" )
    set( ANDROID_JAR "${ANDROID_SDK}/platforms/android-30/android.jar" )
    add_custom_target(
        android_java_sources
        COMMAND javac
            -classpath "${ANDROID_JAR}"
            -source 1.8
            -target 1.8
            -d "${CMAKE_CURRENT_BINARY_DIR}/android/java_sources"
            "${CURRENT_FUNCTION_DIR}/src/MainActivity.java"
            "${CURRENT_FUNCTION_DIR}/src/MainView.java"
            "${CURRENT_FUNCTION_DIR}/src/ApplicationLib.java"
            "${CMAKE_CURRENT_BINARY_DIR}/android/tmp/AppActivity.java"
        SOURCES
            "${CURRENT_FUNCTION_DIR}/src/MainActivity.java"
            "${CURRENT_FUNCTION_DIR}/src/MainView.java"
            "${CURRENT_FUNCTION_DIR}/src/ApplicationLib.java"
            "${CMAKE_CURRENT_BINARY_DIR}/android/tmp/AppActivity.java"
    )

    set( ANDROID_DX "${ANDROID_SDK}/build-tools/30.0.3/dx" )
    add_custom_target(
        android_dex
        DEPENDS android_java_sources
        COMMAND ${ANDROID_DX}
            --dex
            --output="${CMAKE_CURRENT_BINARY_DIR}/android/classes.dex"
            "${CMAKE_CURRENT_BINARY_DIR}/android/java_sources"
    )

    ttb_create_android_target( "arm64-v8a" "${PROJECT_CMAKE_FILE}" "${CMAKE_CURRENT_BINARY_DIR}/android/lib" )

    set( ANDROID_AAPT2 "${ANDROID_SDK}/build-tools/30.0.3/aapt2" )
    set( ANDROID_ZIPALIGN "${ANDROID_SDK}/build-tools/30.0.3/zipalign" )
    add_custom_target(
        android_apk_link
        DEPENDS
            android_dex
            project_library_arm64-v8a_copy
        COMMAND ${ANDROID_AAPT2}
            link
            -I "${ANDROID_JAR}"
            --manifest "${CMAKE_CURRENT_BINARY_DIR}/android/tmp/AndroidManifest.xml"
            -v
            -o "${CMAKE_CURRENT_BINARY_DIR}/android/app_tmp.apk"
        COMMAND zip
            -uj "${CMAKE_CURRENT_BINARY_DIR}/android/app_tmp.apk"
            "${CMAKE_CURRENT_BINARY_DIR}/android/classes.dex"
        COMMAND cd ${CMAKE_CURRENT_BINARY_DIR}/android && zip
            -u "${CMAKE_CURRENT_BINARY_DIR}/android/app_tmp.apk"
            -r "lib"
        COMMAND ${ANDROID_ZIPALIGN}
            -f 4 "${CMAKE_CURRENT_BINARY_DIR}/android/app_tmp.apk"
            "${CMAKE_CURRENT_BINARY_DIR}/android/app_unsigned.apk"
    )

    set( JAVA_SIGNER "jarsigner" )
    add_custom_target(
        android_apk_sign ALL
        DEPENDS android_apk_link
        COMMAND ${JAVA_SIGNER}
            -keystore "/home/tom/.android/debug.keystore"
            -storepass "android"
            -keypass "android"
            -signedjar "${CMAKE_CURRENT_BINARY_DIR}/app_signed.apk"
            "${CMAKE_CURRENT_BINARY_DIR}/android/app_unsigned.apk"
            "androiddebugkey"
    )

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