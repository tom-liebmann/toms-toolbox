set( TARGET_NAME ttbUtils )

ttb_add_module( ${TARGET_NAME} )

if( NOT TARGET ${TARGET_NAME} )
    return()
endif()

target_include_directories( ${TARGET_NAME} PUBLIC include )

target_link_libraries( ${TARGET_NAME} PUBLIC ttbMath )

add_subdirectory( src )

set( EXPORT_TARGETS ${EXPORT_TARGETS} PARENT_SCOPE )
