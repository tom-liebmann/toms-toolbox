include_guard( GLOBAL )

set( TTB_ROOT_DIR "${CMAKE_CURRENT_LIST_DIR}/../.." )

function( _ttb_project_finish PROJECT_NAME )
    get_property( _CMAKE_FILE TARGET ${PROJECT_NAME} PROPERTY TTB_CMAKE_FILE )

    cmake_policy( SET CMP0076 NEW )
    enable_language( C )
    enable_language( CXX )

    set( TARGET_NAME ${PROJECT_NAME}_EXE )

    add_executable( ${TARGET_NAME} EXCLUDE_FROM_ALL )
    set_target_properties( ${TARGET_NAME} PROPERTIES OUTPUT_NAME ${PROJECT_NAME} )

    find_package( ttb REQUIRED )
    target_link_libraries( ${TARGET_NAME} PRIVATE ttb::core ttb::utils ttb::ui ttb::math )

    target_compile_definitions( ${TARGET_NAME} PUBLIC PLATFORM_${BUILD_PLATFORM_UPPER} )

    add_subdirectory( ${_CMAKE_FILE} _target_${PROJECT_NAME} )
    add_subdirectory( "${TTB_ROOT}/project/src" ttb_project )

    add_dependencies( ${PROJECT_NAME} ${TARGET_NAME} )
endfunction()
