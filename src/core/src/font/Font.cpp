#include <ttb/core/fonts/Font.hpp>

#include <ttb/core/fonts/Glyph.hpp>

#include <fmt/core.h>


namespace ttb
{
    Font::Font() = default;

    Glyph const& Font::getGlyph( char codePoint ) const
    {
        if( auto iter = m_glyphs.find( codePoint ); iter != std::end( m_glyphs ) )
        {
            return iter->second;
        }

        throw std::runtime_error( fmt::format( "Invalid character id: {}", codePoint ) );
    }
}
