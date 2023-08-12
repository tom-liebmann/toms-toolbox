#include <ttb/core/fonts/GlyphLoader.hpp>

#include <ttb/core/fonts/Glyph.hpp>
#include <ttb/math/Range.hpp>

#include <cstdint>


namespace
{
    ttb::Range< float, 2 > parseBounds( YAML::Node const& node );
}


namespace ttb
{
    Glyph GlyphLoader::loadFromYaml( YAML::Node const& node )
    {
        auto glyph = Glyph{};

        glyph.m_id = node[ "unicode" ].as< std::uint32_t >();
        glyph.m_advance = node[ "advance" ].as< float >();

        if( auto const planeBounds = node[ "planeBounds" ] )
        {
            glyph.m_range = parseBounds( planeBounds );
        }
        else
        {
            glyph.m_range = ttb::Range< float, 2 >{ { 0.0f, 0.0f }, { 0.0f, 0.0f } };
        }

        if( auto const atlasBounds = node[ "atlasBounds" ] )
        {
            glyph.m_texRange = parseBounds( atlasBounds );
        }
        else
        {
            glyph.m_texRange = ttb::Range< float, 2 >{ { 0.0f, 0.0f }, { 0.0f, 0.0f } };
        }

        return glyph;
    }
}


namespace
{
    ttb::Range< float, 2 > parseBounds( YAML::Node const& node )
    {
        return { {
                     node[ "left" ].as< float >(),
                     node[ "bottom" ].as< float >(),
                 },
                 {
                     node[ "right" ].as< float >(),
                     node[ "top" ].as< float >(),
                 } };
    }
}
