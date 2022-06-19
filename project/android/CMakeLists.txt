cmake_minimum_required( VERSION 3.10 )

set( TTB_ROOT "${CMAKE_CURRENT_LIST_DIR}/../.." )

cmake_policy( SET CMP0076 NEW ) # allow relative target_sources
cmake_policy( SET CMP0079 NEW ) # allow out-of-tree linkage

# To locate and collect all library files for packaging, packages added in
# subdirectories have to be put into the global scope. Otherwise, the library
# targets would not be available in this file's scope.
set( CMAKE_FIND_PACKAGE_TARGETS_GLOBAL TRUE )

project( ${PROJECT_NAME} )

set( TARGET_NAME "project_library" )

add_library( ${TARGET_NAME} SHARED )

target_compile_features( ${TARGET_NAME} PUBLIC cxx_std_20 )
target_compile_definitions( ${TARGET_NAME} PUBLIC PLATFORM_ANDROID )

find_package( ttb REQUIRED )

add_subdirectory( ${PROJECT_CMAKE_FILE} ${PROJECT_NAME} )

add_subdirectory( "${TTB_ROOT}/project/src" ttb_project )

target_link_libraries(
    ${TARGET_NAME}
    PUBLIC
        GLESv2
        android
        log
)


function( collect_link_libraries TARGET OUT_VAR )
    get_target_property( _LINK_LIBRARIES ${TARGET} INTERFACE_LINK_LIBRARIES )

    foreach( _LINK_LIBRARY ${_LINK_LIBRARIES} )

        if( NOT TARGET ${_LINK_LIBRARY} )
            continue()
        endif()

        get_target_property( _TARGET_TYPE ${_LINK_LIBRARY} TYPE )
        get_target_property( _TARGET_LOCATION ${_LINK_LIBRARY} LOCATION )

        if( _TARGET_TYPE STREQUAL "INTERFACE_LIBRARY" )
            collect_link_libraries( ${_LINK_LIBRARY} ${OUT_VAR} )
            continue()
        endif()

        get_target_property( _TARGET_LOCATION ${_LINK_LIBRARY} LOCATION )

        if( _TARGET_LOCATION )
            message( STATUS "Found dependency: ${_TARGET_LOCATION}" )
            list( APPEND ${OUT_VAR} ${_TARGET_LOCATION} )
        else()
            message( ERROR "Unable to retrieve location of library ${_LINK_LIBRARY}" )
        endif()
    endforeach()

    set( ${OUT_VAR} ${${OUT_VAR}} PARENT_SCOPE )
endfunction()

collect_link_libraries( ${TARGET_NAME} _LIBRARIES )

foreach( _LIBRARY ${_LIBRARIES} )
    install( FILES ${_LIBRARY} TYPE LIB )
endforeach()

install( TARGETS ${TARGET_NAME} )
