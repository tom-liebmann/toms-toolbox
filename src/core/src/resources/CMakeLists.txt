target_sources(
    ${TARGET_NAME}

    PRIVATE
        Container.cpp
        io_helper.cpp
)

if( "${BUILD_PLATFORM}" STREQUAL "Android" )

    target_sources(
        ${TARGET_NAME}

        PUBLIC
            AssetManager.cpp
    )

endif()

add_subdirectory( loader )
