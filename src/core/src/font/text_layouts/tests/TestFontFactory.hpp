#pragma once

#include <ttb/core/fonts/Font.hpp>
#include <ttb/core/fonts/Glyph.hpp>


class TestFontFactory
{
public:
    TestFontFactory() = delete;

    static ttb::Glyph createTestGlyph( char id, float advance, float width );

    static ttb::Font createTestFont();
};
