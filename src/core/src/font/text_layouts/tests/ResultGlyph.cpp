#include "ResultGlyph.hpp"

std::ostream& operator<<( std::ostream& stream, ResultGlyph const& glyph )
{
    return stream << "Glyph{" << static_cast< int >( glyph.id ) << ", " << glyph.x << ", "
                  << glyph.y << "}";
}
