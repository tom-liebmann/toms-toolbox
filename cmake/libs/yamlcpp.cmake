if( TARGET yamlcpp )
    return()
endif()

set( YAML_BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE )
add_compile_options( -fPIC )
add_subdirectory( "${CMAKE_CURRENT_LIST_DIR}/../../external/yaml-cpp" yaml_cpp )

add_library( yamlcpp INTERFACE IMPORTED )

set_target_properties( yamlcpp PROPERTIES
    INTERFACE_LINK_LIBRARIES yaml-cpp
)
