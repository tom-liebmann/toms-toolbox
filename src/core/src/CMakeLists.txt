target_sources(
    ${TARGET_NAME}

    PRIVATE
        Bindable.cpp
        Logger.cpp
        State.cpp
        Context.cpp
        GBuffer.cpp
        PNGLoader.cpp
        RenderTarget.cpp
        ViewportBinder.cpp
        gl.cpp
)

add_subdirectory( uniform )
add_subdirectory( geometry )
add_subdirectory( font )
add_subdirectory( shader )
add_subdirectory( texture )
add_subdirectory( window )
add_subdirectory( resources )

set( EXPORT_DEPENDENCIES ${EXPORT_DEPENDENCIES} PARENT_SCOPE )
