include_guard( GLOBAL )

set( TTB_ROOT_DIR "${CMAKE_CURRENT_LIST_DIR}/../.." )
set( TTB_ANDROID_RES_DIR "${TTB_ROOT_DIR}/project/android" )

macro( _ttb_android_request_sdk )

    set( ANDROID_SDK_DIR "" CACHE PATH "Path to the android sdk root" )

    if( NOT ANDROID_SDK_DIR )
        message( FATAL_ERROR "ANDROID_SDK_DIR missing" )
    endif()

endmacro()

macro( _ttb_android_request_sdk_version )

    _ttb_android_request_sdk()

    file( GLOB ANDROID_PLATFORM_VERSIONS RELATIVE "${ANDROID_SDK_DIR}/platforms" "${ANDROID_SDK_DIR}/platforms/*" )

    if( NOT ANDROID_PLATFORM_VERSIONS )
        message( FATAL_ERROR "No installed SDK versions found")
    endif()

    set( ANDROID_SDK_VERSIONS "" )
    foreach( ANDROID_PLATFORM_VERSION ${ANDROID_PLATFORM_VERSIONS} )
        string( REGEX REPLACE "^android-([0-9]+)$" "\\1" ANDROID_SDK_VERSION ${ANDROID_PLATFORM_VERSION} )
        list( APPEND ANDROID_SDK_VERSIONS ${ANDROID_SDK_VERSION} )
    endforeach()

    list( SORT ANDROID_SDK_VERSIONS COMPARE NATURAL )
    list( GET ANDROID_SDK_VERSIONS -1 ANDROID_SDK_VERSION_DEFAULT )

    set( ANDROID_SDK_VERSION_MIN "${ANDROID_SDK_VERSION_DEFAULT}" CACHE STRING "Android minimum SDK version" )

    set( ANDROID_SDK_VERSION_TARGET "${ANDROID_SDK_VERSION_DEFAULT}" CACHE STRING "Android target SDK version" )
    set_property( CACHE ANDROID_SDK_VERSION_TARGET PROPERTY STRINGS ${ANDROID_SDK_VERSIONS} )

endmacro()

macro( _ttb_android_request_ndk_version )

    _ttb_android_request_sdk()

    file( GLOB ANDROID_NDK_VERSIONS RELATIVE "${ANDROID_SDK_DIR}/ndk" "${ANDROID_SDK_DIR}/ndk/*" )

    list( SORT ANDROID_NDK_VERSIONS COMPARE NATURAL )
    list( GET ANDROID_NDK_VERSIONS -1 ANDROID_NDK_VERSION_DEFAULT )

    set( ANDROID_NDK_VERSION "${ANDROID_NDK_VERSION_DEFAULT}" CACHE STRING "Android NDK version" )

    set_property( CACHE ANDROID_NDK_VERSION PROPERTY STRINGS ${ANDROID_NDK_VERSIONS} )

endmacro()

macro( _ttb_android_request_build_tools_version )

    _ttb_android_request_sdk()

    file( GLOB ANDROID_TOOLS_VERSIONS RELATIVE "${ANDROID_SDK_DIR}/build-tools" "${ANDROID_SDK_DIR}/build-tools/*" )
    list( SORT ANDROID_TOOLS_VERSIONS COMPARE NATURAL )
    list( GET ANDROID_TOOLS_VERSIONS -1 ANDROID_TOOLS_VERSION_DEFAULT )

    set( ANDROID_TOOLS_VERSION "${ANDROID_TOOLS_VERSION_DEFAULT}" CACHE STRING "Android build tools version" )

    set_property( CACHE ANDROID_TOOLS_VERSION PROPERTY STRINGS ${ANDROID_TOOLS_VERSIONS} )

endmacro()

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
            -DCMAKE_TOOLCHAIN_FILE=${ANDROID_SDK_DIR}/ndk/${ANDROID_NDK_VERSION}/build/cmake/android.toolchain.cmake
            -DBUILD_PLATFORM=Android
            -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
            -DPROJECT_NAME=${LIBRARY_NAME}
            -DPROJECT_CMAKE_FILE=${PROJECT_CMAKE_FILE}
            -DCMAKE_INSTALL_PREFIX=${CMAKE_CURRENT_BINARY_DIR}/${LIBRARY_NAME}
            -DCMAKE_MODULE_PATH=${CMAKE_CURRENT_BINARY_DIR}
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

