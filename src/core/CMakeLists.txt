find_package( GLEW REQUIRED )
find_package( GLFW REQUIRED )
find_package( OpenGL REQUIRED )

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
    ${SOURCE_FILES} )

target_include_directories(
    ttbCore
    PUBLIC
    ${GLEW_INCLUDE_DIR}
    ${GLFW_INCLUDE_DIR}
    include
    PRIVATE
    src )

target_compile_options(
    ttbCore
    PRIVATE
    -std=gnu++14
    -fPIC )

target_compile_definitions(
    ttbCore
    PRIVATE
    GLEW_STATIC
    GL_EXT_PROTOTYPES )

target_link_libraries(
    ttbCore
    PUBLIC
    ttbMath
    ${GLFW_LIBRARIES}
    ${GLEW_LIBRARY}
    ${OPENGL_LIBRARIES} )

if( WIN32 )

    target_compile_definitions(
        ttbCore
        PRIVATE
        WIN32 )

endif()
