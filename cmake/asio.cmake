if( TARGET asio )
    return()
endif()

find_path( ASIO_INCLUDE_DIR asio.hpp REQUIRED )

add_library( asio INTERFACE IMPORTED )
set_property( TARGET asio PROPERTY INTERFACE_INCLUDE_DIRECTORIES ${ASIO_INCLUDE_DIR} )
