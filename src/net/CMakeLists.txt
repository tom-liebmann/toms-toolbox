if( ${COMPILATION_MODE} STREQUAL "CLANG" )

    set(
        SOURCE_FILES
        src/impl/emscripten/TCPSocket.cpp
        src/packets/IPacket.cpp
        src/packets/OPacket.cpp
        src/packets/DataIPacket.cpp
        src/packets/DataOPacket.cpp
        src/packets/SizedOPacket.cpp
        src/packets/SizedIPacket.cpp
        src/packets/CombinedOPacket.cpp
        )

elseif( ${COMPILATION_MODE} STREQUAL "GCC" )

    set(
        SOURCE_FILES
        src/Listener.cpp
        src/impl/linux/TCPSocket.cpp
        src/impl/linux/Listener.cpp
        src/packets/IPacket.cpp
        src/packets/OPacket.cpp
        src/packets/DataIPacket.cpp
        src/packets/DataOPacket.cpp
        src/packets/SizedOPacket.cpp
        src/packets/SizedIPacket.cpp
        src/packets/CombinedOPacket.cpp
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
