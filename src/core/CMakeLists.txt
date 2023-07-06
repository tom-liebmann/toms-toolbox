set( TARGET_NAME ttbCore )

ttb_add_module( ${TARGET_NAME} ttb::core )

if( NOT TARGET ${TARGET_NAME} )
    return()
endif()

target_include_directories( ${TARGET_NAME} PUBLIC include PRIVATE src )
target_include_directories( ${TARGET_NAME} PRIVATE ${CMAKE_INCLUDE_PATH} )

target_link_libraries( ${TARGET_NAME} PUBLIC ttb::math ttb::utils )

ttb_add_module_dependency( ${TARGET_NAME} yaml-cpp yaml-cpp )

add_subdirectory( src )

if( "${BUILD_PLATFORM}" STREQUAL "Linux" )

    ttb_add_module_dependency( ${TARGET_NAME} GLEW GLEW::GLEW )
    ttb_add_module_dependency( ${TARGET_NAME} opengl_system opengl::opengl )

    target_compile_definitions(
        ${TARGET_NAME}

        PUBLIC
            GLEW_STATIC
            GL_EXT_PROTOTYPES
    )

elseif( "${BUILD_PLATFORM}" STREQUAL "Android" )

    target_link_libraries(
        ${TARGET_NAME}

        PUBLIC
            android
            log
            EGL
            GLESv3
    )

endif()

set( EXPORT_TARGETS ${EXPORT_TARGETS} PARENT_SCOPE )
set( EXPORT_DEPENDENCIES ${EXPORT_DEPENDENCIES} PARENT_SCOPE )
