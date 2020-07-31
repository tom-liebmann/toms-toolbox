if( TARGET glew )
    return()
endif()

find_package( GLEW )

if( NOT ${GLEW_FOUND} )
    message( FATAL_ERROR "Missing dependency: glew" )
endif()

add_library( glew INTERFACE IMPORTED )
set_property( TARGET glew PROPERTY INTERFACE_INCLUDE_DIRETORIES ${GLEW_INCLUDE_DIRS} )
set_property( TARGET glew PROPERTY INTERFACE_LINK_LIBRARIES ${GLEW_LIBRARIES} )
