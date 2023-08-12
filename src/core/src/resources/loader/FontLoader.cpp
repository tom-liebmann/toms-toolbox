#include <ttb/core/resources/loader/FontLoader.hpp>

#include <ttb/core/fonts/GlyphLoader.hpp>
#include <ttb/core/resources/io_helper.hpp>


namespace ttb::resources
{
    FontLoader::FontLoader( std::string const& filename ) : DatabaseLoader( filename )
    {
    }

    std::shared_ptr< ttb::Font > FontLoader::load( YAML::Node const& node ) const
    {
        auto font = std::shared_ptr< Font >( new Font{} );

        auto const filename = node[ "file" ].as< std::string >();
        auto const content = loadFile( rootPath() + "/" + filename );

        auto fontNode = YAML::Load( content );

        font->m_emSize = fontNode[ "metrics" ][ "emSize" ].as< float >();
        font->m_lineHeight = fontNode[ "metrics" ][ "lineHeight" ].as< float >();
        font->m_texWidth = fontNode[ "atlas" ][ "width" ].as< std::uint32_t >();
        font->m_texHeight = fontNode[ "atlas" ][ "height" ].as< std::uint32_t >();

        for( auto const glyphNode : fontNode[ "glyphs" ] )
        {
            auto const glyph = GlyphLoader::loadFromYaml( glyphNode );
            font->m_glyphs.insert( std::make_pair( glyph.getId(), glyph ) );
        }

        return font;
    }
}
