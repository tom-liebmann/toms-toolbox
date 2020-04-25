set( TARGET_NAME ttbUtils )

add_library( ttbUtils SHARED )

target_include_directories(
    ttbUtils

    PUBLIC
        include
)

target_compile_options(
    ttbUtils

    PUBLIC
        -Wall
        -Wextra
        -Wpedantic
)

set_target_properties(
    ttbUtils

    PROPERTIES
        POSITION_INDEPENDENT_CODE ON
        CXX_STANDARD 17
)

target_compile_features(
    ttbUtils

    PUBLIC
        cxx_std_17
)

add_subdirectory( src )

list( APPEND VALID_TARGETS ttbUtils )
set( VALID_TARGETS ${VALID_TARGETS} PARENT_SCOPE )
