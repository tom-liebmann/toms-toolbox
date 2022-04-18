target_sources(
    ${TARGET_NAME}

    PRIVATE
        Packet.cpp
        RandomAccessPacket.cpp
        Writer.cpp
        PacketWriteHelper.cpp
        PacketBridge.cpp
        Reader.cpp
)

add_subdirectory( packets )
add_subdirectory( writers )
add_subdirectory( reader )
