set( TARGET_NAME ttbUtils )

ttb_add_module( ${TARGET_NAME} )

if( NOT TARGET ${TARGET_NAME} )
    return()
endif()

find_package( fmt REQUIRED )

target_include_directories( ${TARGET_NAME} PUBLIC include )

target_link_libraries( ${TARGET_NAME} PUBLIC ttbMath fmt::fmt )

add_subdirectory( src )
add_subdirectory( tests )

set( EXPORT_TARGETS ${EXPORT_TARGETS} PARENT_SCOPE )
