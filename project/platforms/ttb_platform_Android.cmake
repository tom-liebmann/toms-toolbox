set( TTB_ROOT_DIR "${CMAKE_CURRENT_LIST_DIR}/../.." )
set( TTB_ANDROID_RES_DIR "${TTB_ROOT_DIR}/project/android" )

set( ANDROID_NDK_VERSION "22.1.7171670" )

function( _ttb_create_android_target ANDROID_ABI PROJECT_CMAKE_FILE OUTPUT_LIB_DIR )

    include( ExternalProject )

    set( LIBRARY_NAME "project_library_${ANDROID_ABI}" )
    set( OUTPUT_FILE "${CMAKE_CURRENT_BINARY_DIR}/${LIBRARY_NAME}/lib/lib${LIBRARY_NAME}.so" )

    get_cmake_property( _CACHE_VARIABLES CACHE_VARIABLES )
    foreach( _CACHE_VARIABLE ${_CACHE_VARIABLES} )
      get_property( _CURRENT_HELP_STRING CACHE "${_CACHE_VARIABLE}" PROPERTY HELPSTRING )
        if("${_CURRENT_HELP_STRING}" MATCHES "No help, variable specified on the command line." OR "${_CURRENT_HELP_STRING}" STREQUAL "")
            list(APPEND _USER_ARGS "-D${_CACHE_VARIABLE}=${${_CACHE_VARIABLE}}")
        endif()
    endforeach()

    ExternalProject_Add(
        "${LIBRARY_NAME}"
        PREFIX "${LIBRARY_NAME}"
        SOURCE_DIR "${TTB_ANDROID_RES_DIR}"
        BUILD_ALWAYS TRUE
        BUILD_BYPRODUCTS "${OUTPUT_FILE}"
        CMAKE_ARGS
            -DANDROID_PLATFORM=android-${ANDROID_SDK_VERSION_TARGET}
            -DANDROID_ABI=${ANDROID_ABI}
            -DCMAKE_TOOLCHAIN_FILE=${ANDROID_SDK}/ndk/${ANDROID_NDK_VERSION}/build/cmake/android.toolchain.cmake
            -DBUILD_PLATFORM=Android
            -DPROJECT_NAME=${LIBRARY_NAME}
            -DPROJECT_CMAKE_FILE=${PROJECT_CMAKE_FILE}
            -DCMAKE_INSTALL_PREFIX=${CMAKE_CURRENT_BINARY_DIR}/${LIBRARY_NAME}
            -Dcmake-modules_DIR=${cmake-modules_DIR}
            ${_USER_ARGS}
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

macro( _ttb_init_project_impl PROJECT_CMAKE_FILE )

    set( ANDROID_PACKAGE_NAME "" CACHE STRING "Name of the android package" )
    set( ANDROID_SDK_VERSION_MIN "" CACHE STRING "Lowest supported SDK version" )
    set( ANDROID_SDK_VERSION_TARGET "" CACHE STRING "SDK version target" )

    if( NOT ANDROID_PACKAGE_NAME )
        message( FATAL_ERROR "ANDROID_PACKAGE_NAME not set" )
    endif()

    configure_file(
        "${TTB_ANDROID_RES_DIR}/templates/AppActivity.java.in"
        "${CMAKE_CURRENT_BINARY_DIR}/android/tmp/AppActivity.java"
        @ONLY
    )

    configure_file(
        "${TTB_ANDROID_RES_DIR}/templates/AndroidManifest.xml.in"
        "${CMAKE_CURRENT_BINARY_DIR}/android/tmp/AndroidManifest.xml"
        @ONLY
    )

    set( ANDROID_SDK "/home/tom/libraries/android_sdk" )
    set( ANDROID_JAR "${ANDROID_SDK}/platforms/android-${ANDROID_SDK_VERSION_TARGET}/android.jar" )
    add_custom_target(
        android_java_sources
        COMMAND javac
            -classpath "${ANDROID_JAR}"
            -Xlint:deprecation
            -source 1.8
            -target 1.8
            -d "${CMAKE_CURRENT_BINARY_DIR}/android/java_sources"
            "${TTB_ANDROID_RES_DIR}/java_src/MainActivity.java"
            "${TTB_ANDROID_RES_DIR}/java_src/MainView.java"
            "${TTB_ANDROID_RES_DIR}/java_src/ApplicationLib.java"
            "${CMAKE_CURRENT_BINARY_DIR}/android/tmp/AppActivity.java"
        SOURCES
            "${TTB_ANDROID_RES_DIR}/java_src/MainActivity.java"
            "${TTB_ANDROID_RES_DIR}/java_src/MainView.java"
            "${TTB_ANDROID_RES_DIR}/java_src/ApplicationLib.java"
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

    _ttb_create_android_target( "arm64-v8a" "${PROJECT_CMAKE_FILE}" "${CMAKE_CURRENT_BINARY_DIR}/android/lib" )
    # _ttb_create_android_target( "armeabi-v7a" "${PROJECT_CMAKE_FILE}" "${CMAKE_CURRENT_BINARY_DIR}/android/lib" )

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
