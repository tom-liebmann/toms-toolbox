if( TARGET boost )
    return()
endif()

find_package( Boost REQUIRED system thread )

if( NOT ${Boost_FOUND} )
    return()
endif()

add_library( boost INTERFACE IMPORTED )
set_property( TARGET boost PROPERTY INTERFACE_INCLUDE_DIRECTORIES ${Boost_INCLUDE_DIRS} )
set_property( TARGET boost PROPERTY INTERFACE_LINK_LIBRARIES ${Boost_LIBRARIES} )
