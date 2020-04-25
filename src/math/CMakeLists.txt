set( TARGET_NAME ttbMath )

add_library( ttbMath SHARED )

target_include_directories(
    ttbMath

    PUBLIC
        include

    PRIVATE
        src
)

target_compile_options(
    ttbMath

    PUBLIC
        -Wall
        -Wextra
        -Wpedantic
)

set_target_properties(
    ttbMath

    PROPERTIES
        POSITION_INDEPENDENT_CODE ON
        CXX_STANDARD 17
)

target_compile_features(
    ttbMath

    PUBLIC
        cxx_std_17
)

add_subdirectory( src )

list( APPEND VALID_TARGETS ttbMath )
set( VALID_TARGETS ${VALID_TARGETS} PARENT_SCOPE )
