if( ${COMPILATION_MODE} STREQUAL "CLANG" )
    add_executable(
    client

    client/main.cpp
    )

    target_link_libraries(
        client
        PRIVATE
        ttbNet
        )

    install(
        FILES
        ${CMAKE_CURRENT_SOURCE_DIR}/client/index.html
        ${CMAKE_CURRENT_BINARY_DIR}/client.js
        DESTINATION ${CMAKE_INSTALL_PREFIX}
        )

    install(
        TARGETS client
        DESTINATION ${CMAKE_INSTALL_PREFIX}
        )
else()
    add_executable(
        server

        server/main.cpp
        )

    target_link_libraries(
        server
        PRIVATE
        ttbNet
        )
endif()



