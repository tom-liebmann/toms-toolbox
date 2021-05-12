if( TARGET glfw )
    return()
endif()

find_package( GLFW )

if( NOT ${GLFW_FOUND} )
    message( FATAL_ERROR "Did not find GLFW" )
endif()

add_library( glfw INTERFACE IMPORTED )
set_property( TARGET glfw PROPERTY INTERFACE_INCLUDE_DIRECTORIES ${GLFW_INCLUDE_DIR} )
set_property( TARGET glfw PROPERTY INTERFACE_LINK_LIBRARIES ${GLFW_LIBRARIES} )

