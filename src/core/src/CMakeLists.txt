target_sources(
    ${TARGET_NAME}

    PRIVATE
        Logger.cpp
        State.cpp
        Context.cpp
        GBuffer.cpp
        PNGLoader.cpp
        RenderTarget.cpp
        Viewport.cpp
        gl.cpp
)

add_subdirectory( uniform )
add_subdirectory( geometry )
add_subdirectory( font )
add_subdirectory( shader )
add_subdirectory( texture )
add_subdirectory( window )
add_subdirectory( resources )
