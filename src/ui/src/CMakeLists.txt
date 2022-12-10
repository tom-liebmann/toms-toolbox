target_sources(
    ${TARGET_NAME}

    PRIVATE
        Element.cpp
        CombinedElement.cpp
        WrappedElement.cpp
        XmlFactory.cpp
        XmlLoader.cpp
        Root.cpp
)

add_subdirectory( elements )
add_subdirectory( utils )
