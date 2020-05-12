target_sources(
    ${TARGET_NAME}

    PRIVATE
        dummy.cpp
        base64.cpp
        Endianness.cpp
        SHA1.cpp
)

add_subdirectory( data_io )
