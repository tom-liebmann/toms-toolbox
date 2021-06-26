target_sources(
    ${TARGET_NAME}

    PRIVATE
        Element.cpp
        CombinedElement.cpp
        WrappedElement.cpp
        Framework.cpp
)

add_subdirectory( elements )
add_subdirectory( utils )
