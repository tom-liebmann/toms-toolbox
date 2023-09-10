target_sources(
    ${TARGET_NAME}

    PRIVATE
        Font.cpp
        TextFactory.cpp
        GlyphLoader.cpp
        TextLayout.cpp
)

add_subdirectory( text_layouts )
