set( TARGET_NAME ttbNet )

ttb_add_module( ${TARGET_NAME} )

if( NOT TARGET ${TARGET_NAME} )
    return()
endif()

include( boost )

target_include_directories( ${TARGET_NAME} PUBLIC include )

target_link_libraries( ${TARGET_NAME} PUBLIC ttbUtils boost )

add_subdirectory( src )

if( "${BUILD_PLATFORM}" STREQUAL "Android" )

    target_link_libraries( ${TARGET_NAME} PRIVATE android log )

endif()

set( EXPORT_TARGETS ${EXPORT_TARGETS} PARENT_SCOPE )
