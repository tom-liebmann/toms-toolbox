set( TARGET_NAME ttbMath )

set( ACTIVATE_${TARGET_NAME} ON CACHE BOOL "Enable module ${TARGET_NAME}" )

if( NOT ACTIVATE_${TARGET_NAME} )
    return()
endif()

add_library( ${TARGET_NAME} INTERFACE )

target_include_directories(
    ${TARGET_NAME}

    INTERFACE
        include
)

target_compile_features(
    ${TARGET_NAME}

    INTERFACE
        cxx_std_17
)

target_link_libraries(
    ttb

    INTERFACE
        ${TARGET_NAME}
)
