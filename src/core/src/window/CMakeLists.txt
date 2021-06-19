target_sources(
    ${TARGET_NAME}

    PRIVATE
        Window.cpp
)

if( "${BUILD_PLATFORM}" STREQUAL "Linux" )
    add_subdirectory( platform_linux )
elseif( "${BUILD_PLATFORM}" STREQUAL "Android" )
    add_subdirectory( platform_android )
elseif( "${BUILD_PLATFORM}" STREQUAL "Browser" )
    add_subdirectory( platform_browser )
else()
    message( FATAL_ERROR "Build platform ${BUILD_PLATFORM} currently not supported" )
endif()

set( EXPORT_DEPENDENCIES ${EXPORT_DEPENDENCIES} PARENT_SCOPE )
