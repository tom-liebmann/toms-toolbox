target_sources(
    ${TARGET_NAME}

    PRIVATE
        Window.cpp
        WindowCreator.cpp
        WindowImpl.cpp
)

include( glfw )

target_link_libraries(
    ${TARGET_NAME}

    PUBLIC
        glfw
)
