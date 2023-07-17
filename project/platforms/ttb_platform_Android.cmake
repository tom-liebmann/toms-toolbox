include_guard( GLOBAL )

include( GetPrerequisites )

set( TTB_ROOT_DIR "${CMAKE_CURRENT_LIST_DIR}/../.." )
set( TTB_ANDROID_RES_DIR "${TTB_ROOT_DIR}/project/android" )
set( CONAN_COMMAND "conan" )

ttb_define_map( ANDROID_ABI_ARCH
    "arm64-v8a"   "armv8"
    "armeabi-v7a" "armv7"
    "x86"         "x86"
    "x86_64"      "x86_64"
)

ttb_define_map( ANDROID_ABI_COMPILER_PREFIX
    "arm64-v8a"   "aarch64-linux-android"
    "armeabi-v7a" "armv7a-linux-androideabi"
    "x86"         "i686-linux-android"
    "x86_64"      "x86_64-linux-android"
)

function( _ttb_project_finish PROJECT_NAME )

    _ttb_project_check_required_property( ${PROJECT_NAME} TTB_ANDROID_ABI )
    _ttb_project_check_required_property( ${PROJECT_NAME} TTB_KEYSTORE_FILE )
    _ttb_project_check_required_property( ${PROJECT_NAME} TTB_KEYSTORE_PWD_FILE )
    _ttb_project_check_required_property( ${PROJECT_NAME} TTB_VERSION_NUMBER )
    _ttb_project_check_required_property( ${PROJECT_NAME} TTB_VERSION_NAME )

    _ttb_android_request_sdk()
    _ttb_android_request_sdk_version()
    _ttb_android_request_ndk_version()
    _ttb_android_request_build_tools_version()

    set( ANDROID_PACKAGE_NAME "" CACHE STRING "Name of the android package" )

    if( NOT ANDROID_PACKAGE_NAME )
        message( FATAL_ERROR "ANDROID_PACKAGE_NAME not set" )
    endif()

    set( ANDROID_PLATFORM_DIR "${ANDROID_SDK_DIR}/platforms/android-${ANDROID_SDK_VERSION_TARGET}" )
    set( ANDROID_TOOLS_DIR "${ANDROID_SDK_DIR}/build-tools/${ANDROID_TOOLS_VERSION}")

    set( ANDROID_JAR "${ANDROID_PLATFORM_DIR}/android.jar" )
    set( ANDROID_DX "${ANDROID_TOOLS_DIR}/dx" )
    set( ANDROID_AAPT2 "${ANDROID_TOOLS_DIR}/aapt2" )
    set( ANDROID_ZIPALIGN "${ANDROID_TOOLS_DIR}/zipalign" )
    set( ANDROID_APK_SIGNER "${ANDROID_TOOLS_DIR}/apksigner" )
    set( ANDROID_BUNDLETOOL "bundletool" )
    set( JAVA_COMPILER "javac"  )

    get_property( _PROJECT_CONAN_FILE TARGET ${PROJECT_NAME} PROPERTY TTB_CONAN_FILE )
    get_property( _PROJECT_CMAKE_FILE TARGET ${PROJECT_NAME} PROPERTY TTB_CMAKE_FILE )
    get_property( _PROJECT_ASSET_DIRS TARGET ${PROJECT_NAME} PROPERTY TTB_ASSET_DIRECTORIES )
    get_property( _PROJECT_ANDROID_ABI TARGET ${PROJECT_NAME} PROPERTY TTB_ANDROID_ABI )
    get_property( _PROJECT_KEYSTORE_FILE TARGET ${PROJECT_NAME} PROPERTY TTB_KEYSTORE_FILE )
    get_property( _PROJECT_KEYSTORE_PWD_FILE TARGET ${PROJECT_NAME} PROPERTY TTB_KEYSTORE_PWD_FILE )
    get_property( _PROJECT_VERSION_NR TARGET ${PROJECT_NAME} PROPERTY TTB_VERSION_NUMBER )
    get_property( _PROJECT_VERSION_NAME TARGET ${PROJECT_NAME} PROPERTY TTB_VERSION_NAME )

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

    _ttb_project_android_build_java_src(
        OUTPUT_TARGET ${PROJECT_NAME}_build_java_src
        OUTPUT_FILE "${CMAKE_CURRENT_BINARY_DIR}/outputs/dex/classes.dex"
        BUILD_DIR "${CMAKE_CURRENT_BINARY_DIR}/build_java_src"
        SOURCES
            "${TTB_ANDROID_RES_DIR}/java_src/MainActivity.java"
            "${TTB_ANDROID_RES_DIR}/java_src/MainView.java"
            "${TTB_ANDROID_RES_DIR}/java_src/ApplicationLib.java"
            "${TTB_ANDROID_RES_DIR}/java_src/ConnectionManager.java"
            "${CMAKE_CURRENT_BINARY_DIR}/android/tmp/AppActivity.java"
    )

    set( _ARCH_DEPENDENCIES )
    foreach( _ABI ${_PROJECT_ANDROID_ABI} )
        set( _ARCH ${ANDROID_ABI_ARCH_${_ABI}} )
        set( _COMPILER_PREFIX ${ANDROID_ABI_COMPILER_PREFIX_${_ABI}} )

        _ttb_project_android_build_arch(
            OUTPUT_TARGET ${PROJECT_NAME}_build_lib_${_ARCH}
            OUTPUT_DIR "${CMAKE_CURRENT_BINARY_DIR}/outputs/lib/${_ABI}"
            ARCH ${_ARCH}
            ABI ${_ABI}
            COMPILER_PREFIX ${_COMPILER_PREFIX}
            CONAN_FILE ${_PROJECT_CONAN_FILE}
            CMAKE_FILE ${_PROJECT_CMAKE_FILE}
            BUILD_DIR "${CMAKE_CURRENT_BINARY_DIR}/build_lib_${_ARCH}"
        )
        list( APPEND _ARCH_DEPENDENCIES ${PROJECT_NAME}_build_lib_${_ARCH} )
    endforeach()

    _ttb_project_android_build_bundle(
        OUTPUT_TARGET ${PROJECT_NAME}_build_bundle
        OUTPUT_FILE "${CMAKE_CURRENT_BINARY_DIR}/outputs/app_bundle.aab"
        MANIFEST "${CMAKE_CURRENT_BINARY_DIR}/android/tmp/AndroidManifest.xml"
        BUILD_DIR "${CMAKE_CURRENT_BINARY_DIR}/build_bundle"
        DEX_DIR "${CMAKE_CURRENT_BINARY_DIR}/outputs/dex"
        LIB_DIR "${CMAKE_CURRENT_BINARY_DIR}/outputs/lib"
        ASSET_DIRS "${_PROJECT_ASSET_DIRS}"
        KEYSTORE_FILE "${_PROJECT_KEYSTORE_FILE}"
        KEYSTORE_PWD_FILE "${_PROJECT_KEYSTORE_PWD_FILE}"
        DEPENDS
            ${PROJECT_NAME}_build_java_src
            ${_ARCH_DEPENDENCIES}
    )

    add_dependencies( ${PROJECT_NAME} ${PROJECT_NAME}_build_bundle )

