#include <iostream>
#include <ttb/core/PNGLoader.hpp>
#include <ttb/core/texture/TextureFactory.hpp>

namespace ttb
{
    namespace TextureFactory
    {
        std::shared_ptr< Texture2D > loadPNG( std::string const& filename )
        {
            uint8_t* pngData;
            unsigned int width, height;
            lodepng_decode_file( &pngData, &width, &height, filename.c_str(), LCT_RGBA, 8 );

            auto texture =
                Texture2D::create()
                    .upload( width, height, GL_RGBA8, GL_RGBA, GL_UNSIGNED_BYTE, pngData )
                    .minMagFilter( GL_LINEAR, GL_LINEAR )
                    .finish();

            free( pngData );

            return texture;
        }

        std::unique_ptr< Texture3D > loadPNGArray( std::vector< std::string > const& filenames )
        {
            unsigned int width, height;

            std::unique_ptr< uint8_t[] > textureData;

            // copy image data into continuous memory area
            // TODO: get png decode to accept an already allocated memory region to fill the image
            // data into to avoid unnecessary extra copy operation
            // TODO: use a faster png library if possible
            for( size_t fileNr = 0; fileNr < filenames.size(); ++fileNr )
            {
                auto const& file = filenames[ fileNr ];

                uint8_t* pngData;
                unsigned int imageWidth, imageHeight;

                lodepng_decode_file(
                    &pngData, &imageWidth, &imageHeight, file.c_str(), LCT_RGBA, 8 );

                auto const imageSize = imageWidth * imageHeight * 4;

                if( !textureData )
                {
                    width = imageWidth;
                    height = imageHeight;
                    textureData = std::make_unique< uint8_t[] >( imageSize * filenames.size() );
                }

                if( width == imageWidth && height == imageHeight )
                {
                    std::copy(
                        pngData, pngData + imageSize, textureData.get() + fileNr * imageSize );
                    free( pngData );
                }
                else
                {
                    free( pngData );
                    throw std::runtime_error( "Invalid PNG Array. Width and height don't match." );
                }
            }

            std::unique_ptr< Texture3D > texture( new Texture3D() );
            {
                auto modifier = texture->modify();

                modifier->upload( width,
                                  height,
                                  filenames.size(),
                                  GL_RGBA8,
                                  GL_RGBA,
                                  GL_UNSIGNED_BYTE,
                                  textureData.get() );
                modifier->minMagFilter( GL_LINEAR, GL_LINEAR );
                modifier->wrap( GL_REPEAT, GL_REPEAT, GL_REPEAT );
            }

            return texture;
        }
    }
}
