#include <catch2/catch.hpp>

#include "TestFontFactory.hpp"

#include <ttb/core/fonts/Font.hpp>
#include <ttb/core/fonts/text_layouts/NormalLayout.hpp>


using namespace ttb;


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

    auto const& testData = GENERATE( TestData{ "a", { { 'a', 0.0f, 0.0f } } },
                                     TestData{ "aa",
                                               {
                                                   { 'a', 0.0f, 0.0f },
                                                   { 'a', 1.0f, 0.0f },
                                               } },
                                     TestData{ "ba",
                                               {
                                                   { 'b', 0.0f, 0.0f },
                                                   { 'a', 2.0f, 0.0f },
                                               } },
                                     TestData{ "a\na",
                                               {
                                                   { 'a', 0.0f, 0.0f },
                                                   { 'a', 0.0f, 1.0f },
                                               } } );

    auto font = TestFontFactory::createTestFont();

    auto layout = ttb::font::NormalLayout{};

    auto resultGlyphs = std::vector< ResultGlyph >{};

    layout.computeLayout(    //
        font,                // font
        1.0f,                // size
        testData.inputText,  // text
        [ &resultGlyphs ]( auto const& glyph, auto x, auto y )
        {
            resultGlyphs.push_back( ResultGlyph{ glyph.getId(), x, y } );
        } );

    REQUIRE( resultGlyphs == testData.expectedGlyphs );
}
