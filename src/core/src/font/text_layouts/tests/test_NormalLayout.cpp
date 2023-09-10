#include <catch2/catch.hpp>

#include <ttb/core/fonts/Font.hpp>
#include <ttb/core/fonts/text_layouts/NormalLayout.hpp>


using namespace ttb;


namespace
{
    Glyph createTestGlyph( char id, float advance )
    {
        auto glyph = Glyph{};
        glyph.m_id = id;
        glyph.m_advance = advance;
        return glyph;
    }

    Font createTestFont()
    {
        auto font = Font{};
        font.m_emSize = 1.0f;
        font.m_lineHeight = 1.0f;
        font.m_texWidth = 512;
        font.m_texHeight = 512;
        font.m_glyphs = { { 'a', createTestGlyph( 'a', 1.0f ) } };
        return font;
    }
}


TEST_CASE( "NormalLayout", "[font][layout]" )
{
    struct ResultGlyph
    {
        char id;
        float x;
        float y;

        bool operator<=>( ResultGlyph const& ) const = default;
    };

    struct TestData
    {
        std::string inputText;
        std::vector< ResultGlyph > expectedGlyphs;
    };

    auto const& testData = GENERATE( TestData{ "a", { { 'a', 0.0f, 0.0f } } } );

    auto font = createTestFont();

    auto layout = ttb::font::NormalLayout{};

    auto resultGlyphs = std::vector< ResultGlyph >{};

    layout.computeLayout(    //
        font,                // font
        1.0f,                // size
        testData.inputText,  // text
        5.0f,                // maxWidth
        [ &resultGlyphs ]( auto const& glyph, auto x, auto y )
        {
            resultGlyphs.push_back( ResultGlyph{ glyph.getId(), x, y } );
        } );

    REQUIRE( resultGlyphs == testData.expectedGlyphs );
}
