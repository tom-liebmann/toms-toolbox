target_sources(
    ${TARGET_NAME}

    PRIVATE
        Element.cpp
        ElementParent.cpp
        CombinedElement.cpp
        WrappedElement.cpp
        XmlFactory.cpp
        XmlLoader.cpp
        Root.cpp
        fromStringView.cpp
)

add_subdirectory( elements )
add_subdirectory( utils )
