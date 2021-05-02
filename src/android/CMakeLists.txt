set( ANDROID_SDK "" CACHE PATH "Path to android sdk" )
set( ANDROID_REV "" CACHE STRING "Android version" )

if( not "${ANDROID_SDK}" )
    message( FATAL_ERROR "ANDROID_SDK not defined" )
endif()

set( ANDROID_TOOLS "${ANDROID_SDK}/platforms/${ANDROID_REV}/tools" )

set( ANDROID_AAPT_PACK "${ANDROID_SDK}/build-tools/${ANDROID_REV}/aapt package -v -f -I ${ANDROID_SDK}/platforms/${ANDROID_REV}/android.jar" )

add_custom_command(
    OUTPUT resources.ap_
    COMMAND "${ANDROID_AAPT_PACK}"
            "-M \"${CMAKE_CURRENT_LIST_DIR}/AndroidManifest.xml\""
            "-A \"${CMAKE_CURRENT_LIST_DIR}/assets\""
            "-S \"${CMAKE_CURRENT_LIST_DIR}/res\""
            "-m"
            "-J \"${CMAKE_CURRENT_LIST_DIR}/gen\""
            "-F \"${CMAKE_CURRENT_BINARY_DIR}/resources.ap_\""
)
