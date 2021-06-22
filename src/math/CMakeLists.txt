set( TARGET_NAME ttbMath )

ttb_add_module( ${TARGET_NAME} )

if( NOT TARGET ${TARGET_NAME} )
    return()
endif()

target_include_directories(
    ${TARGET_NAME}

    PUBLIC
        include
)

add_subdirectory( src )
add_subdirectory( tests )

set( EXPORT_TARGETS ${EXPORT_TARGETS} PARENT_SCOPE )
