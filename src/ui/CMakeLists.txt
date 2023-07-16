set( TARGET_NAME ttbUi )

ttb_add_module( ${TARGET_NAME} ttb::ui )

if( NOT TARGET ${TARGET_NAME} )
    return()
endif()

ttb_add_module_dependency( ${TARGET_NAME} rapidxml rapidxml::rapidxml )

target_include_directories( ${TARGET_NAME} PUBLIC include PRIVATE src )

target_link_libraries( ${TARGET_NAME} PUBLIC ttb::core ttb::math ttb::utils )

add_subdirectory( src )
add_subdirectory( tests )

set( EXPORT_TARGETS ${EXPORT_TARGETS} PARENT_SCOPE )
