target_sources(
    ${TARGET_NAME}

    PRIVATE
        Element.cpp
        ElementWrapper.cpp
)

add_subdirectory( elements )
add_subdirectory( utils )
