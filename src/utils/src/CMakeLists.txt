target_sources(
    ${TARGET_NAME}

    PRIVATE
        base64.cpp
        Endianness.cpp
        SHA1.cpp
        RingBufferWriter.cpp
        RingBuffer.cpp
)

add_subdirectory( data_io )
add_subdirectory( dcel )
