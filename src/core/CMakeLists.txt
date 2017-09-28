set( LINK_LIBRARIES )

include( glew )
if( NOT TARGET glew )
    message( "Missing library: glew" )
    return()
endif()

include( opengl )
if( NOT TARGET opengl )
    message( "Missing library: opengl" )
    return()
endif()

if( ${COMPILATION_MODE} STREQUAL "GCC" )

    include( glfw )
    if( NOT TARGET glfw )
        message( "Missing library: glfw" )
        return()
    endif()

    set( LINK_LIBRARIES glew glfw opengl )

    set(
        SOURCE_FILES

        src/window/Window.cpp
        src/impl/linux/window/Window.cpp
        )

elseif( ${COMPILATION_MODE} STREQUAL "CLANG" )

    include( sdl )
    if( NOT TARGET sdl )
        message( "Missing library: sdl" )
        return()
    endif()

    set( LINK_LIBRARIES glew sdl opengl )

    set(
        SOURCE_FILES

        src/RenderTarget.cpp
        src/State.cpp
        src/geometry/Geometry.cpp
        src/geometry/IndexBuffer.cpp
        src/geometry/VertexBuffer.cpp
        src/impl/emscripten/window/Window.cpp
        src/shader/Program.cpp
        src/shader/Shader.cpp
        src/window/Window.cpp
        )

endif()

add_library(
    ttbCore
    STATIC
    ${SOURCE_FILES}
    )

target_include_directories(
    ttbCore
    PUBLIC
        include
    PRIVATE
        src
    )

target_compile_options(
    ttbCore
    PRIVATE
        -std=gnu++14
        -fPIC
    )

if( ${COMPILATION_MODE} STREQUAL "CLANG" )

    target_compile_options(
        ttbCore
        PRIVATE
            -s USE_SDL=2
        )

    target_link_libraries(
        ttbCore
        PUBLIC
        "-s USE_SDL=2"
        "-s DISABLE_EXCEPTION_CATCHING=0"
        )

endif()

target_compile_definitions(
    ttbCore
    PRIVATE
        GLEW_STATIC
        GL_EXT_PROTOTYPES
    )

target_link_libraries(
    ttbCore
    PUBLIC
        ttbMath
        ttbUtils
        ${LINK_LIBRARIES}
    )

list( APPEND VALID_TARGETS ttbCore )
set( VALID_TARGETS ${VALID_TARGETS} PARENT_SCOPE )
