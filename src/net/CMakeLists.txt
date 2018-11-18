set(
    SOURCE_FILES
    src/DataReader.cpp
    src/DataWriter.cpp

    src/packets/DataIPacket.cpp
    src/packets/DataOPacket.cpp
    src/packets/IPacket.cpp
    src/packets/OPacket.cpp
    src/packets/SizedIPacket.cpp

    src/PacketBridge.cpp
    )

if( ${COMPILATION_MODE} STREQUAL "GCC" )
    add_library(
        ttbNet
        SHARED
        ${SOURCE_FILES}
        )
elseif( ${COMPILATION_MODE} STREQUAL "CLANG" )
    add_library(
        ttbNet
        STATIC
        ${SOURCE_FILES}
        )
endif()

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

set_property(
    TARGET
    ttbNet
    PROPERTY POSITION_INDEPENDENT_CODE ON)

list( APPEND VALID_TARGETS ttbNet )
set( VALID_TARGETS ${VALID_TARGETS} PARENT_SCOPE )
