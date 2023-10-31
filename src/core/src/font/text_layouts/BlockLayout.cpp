#include <ttb/core/fonts/text_layouts/BlockLayout.hpp>

#include <ttb/core/fonts/Font.hpp>


namespace ttb::font
{
    BlockLayout::BlockLayout( float minSpaceWidth, float maxWidth )
        : m_minSpaceWidth{ minSpaceWidth }, m_maxWidth{ maxWidth }
    {
    }

    void BlockLayout::setMaxWidth( float value )
    {
        m_maxWidth = value;
    }

    float BlockLayout::getMaxWidth() const
    {
        return m_maxWidth;
    }

    void BlockLayout::computeLayout( Font const& font,
                                     float size,
                                     std::string_view text,
                                     CharacterCallback const& callback ) const
    {
        auto const scaleFactor = size / font.getEmSize();

        auto const checkWhitespace = []( char v )
        {
            return isWhiteSpace( v );
        };

        auto wordStart = std::find_if_not( std::begin( text ), std::end( text ), checkWhitespace );

        if( wordStart == std::end( text ) )
        {
            // Text does not contain any non-whitespace characters
            return;
        }

        auto wordEnd = std::find_if( wordStart, std::end( text ), checkWhitespace );

        auto y = 0.0f;
        auto lineStart = wordStart;
        auto lineWordWidth = computeWordWidth( font, wordStart, wordEnd ) * scaleFactor;
        auto lineWordCount = 1u;

        while( true )
        {
            wordStart = std::find_if_not( wordEnd, std::end( text ), checkWhitespace );

            if( wordStart == std::end( text ) )
            {
                break;
            }

            wordEnd = std::find_if( wordStart, std::end( text ), checkWhitespace );

            auto const wordWidth = computeWordWidth( font, wordStart, wordEnd ) * scaleFactor;

            if( lineWordWidth + lineWordCount * m_minSpaceWidth + wordWidth > m_maxWidth )
            {
                printLine( font,
                           size,
                           lineStart,
                           wordStart,
                           lineWordCount == 1u
                               ? 0.0f
                               : ( m_maxWidth - lineWordWidth ) / ( lineWordCount - 1u ),
                           y,
                           callback );

                lineStart = wordStart;
                lineWordWidth = wordWidth;
                lineWordCount = 1u;
                y += font.getLineHeight() * scaleFactor;
            }
            else
            {
                lineWordWidth += wordWidth;
                ++lineWordCount;
            }
        }

        printLine( font, size, lineStart, wordEnd, m_minSpaceWidth, y, callback );
    }

    bool BlockLayout::isWhiteSpace( char character )
    {
        switch( character )
        {
            case ' ':
            case '\n':
            case '\r':
            case '\t':
                return true;
            default:
                return false;
        }
    }

    float BlockLayout::computeWordWidth( Font const& font,
                                         std::string_view::const_iterator start,
                                         std::string_view::const_iterator end )
    {
        auto x = 0.0f;
        auto width = 0.0f;

        for( auto iter = start; iter != end; ++iter )
        {
            auto const& glyph = font.getGlyph( *iter );
            width = std::max( width, x + glyph.getRange().getMax( 0 ) );
            x += glyph.getAdvance();
        }

        return width;
    }

    void BlockLayout::printLine( Font const& font,
                                 float size,
                                 std::string_view::const_iterator start,
                                 std::string_view::const_iterator end,
                                 float spaceWidth,
                                 float y,
                                 CharacterCallback const& callback )
    {
        auto const scaleFactor = size / font.getEmSize();
        auto x = 0.0f;
        auto lastWasWhitespace = false;

        for( auto iter = start; iter != end; ++iter )
        {
            if( isWhiteSpace( *iter ) )
            {
                if( !lastWasWhitespace )
                {
                    x += spaceWidth;
                }
                lastWasWhitespace = true;
                continue;
            }

            lastWasWhitespace = false;

            auto const& glyph = font.getGlyph( *iter );

            callback( glyph, x, y );

            x += glyph.getAdvance() * scaleFactor;
        }
    }
}
