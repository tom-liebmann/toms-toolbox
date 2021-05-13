set( TARGET_NAME ttbCore )

ttb_add_module( ${TARGET_NAME} )

if( NOT TARGET ${TARGET_NAME} )
    return()
endif()

include( yamlcpp )

target_include_directories( ${TARGET_NAME} PUBLIC include PRIVATE src )

target_link_libraries( ${TARGET_NAME} PUBLIC ttbMath ttbUtils yamlcpp )

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

endif()

set( EXPORT_TARGETS ${EXPORT_TARGETS} PARENT_SCOPE )
