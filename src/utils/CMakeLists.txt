set( TARGET_NAME ttbUtils )

ttb_add_module( ${TARGET_NAME} ttb::utils )

if( NOT TARGET ${TARGET_NAME} )
    return()
endif()

ttb_add_module_dependency( ${TARGET_NAME} fmt fmt::fmt )

target_include_directories( ${TARGET_NAME} PUBLIC include )

target_link_libraries( ${TARGET_NAME} PUBLIC ttb::math )

add_subdirectory( src )
add_subdirectory( tests )

set( EXPORT_TARGETS ${EXPORT_TARGETS} PARENT_SCOPE )
