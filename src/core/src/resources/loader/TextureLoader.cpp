#include <ttb/core/resources/loader/TextureLoader.hpp>


namespace ttb::resources
{
    TextureLoader::TextureLoader( std::string const& filename ) : DatabaseLoader( filename )
    {
    }

    std::shared_ptr< ttb::Texture2D > TextureLoader::load( YAML::Node const& node ) const
    {
        if( auto const fileNode = node[ "file" ] )
        {
            auto const file = rootPath() + "/" + fileNode.as< std::string >();

            uint8_t levels = 1;

            if( auto const levelNode = node[ "levels" ] )
            {
                levels = levelNode.as< int >();
            }

#if defined( PLATFORM_ANDROID )

            auto& assetMngr = AssetManager::instance();
            auto const data = assetMngr.loadData( file );
            auto texture = ttb::TextureFactory::loadData( data.data(), data.size(), levels );

#elif defined( PLATFORM_LINUX ) || defined( PLATFORM_BROWSER )

            auto texture = ttb::TextureFactory::loadPNG( file, levels );

#else

#error Unknown platform

#endif

            return texture;
        }

        return {};
    }
}
