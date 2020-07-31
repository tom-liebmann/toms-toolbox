target_sources(
    ${TARGET_NAME}

    PRIVATE
        Packet.cpp
        RandomAccessPacket.cpp
        Writer.cpp
)

add_subdirectory( packets )
add_subdirectory( writers )
