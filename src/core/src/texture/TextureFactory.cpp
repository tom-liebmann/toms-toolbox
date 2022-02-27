#include <iostream>
#include <ttb/core/PNGLoader.hpp>
#include <ttb/core/texture/TextureFactory.hpp>

#include <ttb/core/gl.hpp>


namespace
{
    std::shared_ptr< ttb::Texture2D > createTextureFromData( size_t width,
                                                             size_t height,
                                                             GLenum internalFormat,
                                                             uint8_t levels,
                                                             GLenum dataFormat,
                                                             GLenum dataType,
                                                             void const* data );
}

namespace ttb
{
    namespace TextureFactory
    {
        std::shared_ptr< Texture2D > loadData( void const* data, size_t dataSize, uint8_t levels )
        {
            uint8_t* pngData;
            unsigned int width, height;
            lodepng_decode_memory( &pngData,
                                   &width,
                                   &height,
                                   static_cast< unsigned char const* >( data ),
                                   dataSize,
                                   LCT_RGBA,
                                   8 );


#if defined( PLATFORM_ANDROID )
            // TODO: Older android versions do not support GL_RGB8
            auto texture = createTextureFromData(
                width, height, GL_RGBA, levels, GL_RGBA, GL_UNSIGNED_BYTE, pngData );
#else
            auto texture = createTextureFromData(
                width, height, GL_RGBA8, levels, GL_RGBA, GL_UNSIGNED_BYTE, pngData );
#endif

            free( pngData );

            return texture;
        }

#if !defined( PLATFORM_ANDROID )
        std::shared_ptr< Texture2D > loadPNG( std::string const& filename, uint8_t levels )
        {
            uint8_t* pngData;
            unsigned int width, height;
            lodepng_decode_file( &pngData, &width, &height, filename.c_str(), LCT_RGBA, 8 );

            auto texture = createTextureFromData(
                width, height, GL_RGBA8, levels, GL_RGBA, GL_UNSIGNED_BYTE, pngData );

            free( pngData );

            return texture;
        }
#endif

        // std::unique_ptr< Texture3D > loadPNGArray( std::vector< std::string > const& filenames )
        // {
        //     unsigned int width, height;

        //     std::unique_ptr< uint8_t[] > textureData;

        //     // copy image data into continuous memory area
        //     // TODO: get png decode to accept an already allocated memory region to fill the
        //     image
        //     // data into to avoid unnecessary extra copy operation
        //     // TODO: use a faster png library if possible
        //     for( size_t fileNr = 0; fileNr < filenames.size(); ++fileNr )
        //     {
        //         auto const& file = filenames[ fileNr ];

        //         uint8_t* pngData;
        //         unsigned int imageWidth, imageHeight;

        //         lodepng_decode_file(
        //             &pngData, &imageWidth, &imageHeight, file.c_str(), LCT_RGBA, 8 );

        //         auto const imageSize = imageWidth * imageHeight * 4;

        //         if( !textureData )
        //         {
        //             width = imageWidth;
        //             height = imageHeight;
        //             textureData = std::make_unique< uint8_t[] >( imageSize * filenames.size() );
        //         }

        //         if( width == imageWidth && height == imageHeight )
        //         {
        //             std::copy(
        //                 pngData, pngData + imageSize, textureData.get() + fileNr * imageSize );
        //             free( pngData );
        //         }
        //         else
        //         {
        //             free( pngData );
        //             throw std::runtime_error( "Invalid PNG Array. Width and height don't match."
        //             );
        //         }
        //     }

        //     std::unique_ptr< Texture3D > texture( new Texture3D() );
        //     {
        //         auto modifier = texture->modify();

        //         modifier->upload( width,
        //                           height,
        //                           filenames.size(),
        //                           GL_RGBA8,
        //                           GL_RGBA,
        //                           GL_UNSIGNED_BYTE,
        //                           textureData.get() );
        //         modifier->minMagFilter( GL_LINEAR, GL_LINEAR );
        //         modifier->wrap( GL_REPEAT, GL_REPEAT, GL_REPEAT );
        //     }

        //     return texture;
        // }

#if !defined( PLATFORM_ANDROID )
        void savePNG( std::shared_ptr< Texture2D > texture, std::string const& filename )
        {
            std::vector< uint8_t > data;

            modify( texture ).download( 0, data ).finish();

            LodePNGColorType type = []( uint8_t channel )
            {
                switch( channel )
                {
                    case 3:
                        return LCT_RGB;

                    case 4:
                        return LCT_RGBA;

                    default:
                        throw std::runtime_error( "Unknown pixel format" );
                }
            }( texture->colorChannel() );

            // TODO: Find out why lodepng crashes with "double free" error when textures are in RGB
            // rather than RGBA format

            // TODO: Proparly compute bit depth
            lodepng_encode_file(
                filename.c_str(), data.data(), texture->width(), texture->height(), type, 4 * 8 );
        }
#endif
    }
}


namespace
{
    std::shared_ptr< ttb::Texture2D > createTextureFromData( size_t width,
                                                             size_t height,
                                                             GLenum internalFormat,
                                                             uint8_t levels,
                                                             GLenum dataFormat,
                                                             GLenum dataType,
                                                             void const* data )
    {
        auto textureMod = ttb::Texture2D::create( width, height, internalFormat, levels, 1 );

        textureMod.upload( dataFormat, dataType, data );

        if( levels > 1 )
        {
            textureMod.minMagFilter( GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR );
            textureMod.generateMipMap();
        }
        else
        {
            textureMod.minMagFilter( GL_LINEAR, GL_LINEAR );
        }

        return textureMod.finish();
    }
}
