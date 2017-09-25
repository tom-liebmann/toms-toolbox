if( ${COMPILATION_MODE} STREQUAL "CLANG" )

    set(
        SOURCE_FILES
        src/impl/emscripten/TCPSocket.cpp
        )

endif()

add_library(
    ttbNet
    STATIC
    ${SOURCE_FILES}
    )

if( ${COMPILATION_MODE} STREQUAL "CLANG" )

    target_include_directories(
        ttbNet
        PRIVATE
        src/impl/emscripten
        )

endif()

target_include_directories(
    ttbNet
    PUBLIC
    include
    PRIVATE
    src
    )

target_compile_options(
    ttbNet
    PRIVATE
    -std=gnu++14
    -fPIC
    )

target_link_libraries(
    ttbNet
    PUBLIC
    ttbUtils
    )

list( APPEND VALID_TARGETS ttbNet )
set( VALID_TARGETS ${VALID_TARGETS} PARENT_SCOPE )
