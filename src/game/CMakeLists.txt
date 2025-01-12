set( TARGET_NAME ttbGame )

ttb_add_module( ${TARGET_NAME} ttb::game )

if( NOT TARGET ${TARGET_NAME} )
    return()
endif()

target_include_directories( ${TARGET_NAME} PUBLIC include PRIVATE src )
target_include_directories( ${TARGET_NAME} PRIVATE ${CMAKE_INCLUDE_PATH} )

target_link_libraries( ${TARGET_NAME} PUBLIC ttb::core ttb::math ttb::utils )

ttb_add_module_dependency( ${TARGET_NAME} yaml-cpp yaml-cpp )

add_subdirectory( src )

set( EXPORT_TARGETS ${EXPORT_TARGETS} PARENT_SCOPE )
set( EXPORT_DEPENDENCIES ${EXPORT_DEPENDENCIES} PARENT_SCOPE )
