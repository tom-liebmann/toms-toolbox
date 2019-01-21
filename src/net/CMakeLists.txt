set(
    SOURCE_FILES

    src/DataReader.cpp
    src/DataWriter.cpp
    src/BufferedWriter.cpp

    src/packets/DataIPacket.cpp
    src/packets/DataOPacket.cpp
    src/packets/IPacket.cpp
    src/packets/OPacket.cpp
    src/packets/SizedIPacket.cpp
    src/packets/SubIPacket.cpp

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

target_compile_options(
    ttbNet
    PUBLIC
        -Wall
        -Wextra
        -Wpedantic
        -Wconversion
    )

set_target_properties(
    ttbNet
    PROPERTIES
        POSITION_INDEPENDENT_CODE ON
        CXX_STANDARD 17
    )

list( APPEND VALID_TARGETS ttbNet )
set( VALID_TARGETS ${VALID_TARGETS} PARENT_SCOPE )
