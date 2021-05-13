#include <ttb/core/resources/io_helper.hpp>

#include <ttb/core/resources/AssetManager.hpp>

#if defined( PLATFORM_LINUX ) || defined( PLATFORM_BROWSER )
#include <fstream>
#endif


namespace ttb::resources
{
    std::string dirname( std::string const& filename )
    {
        auto const pos = filename.find_last_of( '/' );
        return filename.substr( 0, pos );
    }

    std::string loadFile( std::string const& filename )
    {
#if defined( PLATFORM_LINUX ) || defined( PLATFORM_BROWSER )

        std::ifstream stream( filename, std::ios::in | std::ios::binary );

        if( !stream.is_open() )
        {
            throw std::runtime_error( "Unable to open file " + filename );
        }

        // determine length of file
        stream.seekg( 0, std::ios::end );
        size_t const size = stream.tellg();
        stream.seekg( 0, std::ios::beg );

        std::vector< char > buffer( size + 1, 0 );

        stream.read( buffer.data(), size );

        return std::string{ buffer.data() };

#elif defined( PLATFORM_ANDROID )

        auto& assetMngr = AssetManager::instance();
        return assetMngr.loadContent( filename );

#endif
    }
}
