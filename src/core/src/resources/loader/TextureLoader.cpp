#include <ttb/core/resources/loader/TextureLoader.hpp>


namespace ttb::resources
{
    TextureLoader::TextureLoader( std::string const& filename ) : DatabaseLoader( filename )
    {
    }

    std::shared_ptr< ttb::Texture2D > TextureLoader::load( YAML::Node const& node ) const
    {
        if( auto const fileNode = node[ "file" ]; fileNode )
        {
            auto const file = rootPath() + "/" + fileNode.as< std::string >();

#if defined( PLATFORM_ANDROID )

            auto& assetMngr = AssetManager::instance();
            auto const data = assetMngr.loadData( file );
            auto texture = ttb::TextureFactory::loadData( data.data(), data.size() );

#elif defined( PLATFORM_LINUX ) || defined( PLATFORM_BROWSER )

            auto texture = ttb::TextureFactory::loadPNG( file );

#endif

            auto mod = ttb::modify( texture );
            // mod.generateMipMap();
            // mod.minMagFilter( GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR );
            mod.minMagFilter( GL_LINEAR, GL_LINEAR );
            mod.finish();

            return texture;
        }

        return {};
    }
}
