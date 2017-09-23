include( glew )
if( NOT TARGET glew )
    message( "Missing library: glew" )
    return()
endif()

include( glfw )
if( NOT TARGET glfw )
    message( "Missing library: glfw" )
    return()
endif()

include( opengl )
if( NOT TARGET opengl )
    message( "Missing library: opengl" )
    return()
endif()

# find source files
file(
    GLOB_RECURSE
    SOURCE_FILES
    ABSOLUTE "${CMAKE_CURRENT_SOURCE_DIR}"
    src/*.cpp
    )

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
        glew
        glfw
        opengl
    )

list( APPEND VALID_TARGETS ttbCore )
set( VALID_TARGETS ${VALID_TARGETS} PARENT_SCOPE )
