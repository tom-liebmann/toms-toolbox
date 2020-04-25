set( TARGET_NAME ttbCore )

set( ACTIVATE_${TARGET_NAME} ON CACHE BOOL "Enable module ${TARGET_NAME}" )

if( NOT ACTIVATE_${TARGET_NAME} )
    return()
endif()

add_library( ${TARGET_NAME} SHARED )

string( TOUPPER ${BUILD_PLATFORM} BUILD_PLATFORM_UPPER )
target_compile_definitions(
    ${TARGET_NAME}

    PUBLIC
        PLATFORM_${BUILD_PLATFORM_UPPER}
)

target_include_directories(
    ${TARGET_NAME}

    PUBLIC
        include

    PRIVATE
        src
)

target_link_libraries(
    ${TARGET_NAME}

    PUBLIC
        ttbMath
        ttbUtils
)

target_compile_options(
    ${TARGET_NAME}

    PUBLIC
        -Wall
        -Wextra
        -Wpedantic
)

set_target_properties(
    ${TARGET_NAME}

    PROPERTIES
        POSITION_INDEPENDENT_CODE ON
        CXX_STANDARD 17
)

target_compile_features(
    ${TARGET_NAME}

    PUBLIC
        cxx_std_17
)

add_subdirectory( src )

if( "${BUILD_PLATFORM}" STREQUAL "Desktop" )

    include( glew )
    include( opengl )
    include( glfw )

    target_link_libraries(
        ${TARGET_NAME}

        PRIVATE
            glew
            glfw
            opengl
    )

    target_compile_definitions(
        ${TARGET_NAME}

        PRIVATE
            GLEW_STATIC
            GL_EXT_PROTOTYPES
    )

elseif( "${BUILD_PLATFORM}" STREQUAL "Android" )

    target_link_libraries(
        ${TARGET_NAME}

        PRIVATE
            android
            log
            EGL
            GLESv3
    )

elseif( "${BUILD_PLATFORM}" STREQUAL "Browser" )

    target_compile_options(
        ${TARGET_NAME}

        PRIVATE
            -s USE_SDL=2
    )

    target_link_libraries(
        ${TARGET_NAME}

        PUBLIC
            "-s USE_SDL=2"
            "-s DISABLE_EXCEPTION_CATCHING=0"
            "-s DEMANGLE_SUPPORT=1"
            "-s ASSERTIONS=2"
    )

endif()

list( APPEND VALID_TARGETS ${TARGET_NAME} )
set( VALID_TARGETS ${VALID_TARGETS} PARENT_SCOPE )
