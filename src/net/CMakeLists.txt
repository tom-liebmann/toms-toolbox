if( ${COMPILATION_MODE} STREQUAL "GCC" )

    # find source files
    file(
        GLOB_RECURSE
        SOURCE_FILES
        ABSOLUTE "${CMAKE_CURRENT_SOURCE_DIR}"
        src/*.cpp )

    add_library(
        ttbNet
        STATIC
        ${SOURCE_FILES} )

    target_include_directories(
        ttbNet
        PUBLIC
        include
        PRIVATE
        src )

    target_compile_options(
        ttbNet
        PRIVATE
        -std=gnu++14
        -fPIC )

    target_link_libraries(
        ttbNet
        PUBLIC
        ttbUtils )

    list( APPEND VALID_TARGETS ttbNet )
    set( VALID_TARGETS ${VALID_TARGETS} PARENT_SCOPE )

endif()
