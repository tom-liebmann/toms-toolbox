target_sources(
    ${TARGET_NAME}

    PRIVATE
        Window.cpp
        OffscreenContext.cpp
        WindowImpl.cpp
)

ttb_add_module_dependency( ${TARGET_NAME} "glfw3" glfw::glfw )

set( EXPORT_DEPENDENCIES ${EXPORT_DEPENDENCIES} PARENT_SCOPE )
