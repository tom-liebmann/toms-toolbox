set(
    SOURCE_FILES
    src/DataWriter.cpp
    src/Listener.cpp
    src/NetSelector.cpp
    src/Selectable.cpp
    src/Socket.cpp
    src/WebSocket.cpp
    src/impl/posix/Listener.cpp
    src/impl/posix/NetSelector.cpp
    src/impl/posix/Selectable.cpp
    src/impl/posix/TCPSocket.cpp
    src/impl/posix/webSocket/WebSocket.cpp
    src/impl/posix/webSocket/states/SocketState.cpp
    src/impl/posix/webSocket/states/ReceivingHandshakeState.cpp
    src/impl/posix/webSocket/states/SendingHandshakeState.cpp
    src/impl/posix/webSocket/states/ConnectedState.cpp
    src/impl/posix/SocketDataWriter.cpp
    src/packets/DataIPacket.cpp
    src/packets/DataOPacket.cpp
    src/packets/IPacket.cpp
    src/packets/OPacket.cpp
    src/packets/SizedIPacket.cpp
    src/packets/SizedOPacket.cpp
    src/packets/WebSocketOPacket.cpp
    )

add_library(
    ttbNet
    STATIC
    ${SOURCE_FILES}
    )

target_include_directories(
    ttbNet
    PUBLIC
    include
    PRIVATE
    src
    )

target_link_libraries(
    ttbNet
    PUBLIC
    ttbUtils
    )

list( APPEND VALID_TARGETS ttbNet )
set( VALID_TARGETS ${VALID_TARGETS} PARENT_SCOPE )