endfunction()

#! Generates a .dex file that contains all compiled class files.
#
# \param:OUTPUT_TARGET Name of the target for generating dex file.
# \param:OUTPUT_FILE Name of generated dex file.
# \param:BUILD_DIR Directory used for storing build files.
# \group:SOURCES List of Java source files
#
function( _ttb_project_android_build_java_src )

    cmake_parse_arguments(
        _ARGS
        ""                         # Options
        "OUTPUT_TARGET;OUTPUT_FILE;BUILD_DIR"  # Single value keywords
        "SOURCES"                  # Multi value keywords
        ${ARGN}
    )

    set( _CLASS_DIR "${_ARGS_BUILD_DIR}/java_class" )
    set( _DEPENDENCY_JAR_DIR "${_ARGS_BUILD_DIR}/dependency_jars" )

    add_custom_command(
        OUTPUT ${_CLASS_DIR}
        COMMAND ${CMAKE_COMMAND} -E make_directory ${_CLASS_DIR}
    )

    add_custom_target(
        ${_ARGS_PREFIX}_dep_download
        COMMAND sh ${TTB_ANDROID_RES_DIR}/download_java_deps.sh
            ${TTB_ANDROID_RES_DIR}/pom.xml
            ${_DEPENDENCY_JAR_DIR}
    )

    file( GLOB _DEPENDENCY_JARS ${_DEPENDENCY_JAR_DIR}/*.jar )
    list( JOIN _DEPENDENCY_JARS ":" _DEPENDENCY_CLASSPATH )

    add_custom_target(
        ${_ARGS_PREFIX}_class
        COMMAND ${JAVA_COMPILER}
            -classpath "${ANDROID_JAR}:${_DEPENDENCY_CLASSPATH}"
            -Xlint:deprecation
            -source 1.8
            -target 1.8
            -d ${_CLASS_DIR}
            ${_ARGS_SOURCES}
        DEPENDS
            ${_ARGS_SOURCES}
            ${_CLASS_DIR}
        COMMAND_EXPAND_LISTS
    )

    get_filename_component( _OUTPUT_DIR "${_ARGS_OUTPUT_FILE}" DIRECTORY )

    add_custom_command(
        OUTPUT "${_OUTPUT_DIR}"
        COMMAND ${CMAKE_COMMAND} -E make_directory
            "${_OUTPUT_DIR}"
    )

    add_custom_command(
        OUTPUT ${_ARGS_OUTPUT_FILE}
        COMMAND ${ANDROID_DX}
            --dex
            --output="${_ARGS_OUTPUT_FILE}"
            ${_CLASS_DIR}
            ${_DEPENDENCY_JARS}
        DEPENDS
            ${_ARGS_PREFIX}_class
            "${_OUTPUT_DIR}"
    )

    add_custom_target( ${_ARGS_OUTPUT_TARGET} DEPENDS ${_ARGS_OUTPUT_FILE} )

endfunction()

function( _ttb_project_android_build_arch )

    set( _OPTIONS )
    set( _SINGLE_KEYWORDS
        "OUTPUT_TARGET"
        "OUTPUT_DIR"
        "ARCH"
        "ABI"
        "COMPILER_PREFIX"
        "CONAN_FILE"
        "CMAKE_FILE"
        "BUILD_DIR"
    )
    set( _MULTI_KEYWORDS )
    cmake_parse_arguments( _ARGS "${_OPTIONS}" "${_SINGLE_KEYWORDS}" "${_MULTI_KEYWORDS}" ${ARGN} )

    include( ExternalProject )

    set( ANDROID_TOOLCHAIN_FILE "${ANDROID_SDK_DIR}/ndk/${ANDROID_NDK_VERSION}/build/cmake/android.toolchain.cmake" )

    set( CONAN_PROFILE_FILE "${_ARGS_BUILD_DIR}/conan_profile" )
    configure_file(
        "${TTB_ANDROID_RES_DIR}/conan_profile.in"
        "${CONAN_PROFILE_FILE}"
        @ONLY
    )

    get_cmake_property( _CACHE_VARIABLES CACHE_VARIABLES )
    foreach( _CACHE_VARIABLE ${_CACHE_VARIABLES} )
        get_property( _CURRENT_HELP_STRING CACHE "${_CACHE_VARIABLE}" PROPERTY HELPSTRING )
        if("${_CURRENT_HELP_STRING}" MATCHES "No help, variable specified on the command line." OR "${_CURRENT_HELP_STRING}" STREQUAL "")
            list(APPEND _USER_ARGS "-D${_CACHE_VARIABLE}=${${_CACHE_VARIABLE}}")
        endif()
    endforeach()

    add_custom_command(
        OUTPUT ${_ARGS_BUILD_DIR}/conan/conan_toolchain.cmake
        COMMAND ${CONAN_COMMAND} install
            --build=missing
            --output-folder "${_ARGS_BUILD_DIR}/conan"
            --profile "${CONAN_PROFILE_FILE}"
            "${_ARGS_CONAN_FILE}"
        DEPENDS
            ${_ARGS_CONAN_FILE}
    )

    add_custom_target( ${_ARGS_OUTPUT_TARGET}_conan DEPENDS ${_ARGS_BUILD_DIR}/conan/conan_toolchain.cmake )

    ExternalProject_Add(
        ${_ARGS_OUTPUT_TARGET}_external
        DEPENDS ${_ARGS_OUTPUT_TARGET}_conan
        PREFIX "${_ARGS_BUILD_DIR}"
        SOURCE_DIR "${TTB_ANDROID_RES_DIR}"
        CMAKE_ARGS
            "${TTB_ANDROID_RES_DIR}"
            -DCMAKE_TOOLCHAIN_FILE=${_ARGS_BUILD_DIR}/conan/conan_toolchain.cmake
            -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
            -DCMAKE_INSTALL_PREFIX=${_ARGS_OUTPUT_DIR}
            -DANDROID_PLATFORM=android-${ANDROID_SDK_VERSION_TARGET}
            -DANDROID_ABI=${_ARGS_ABI}
            -DBUILD_PLATFORM=Android
            -DPROJECT_NAME=${_ARGS_OUTPUT_TARGET}_external
            -DPROJECT_CMAKE_FILE=${_ARGS_CMAKE_FILE}
            -DPROJECT_CONAN_DIR=${_ARGS_BUILD_DIR}/conan
            -DPROJECT_LIB_PATH_FILE=${_ARGS_BUILD_DIR}/lib_path.txt
            ${_USER_ARGS}
        BUILD_ALWAYS TRUE
    )

    add_custom_target(
        ${_ARGS_OUTPUT_TARGET}
        DEPENDS ${_ARGS_OUTPUT_TARGET}_external
        COMMAND sh ${TTB_ANDROID_RES_DIR}/copy_dep_libraries.sh
            ${_ARGS_OUTPUT_DIR}/libproject_library.so
            ${_ARGS_BUILD_DIR}/lib_path.txt
            ${_ARGS_OUTPUT_DIR}
    )

endfunction()

function( _ttb_project_android_build_bundle )

    set( _OPTIONS )
    set( _SINGLE_KEYWORDS
        "OUTPUT_TARGET"
        "OUTPUT_FILE"
        "BUILD_DIR"
        "LIB_DIR"
        "DEX_DIR"
        "MANIFEST"
        "KEYSTORE_FILE"
        "KEYSTORE_PWD_FILE"
    )
    set( _MULTI_KEYWORDS
        "DEPENDS"
        "ASSET_DIRS"
    )
    cmake_parse_arguments( _ARGS "${_OPTIONS}" "${_SINGLE_KEYWORDS}" "${_MULTI_KEYWORDS}" ${ARGN} )

    # TODO Implement resource directories
    # COMMAND ${ANDROID_AAPT2}
    #    compile --dir "/home/tom/development/nonogram_solver/app/res"
    #    -o "${CMAKE_CURRENT_BINARY_DIR}/android/app_res.zip"
    #
    # Add them to linking command

    set( _REQUIRED_DIRECTORIES
        "${_ARGS_BUILD_DIR}"
        "${_ARGS_BUILD_DIR}/bundle_files"
        "${_ARGS_BUILD_DIR}/bundle_files/manifest"
        "${_ARGS_BUILD_DIR}/bundle_files/dex"
        "${_ARGS_BUILD_DIR}/bundle_files/res"
        "${_ARGS_BUILD_DIR}/bundle_files/assets"
        "${_ARGS_BUILD_DIR}/bundle_files/lib"
        "${_ARGS_BUILD_DIR}/linked_files"
    )

    add_custom_command(
        OUTPUT ${_REQUIRED_DIRECTORIES}
        DEPENDS ${_ARGS_DEPENDS}
        COMMAND ${CMAKE_COMMAND} -E make_directory ${_REQUIRED_DIRECTORIES}
    )

    add_custom_command(
        OUTPUT
            "${_ARGS_BUILD_DIR}/linked_files/resources.pb"
            "${_ARGS_BUILD_DIR}/linked_files/AndroidManifest.xml"
        DEPENDS
            "${_REQUIRED_DIRECTORIES}"
            "${_ARGS_MANIFEST}"
        COMMAND ${ANDROID_AAPT2}
            link
            --proto-format
            -I "${ANDROID_JAR}"
            --manifest "${_ARGS_MANIFEST}"
            -o "${_ARGS_BUILD_DIR}/app_tmp.apk"
        COMMAND unzip
            -u
            "${_ARGS_BUILD_DIR}/app_tmp.apk"
            -d "${_ARGS_BUILD_DIR}/linked_files"
    )

    foreach( _ASSET_DIR ${_ARGS_ASSET_DIRS} )
        list( APPEND _ASSET_COMMANDS
            COMMAND ${CMAKE_COMMAND} -E copy_directory
            "${_ASSET_DIR}"
            "${_ARGS_BUILD_DIR}/bundle_files/assets"
        )
    endforeach()

    add_custom_command(
        OUTPUT "${_ARGS_BUILD_DIR}/bundle.zip"
        DEPENDS
            "${_REQUIRED_DIRECTORIES}"
            "${_ARGS_BUILD_DIR}/linked_files/resources.pb"
            "${_ARGS_BUILD_DIR}/linked_files/AndroidManifest.xml"
        WORKING_DIRECTORY "${_ARGS_BUILD_DIR}/bundle_files"
        ${_ASSET_COMMANDS}
        COMMAND ${CMAKE_COMMAND} -E copy
            "${_ARGS_BUILD_DIR}/linked_files/AndroidManifest.xml"
            "${_ARGS_BUILD_DIR}/bundle_files/manifest/"
        COMMAND ${CMAKE_COMMAND} -E copy
            "${_ARGS_BUILD_DIR}/linked_files/resources.pb"
            "${_ARGS_BUILD_DIR}/bundle_files/"
        COMMAND ${CMAKE_COMMAND} -E copy_directory
            "${_ARGS_DEX_DIR}"
            "${_ARGS_BUILD_DIR}/bundle_files/dex/"
        COMMAND ${CMAKE_COMMAND} -E copy_directory
            "${_ARGS_LIB_DIR}"
            "${_ARGS_BUILD_DIR}/bundle_files/lib/"
        COMMAND zip
            -r
            "${_ARGS_BUILD_DIR}/bundle.zip"
            "./*"
    )

    add_custom_command(
        OUTPUT "${_ARGS_BUILD_DIR}/bundle_unsigned.aab"
        DEPENDS
            "${_ARGS_BUILD_DIR}/bundle.zip"
        COMMAND ${ANDROID_BUNDLETOOL}
            build-bundle
            --modules="${_ARGS_BUILD_DIR}/bundle.zip"
            --output="${_ARGS_BUILD_DIR}/bundle_unsigned.aab"
            --overwrite
    )

    add_custom_command(
        OUTPUT "${_ARGS_OUTPUT_FILE}"
        DEPENDS "${_ARGS_BUILD_DIR}/bundle_unsigned.aab"
        COMMAND ${ANDROID_APK_SIGNER} sign
            --ks "${_ARGS_KEYSTORE_FILE}"
            --ks-pass file:${_ARGS_KEYSTORE_PWD_FILE}
            --min-sdk-version ${ANDROID_SDK_VERSION_MIN}
            --out "${_ARGS_OUTPUT_FILE}"
            "${_ARGS_BUILD_DIR}/bundle_unsigned.aab"
    )

    add_custom_target( ${_ARGS_OUTPUT_TARGET} DEPENDS "${_ARGS_OUTPUT_FILE}" )

endfunction()

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

macro( _ttb_add_project_impl PROJECT_NAME PROJECT_CONAN_FILE PROJECT_CMAKE_FILE )

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

    _ttb_create_android_target( "armv8" "arm64-v8a" "${PROJECT_CONAN_FILE}" "${PROJECT_CMAKE_FILE}" "${CMAKE_CURRENT_BINARY_DIR}/android/lib" )

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

    if( NOT TARGET android_apk_link )
        message( FATAL_ERROR "Missing target android_apk_link" )
    endif()

    add_dependencies( android_apk_link ${TTB_PROJECT_ASSET_TARGET} )

endmacro()
