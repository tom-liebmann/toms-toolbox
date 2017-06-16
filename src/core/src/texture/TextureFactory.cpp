#include <ttb/core/texture/TextureFactory.hpp>

#include <ttb/core/PNGLoader.hpp>

namespace ttb
{
    namespace TextureFactory
    {
        std::unique_ptr< Texture2D > loadPNG( std::string const& filename )
        {
            uint8_t* pngData;
            unsigned int width, height;
            lodepng_decode_file( &pngData, &width, &height, filename.c_str(), LCT_RGBA, 8 );

            std::unique_ptr< Texture2D > texture( new Texture2D() );
            {
                auto modifier = texture->modify();

                modifier->upload( width, height, GL_RGBA8, GL_RGBA, GL_UNSIGNED_BYTE, pngData );
                modifier->minMagFilter( GL_LINEAR, GL_LINEAR );
            }

            free( pngData );

            return texture;
        }
    }
}
