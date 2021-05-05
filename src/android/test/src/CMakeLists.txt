message( STATUS "Project CMake" )

target_sources(
    ${TARGET_NAME}
    PRIVATE
        main.cpp
)

set( BUILD_PLATFORM "Android" CACHE PATH "" FORCE )
set( LIBRARY_TYPE "SHARED" CACHE STRING "" FORCE )
add_subdirectory( "${CMAKE_CURRENT_LIST_DIR}/../../../.." toms-toolbox )

target_link_libraries(
    ${TARGET_NAME}
    PRIVATE
        ttb
)
