if( TARGET boost )
    return()
endif()

find_package( Boost REQUIRED system thread )

if( NOT ${Boost_FOUND} )
    return()
endif()

add_library( boost INTERFACE IMPORTED )
target_include_directories( boost INTERFACE ${Boost_INCLUDE_DIRS} )
target_link_libraries( boost INTERFACE ${Boost_LIBRARIES} )
