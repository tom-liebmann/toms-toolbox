target_sources(
    ${TARGET_NAME}

    PRIVATE
        Element.cpp
        ElementWrapper.cpp
        Framework.cpp
)

add_subdirectory( elements )
add_subdirectory( utils )