macro( _ttb_add_project_impl PROJECT_NAME PROJECT_CMAKE_FILE )

    _ttb_android_request_sdk()
    _ttb_android_request_sdk_version()
    _ttb_android_request_ndk_version()
    _ttb_android_request_build_tools_version()

    set( ANDROID_PACKAGE_NAME "" CACHE STRING "Name of the android package" )

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

    set( ANDROID_PLATFORM_DIR "${ANDROID_SDK_DIR}/platforms/android-${ANDROID_SDK_VERSION_TARGET}" )
    set( ANDROID_TOOLS_DIR "${ANDROID_SDK_DIR}/build-tools/${ANDROID_TOOLS_VERSION}")

    set( ANDROID_JAR "${ANDROID_PLATFORM_DIR}/android.jar" )
    set( ANDROID_DX "${ANDROID_TOOLS_DIR}/dx" )
    set( ANDROID_AAPT2 "${ANDROID_TOOLS_DIR}/aapt2" )
    set( ANDROID_ZIPALIGN "${ANDROID_TOOLS_DIR}/zipalign" )
    set( ANDROID_APK_SIGNER "${ANDROID_TOOLS_DIR}/apksigner" )
    set( JAVA_COMPILER "javac"  )

    file( MAKE_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/android/java_sources")

    add_custom_target(
        android_java_sources
        COMMAND ${JAVA_COMPILER}
            -classpath "${ANDROID_JAR}"
            -Xlint:deprecation
            -source 1.8
            -target 1.8
            -d "${CMAKE_CURRENT_BINARY_DIR}/android/java_sources"
            "${TTB_ANDROID_RES_DIR}/java_src/MainActivity.java"
            "${TTB_ANDROID_RES_DIR}/java_src/MainView.java"
            "${TTB_ANDROID_RES_DIR}/java_src/ApplicationLib.java"
            "${TTB_ANDROID_RES_DIR}/java_src/ConnectionManager.java"
            "${CMAKE_CURRENT_BINARY_DIR}/android/tmp/AppActivity.java"
        SOURCES
            "${TTB_ANDROID_RES_DIR}/java_src/MainActivity.java"
            "${TTB_ANDROID_RES_DIR}/java_src/MainView.java"
            "${TTB_ANDROID_RES_DIR}/java_src/ConnectionManager.java"
            "${TTB_ANDROID_RES_DIR}/java_src/ApplicationLib.java"
            "${CMAKE_CURRENT_BINARY_DIR}/android/tmp/AppActivity.java"
    )

    add_custom_target(
        android_dex
        DEPENDS android_java_sources
        COMMAND ${ANDROID_DX}
            --dex
            --output="${CMAKE_CURRENT_BINARY_DIR}/android/classes.dex"
            "${CMAKE_CURRENT_BINARY_DIR}/android/java_sources"
    )

    _ttb_create_android_target( "arm64-v8a" "${PROJECT_CMAKE_FILE}" "${CMAKE_CURRENT_BINARY_DIR}/android/lib" )

    add_custom_target(
        ttb_project_initialize_asset_args
        COMMAND
            rm -f "${CMAKE_CURRENT_BINARY_DIR}/android/asset_args"
        COMMAND
            touch "${CMAKE_CURRENT_BINARY_DIR}/android/asset_args"
    )

# TODO:
# - implement resource directory
# - fix behavior when no asset dir is given

    add_custom_target(
        android_apk_link
        DEPENDS
            ttb_project_initialize_asset_args
            android_dex
            project_library_arm64-v8a_copy
        #COMMAND ${ANDROID_AAPT2}
        #    compile --dir "/home/tom/development/nonogram_solver/app/res"
        #    -o "${CMAKE_CURRENT_BINARY_DIR}/android/app_res.zip"
        COMMAND
            xargs
                -0
                -a "${CMAKE_CURRENT_BINARY_DIR}/android/asset_args"
                -I "ASSET_ARGS"
            sh -c '
            ${ANDROID_AAPT2}
                link
                # "${CMAKE_CURRENT_BINARY_DIR}/android/app_res.zip"
                -I "${ANDROID_JAR}"
                --manifest "${CMAKE_CURRENT_BINARY_DIR}/android/tmp/AndroidManifest.xml"
                ASSET_ARGS
                -v
                -o "${CMAKE_CURRENT_BINARY_DIR}/android/app_tmp.apk"
            '
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

    add_custom_target(
        android_apk_sign ALL
        DEPENDS android_apk_link
        COMMAND ${ANDROID_APK_SIGNER}
            sign
            --ks "${TTB_ANDROID_RES_DIR}/debug.keystore"
            --ks-pass pass:android
            --min-sdk-version ${ANDROID_SDK_VERSION_MIN}
            --out "${CMAKE_CURRENT_BINARY_DIR}/app_signed.apk"
            "${CMAKE_CURRENT_BINARY_DIR}/android/app_unsigned.apk"
    )

endmacro()

set( TTB_PROJECT_ASSET_TARGET_NR "0" )

macro( _ttb_project_assets_impl ASSET_DIR )

    set( TTB_PROJECT_ASSET_TARGET "ttb_project_asset_target_${TTB_PROJECT_ASSET_TARGET_NR}")
    math( EXPR TTB_PROJECT_ASSET_TARGET_NR "${TTB_PROJECT_ASSET_TARGET_NR}+1" )

    add_custom_target(
        ${TTB_PROJECT_ASSET_TARGET}
        DEPENDS
        ttb_project_initialize_asset_args
        COMMAND
        echo -n "-A ${ASSET_DIR} " >> "${CMAKE_CURRENT_BINARY_DIR}/android/asset_args"
        )

    add_dependencies( android_apk_link ${TTB_PROJECT_ASSET_TARGET} )

endmacro()
