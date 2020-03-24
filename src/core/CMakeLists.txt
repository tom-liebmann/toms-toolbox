add_library(
    ttbCore
    SHARED
    src/Context.cpp
    src/window/Window.cpp
    src/State.cpp
    src/geometry/Geometry.cpp
    src/geometry/IndexBuffer.cpp
    src/geometry/VertexBuffer.cpp
    src/shader/Program.cpp
    src/shader/Shader.cpp
    src/RenderTarget.cpp
    src/texture/Texture2D.cpp
    src/texture/Texture.cpp
    src/texture/TextureFactory.cpp
    src/uniform/Uniform.cpp
    src/uniform/uniformHelper.cpp
    src/GBuffer.cpp
    src/PNGLoader.cpp
    src/font/Font.cpp
    src/font/TextFactory.cpp
)

target_compile_definitions(
    ttbCore
    PRIVATE
        MODE_${COMPILATION_MODE}
)


if( "${COMPILATION_MODE}" STREQUAL "GCC" )

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

    include( glfw )
    if( NOT TARGET glfw )
        message( "Missing library: glfw" )
        return()
    endif()

    target_link_libraries(
        ttbCore
        PRIVATE
        glew
        glfw
        opengl
    )

    target_compile_definitions(
        ttbCore
        PRIVATE
            GLEW_STATIC
            GL_EXT_PROTOTYPES
    )

    target_sources(
        ttbCore
        src/texture/Texture3D.cpp
        src/impl/linux/window/Window.cpp
    )

elseif( "${COMPILATION_MODE}" STREQUAL "ANDROID" )

    target_link_libraries(
        ttbCore
        PRIVATE
        android
        log
        EGL
        GLESv3
        )

endif()

target_include_directories(
    ttbCore
    PUBLIC
        include
    PRIVATE
        src
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
        "-s DEMANGLE_SUPPORT=1"
        "-s ASSERTIONS=2"
        )

endif()

target_link_libraries(
    ttbCore
    PUBLIC
        ttbMath
        ttbUtils
    )

target_compile_options(
    ttbCore
    PUBLIC
        -Wall
        -Wextra
        -Wpedantic
    )

set_target_properties(
    ttbCore
    PROPERTIES
        POSITION_INDEPENDENT_CODE ON
        CXX_STANDARD 17
    )

target_compile_features(
    ttbCore
    PUBLIC
        cxx_std_17
    )

list( APPEND VALID_TARGETS ttbCore )
set( VALID_TARGETS ${VALID_TARGETS} PARENT_SCOPE )
