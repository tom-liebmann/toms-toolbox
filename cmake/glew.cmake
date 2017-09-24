if( TARGET glew )
    return()
endif()

if( ${COMPILATION_MODE} STREQUAL "CLANG" )

    add_library( glew INTERFACE IMPORTED )

else()

    find_package( GLEW )

    if( NOT ${GLEW_FOUND} )
        return()
    endif()

    add_library( glew INTERFACE IMPORTED )
    set_property( TARGET glew PROPERTY INTERFACE_INCLUDE_DIRETORIES ${GLEW_INCLUDE_DIRS} )
    set_property( TARGET glew PROPERTY INTERFACE_LINK_LIBRARIES ${GLEW_LIBRARIES} )

endif()
