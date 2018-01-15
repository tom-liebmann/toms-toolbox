#include <ttb/core/fonts/Font.hpp>

#include <fstream>
#include <regex>


namespace ttb
{
    Font::Character::Character( std::istream& stream )
    {
        static std::regex const charReg(
            "^char "
            "id=(\\d+)\\s+x=(-?\\d+)\\s+y=(-?\\d+)\\s+width=(-?\\d+)\\s+height=(-?\\d+)\\s+xoffset="
            "(-?\\d+)"
            "\\s+"
            "yoffset=(-?\\d+)\\s+xadvance=(-?\\d+).*$" );

        std::string line;
        std::getline( stream, line );

        std::smatch match;
        if( !std::regex_match( line, match, charReg ) )
        {
            throw std::runtime_error( "Invalid file format" );
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


    Font::Font( std::string const& filename )
    {
        std::ifstream stream( filename );

        std::string line;
        std::smatch match;

        std::getline( stream, line );
        std::getline( stream, line );

        std::regex lineHeightReg( "^common lineHeight=(\\d+).*$" );
        if( !std::regex_match( line, match, lineHeightReg ) )
        {
            throw std::runtime_error( "Invalid file format" );
        }

        m_lineHeight = std::stof( match[ 1 ].str() );

        std::getline( stream, line );

        std::regex fileReg( "^page id=(\\d+) file=\"(\\w+\\.png)\"\\s*$" );
        if( !std::regex_match( line, match, fileReg ) )
        {
            throw std::runtime_error( "Invalid file format" );
        }

        std::string textureFile = match[ 2 ].str();

        std::getline( stream, line );

        std::regex countReg( "^chars count=(\\d+)\\s*$" );
        if( !std::regex_match( line, match, countReg ) )
        {
            throw std::runtime_error( "Invalid file format" );
        }

        size_t charCount = std::stoi( match[ 1 ].str() );

        for( size_t lineNr = 0; lineNr < charCount; ++lineNr )
        {
            Character c( stream );

            m_characters.insert( { c.id(), c } );
        }

        // Load texture file
        std::regex dirReg( "^(.*\\/).*\\.fnt$" );
        if( !std::regex_match( filename, match, dirReg ) )
        {
            throw std::runtime_error( "Invalid file directory" );
        }
        m_texture = TextureFactory::loadPNG( match[ 1 ].str() + textureFile );
        m_texture->modify()->minMagFilter( GL_LINEAR, GL_LINEAR );
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

    std::shared_ptr< ttb::Texture2D > const& Font::texture() const
    {
        return m_texture;
    }
}
