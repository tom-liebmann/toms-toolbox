#include <catch2/catch.hpp>

#include "ResultGlyph.hpp"
#include "TestFontFactory.hpp"

#include <ttb/core/fonts/Font.hpp>
#include <ttb/core/fonts/text_layouts/BlockLayout.hpp>


using namespace ttb;


TEST_CASE( "BlockLayout", "[font][layout]" )
{
    struct TestData
    {
        std::string inputText;
        float minSpaceWidth;
        float maxTextWidth;
        std::vector< ResultGlyph > expectedGlyphs;
    };

    auto const& testData =
        GENERATE( TestData{ "a", 0.0f, 5.0f, { { 'a', 0.0f, 0.0f } } },
                  TestData{ "ba", 0.0f, 5.0f, { { 'b', 0.0f, 0.0f }, { 'a', 2.0f, 0.0f } } },
                  TestData{ "b b", 0.5f, 5.0f, { { 'b', 0.0f, 0.0f }, { 'b', 2.5f, 0.0f } } },
                  TestData{ "b b b",
                            0.5f,
                            5.0f,
                            { { 'b', 0.0f, 0.0f }, { 'b', 3.0f, 0.0f }, { 'b', 0.0f, 1.0f } } },
                  TestData{ "aa aa b b b",
                            0.5f,
                            5.0f,
                            { { 'a', 0.0f, 0.0f },
                              { 'a', 1.0f, 0.0f },
                              { 'a', 3.0f, 0.0f },
                              { 'a', 4.0f, 0.0f },
                              { 'b', 0.0f, 1.0f },
                              { 'b', 3.0f, 1.0f },
                              { 'b', 0.0f, 2.0f } } } );

    auto font = TestFontFactory::createTestFont();

    auto layout = ttb::font::BlockLayout{ testData.minSpaceWidth };

    auto resultGlyphs = std::vector< ResultGlyph >{};

    layout.computeLayout(       //
        font,                   // font
        1.0f,                   // size
        testData.inputText,     // text
        testData.maxTextWidth,  // maxWidth
        [ &resultGlyphs ]( auto const& glyph, auto x, auto y )
        {
            resultGlyphs.push_back( ResultGlyph{ glyph.getId(), x, y } );
        } );

    REQUIRE( resultGlyphs == testData.expectedGlyphs );
}
