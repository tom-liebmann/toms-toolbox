if( TARGET opengl )
    return()
endif()

set( OpenGL_GL_PREFERENCE "GLVND" )

find_package( OpenGL )

if( NOT ${OPENGL_FOUND} )
    return()
endif()

add_library( opengl INTERFACE IMPORTED )
set_property( TARGET opengl PROPERTY INTERFACE_INCLUDE_DIRETORIES ${OPENGL_INCLUDE_DIR} )
set_property( TARGET opengl PROPERTY INTERFACE_LINK_LIBRARIES ${OPENGL_LIBRARIES} )
