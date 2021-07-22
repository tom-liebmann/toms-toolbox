set( TARGET_NAME ttbUi )

ttb_add_module( ${TARGET_NAME} )

if( NOT TARGET ${TARGET_NAME} )
    return()
endif()

target_include_directories( ${TARGET_NAME} PUBLIC include PRIVATE src )

target_link_libraries( ${TARGET_NAME} PUBLIC ttbCore ttbMath ttbUtils )

add_subdirectory( src )
add_subdirectory( tests )

set( EXPORT_TARGETS ${EXPORT_TARGETS} PARENT_SCOPE )
