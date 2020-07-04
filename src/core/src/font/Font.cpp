#include <ttb/core/fonts/Font.hpp>

#include <fstream>
#include <regex>


namespace ttb
{
    Font::Character::Character( std::string const& line )
    {
        static std::regex const charReg(
            "^char "
            "id=(\\d+)\\s+x=(-?\\d+)\\s+y=(-?\\d+)\\s+width=(-?\\d+)\\s+height=(-?\\d+)\\s+xoffset="
            "(-?\\d+)"
            "\\s+"
            "yoffset=(-?\\d+)\\s+xadvance=(-?\\d+).*$" );

        std::smatch match;
        if( !std::regex_match( line, match, charReg ) )
        {
            throw std::runtime_error( "Invalid file format (missing char match)" );
        }

        m_id = std::stof( match[ 1 ].str() );
        m_x = std::stof( match[ 2 ].str() );
        m_y = std::stof( match[ 3 ].str() );
        m_width = std::stof( match[ 4 ].str() );
        m_height = std::stof( match[ 5 ].str() );
        m_xOffset = std::stof( match[ 6 ].str() );
        m_yOffset = std::stof( match[ 7 ].str() );
        m_xAdvance = std::stof( match[ 8 ].str() );
    }

    char Font::Character::id() const
    {
        return m_id;
    }

    float Font::Character::x() const
    {
        return m_x;
    }

    float Font::Character::y() const
    {
        return m_y;
    }

    float Font::Character::width() const
    {
        return m_width;
    }

    float Font::Character::height() const
    {
        return m_height;
    }

    float Font::Character::xOffset() const
    {
        return m_xOffset;
    }

    float Font::Character::yOffset() const
    {
        return m_yOffset;
    }

    float Font::Character::xAdvance() const
    {
        return m_xAdvance;
    }


    Font::Font( float lineHeight, uint16_t width, uint16_t height, std::string const& charFile )
        : m_lineHeight{ lineHeight }, m_width{ width }, m_height{ height }
    {
        std::ifstream stream{ charFile };

        std::string line;

        while( true )
        {
            if( std::getline( stream, line ) )
            {
                auto const character = Character{ line };
                m_characters.insert( { character.id(), character } );
            }
            else
            {
                break;
            }
        }
    }

    Font::Character const& Font::character( char index ) const
    {
        auto iter = m_characters.find( index );

        if( iter == std::end( m_characters ) )
        {
            throw std::runtime_error( "Invalid character id" );
        }

        return iter->second;
    }

    float Font::lineHeight() const
    {
        return m_lineHeight;
    }

    uint16_t Font::width() const
    {
        return m_width;
    }

    uint16_t Font::height() const
    {
        return m_height;
    }

    ttb::Range< float, 2 > Font::textDimensions( float size, std::string const& text ) const
    {
        ttb::Range< float, 2 > dimensions{ { 0.0f, 0.0f }, { 0.0f, 0.0f } };

        float scaleFactor = size / lineHeight();

        float x = 0.0f;
        float y = 0.0f;
        for( size_t i = 0; i < text.size(); ++i )
        {
            char cId = text[ i ];

            if( cId != '\n' )
            {
                auto const& c = character( text[ i ] );

                auto charRightX = ( x + c.xOffset() + c.width() ) * scaleFactor;
                auto charBottomY = ( y + c.yOffset() + c.height() ) * scaleFactor;

                dimensions.max( 0, std::max( dimensions.getMax( 0 ), charRightX ) );
                dimensions.max( 1, std::max( dimensions.getMax( 1 ), charBottomY ) );

                x += c.xAdvance();
            }
            else
            {
                x = 0.0f;
                y += lineHeight();
            }
        }

        return dimensions;
    }
}
