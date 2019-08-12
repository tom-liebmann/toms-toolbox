add_executable(
    data_io
    src/main.cpp
    )

target_link_libraries(
    data_io
    PUBLIC
        ttbUtils
    )
