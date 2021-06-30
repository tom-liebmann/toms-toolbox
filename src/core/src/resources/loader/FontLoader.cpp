#include <ttb/core/resources/loader/FontLoader.hpp>

#include <ttb/core/resources/io_helper.hpp>


namespace ttb::resources
{
    FontLoader::FontLoader( std::string const& filename ) : DatabaseLoader( filename )
    {
    }

    std::shared_ptr< ttb::Font > FontLoader::load( YAML::Node const& node ) const
    {
        auto const filename = node[ "file" ].as< std::string >();
        auto const lineHeight = node[ "line_height" ].as< float >();
        auto const width = node[ "width" ].as< uint16_t >();
        auto const height = node[ "height" ].as< uint16_t >();

        auto const content = loadFile( rootPath() + "/" + filename );

        return std::make_shared< ttb::Font >( lineHeight, width, height, content );
    }
}
