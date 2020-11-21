set( TARGET_NAME ttbNet )

set( ACTIVATE_${TARGET_NAME} ON CACHE BOOL "Enable module ${TARGET_NAME}" )

if( NOT ACTIVATE_${TARGET_NAME} )
    return()
endif()

include( asio )

add_library( ${TARGET_NAME} OBJECT EXCLUDE_FROM_ALL )

target_include_directories(
    ${TARGET_NAME}

    PUBLIC
    include
    )

target_compile_options(
    ${TARGET_NAME}

    PRIVATE
    -Wall
    -Wextra
    -Wpedantic
)

target_link_libraries(
    ${TARGET_NAME}

    PUBLIC
        ttbUtils
        asio
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
