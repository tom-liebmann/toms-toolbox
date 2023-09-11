#pragma once

#include <iostream>

struct ResultGlyph
{
    char id;
    float x;
    float y;

    bool operator<=>( ResultGlyph const& ) const = default;
};


std::ostream& operator<<( std::ostream& stream, ResultGlyph const& glyph );
