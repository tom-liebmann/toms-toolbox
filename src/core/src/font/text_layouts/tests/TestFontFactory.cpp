#include "TestFontFactory.hpp"


ttb::Glyph TestFontFactory::createTestGlyph( char id, float advance, float width )
{
    auto glyph = ttb::Glyph{};
    glyph.m_id = id;
    glyph.m_advance = advance;
    glyph.m_range = ttb::Range{ { 0.0f, 0.0f }, { width, 1.0f } };
    return glyph;
}

ttb::Font TestFontFactory::createTestFont()
{
    auto font = ttb::Font{};
    font.m_emSize = 1.0f;
    font.m_lineHeight = 1.0f;
    font.m_texWidth = 512;
    font.m_texHeight = 512;
    font.m_glyphs = {
        { 'a', createTestGlyph( 'a', 1.0f, 1.0f ) },
        { 'b', createTestGlyph( 'b', 2.0f, 2.0f ) },
    };
    return font;
}
