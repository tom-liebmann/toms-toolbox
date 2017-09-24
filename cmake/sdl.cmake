if( TARGET sdl )
    return()
endif()

if( ${COMPILATION_MODE} STREQUAL "CLANG" )

    add_library( sdl INTERFACE IMPORTED )

else()

    find_package( SDL )

    if( NOT ${SDL_FOUND} )
        return()
    endif()

    add_library( sdl INTERFACE IMPORTED )
    set_property( TARGET sdl PROPERTY INTERFACE_INCLUDE_DIRECTORIES ${SDL_INCLUDE_DIR} )
    set_property( TARGET sdl PROPERTY INTERFACE_LINK_LIBRARIES ${SDL_LIBRARY} )

endif()
