set( TARGET_NAME ttbCore )

set( ACTIVATE_${TARGET_NAME} ON CACHE BOOL "Enable module ${TARGET_NAME}" )

if( NOT ACTIVATE_${TARGET_NAME} )
    return()
endif()

add_library( ${TARGET_NAME} OBJECT EXCLUDE_FROM_ALL )

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

    PRIVATE
        -Wall
        -Wextra
        -Wpedantic
)

set_target_properties(
    ${TARGET_NAME}

    PROPERTIES
        POSITION_INDEPENDENT_CODE ON
)

target_compile_features(
    ${TARGET_NAME}

    PUBLIC
        cxx_std_17
)

add_subdirectory( src )

if( "${BUILD_PLATFORM}" STREQUAL "Linux" )

    include( glew )
    include( opengl )

    target_link_libraries(
        ${TARGET_NAME}

        PRIVATE
            glew
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

if( "${LIBRARY_TYPE}" STREQUAL "OBJECT" )
    target_sources(
        ttb

        PUBLIC
            $<TARGET_OBJECTS:${TARGET_NAME}>
    )
else()
    target_link_libraries(
        ttb

        PUBLIC
            ${TARGET_NAME}
    )

    list( APPEND EXPORT_TARGETS ${TARGET_NAME} )
    set( EXPORT_TARGETS ${EXPORT_TARGETS} PARENT_SCOPE )
endif()
