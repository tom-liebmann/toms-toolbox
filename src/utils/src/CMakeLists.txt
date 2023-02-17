target_sources(
    ${TARGET_NAME}

    PRIVATE
        base64.cpp
        Endianness.cpp
        SHA1.cpp
        RingBufferWriter.cpp
        RingBuffer.cpp
        EventManager.cpp
        UnionFind.cpp
        Event.cpp
        Color.cpp
        TypeValueEvent.cpp
)

add_subdirectory( data_io )
add_subdirectory( dcel )
add_subdirectory( gesture )
add_subdirectory( event )
add_subdirectory( random )
add_subdirectory( algorithm )
add_subdirectory( co )
