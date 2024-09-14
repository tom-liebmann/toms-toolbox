target_sources(
    ${TARGET_NAME}

    PRIVATE
        FitExtent.cpp
        Margin.cpp
        Element.cpp
        ElementParent.cpp
        XmlFactory.cpp
        XmlLoader.cpp
        Root.cpp
        fromStringView.cpp
        Extent.cpp
)

add_subdirectory( elements )
add_subdirectory( utils )
