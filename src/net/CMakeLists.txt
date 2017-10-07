if( ${COMPILATION_MODE} STREQUAL "CLANG" )

    set(
        SOURCE_FILES
        src/impl/emscripten/TCPSocket.cpp
        src/impl/emscripten/NetSelector.cpp
        src/NetSelector.cpp
        src/DataWriter.cpp
        src/packets/IPacket.cpp
        src/packets/OPacket.cpp
        src/packets/DataIPacket.cpp
        src/packets/DataOPacket.cpp
        src/packets/SizedOPacket.cpp
        src/packets/SizedIPacket.cpp
        )

elseif( ${COMPILATION_MODE} STREQUAL "GCC" )

    set(
        SOURCE_FILES
        src/DataWriter.cpp
        src/Listener.cpp
        src/NetSelector.cpp
        src/Selectable.cpp
        src/WebSocket.cpp
        src/Socket.cpp
        src/impl/linux/Listener.cpp
        src/impl/linux/NetSelector.cpp
        src/impl/linux/Selectable.cpp
        src/impl/linux/TCPSocket.cpp
        src/impl/linux/WebSocket.cpp
        src/packets/DataIPacket.cpp
        src/packets/DataOPacket.cpp
        src/packets/IPacket.cpp
        src/packets/OPacket.cpp
        src/packets/SizedIPacket.cpp
        src/packets/SizedOPacket.cpp
        )

endif()

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
