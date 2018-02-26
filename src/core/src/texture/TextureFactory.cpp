#include <iostream>
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

        std::unique_ptr< Texture3D > loadPNGArray( std::vector<std::string> const& filenames )
        {          
            uint8_t* texData;
            unsigned int width, height, depth;
            
            // copy image data into continuous memory area
            // TODO: get png decode to accept an already allocated memory region to fill the image data into
            //       to avoid unnecessary extra copy operation
            // TODO: use a faster png library if possible
            for( unsigned i=0; i < filenames.size(); i++ ) {
              const auto& file = filenames[i];
              uint8_t* pngData;
              unsigned int pngSize;
              unsigned int fw, fh;
              lodepng_decode_file( &pngData, &fw, &fh, file.c_str(), LCT_RGBA, 8 );
              // quick n dirty init
              if( i==0 ) {
                width = fw;              
                height = fh;
                depth = filenames.size();
                pngSize = width * height * 4;
                texData = (uint8_t*) malloc(pngSize * depth);
              }
              
              if( width == fw && height == fh ) {
                memcpy(texData + (i * pngSize), pngData, pngSize);
                free( pngData );  
              } else {
                free( pngData );
                free( texData );
                throw std::runtime_error( "Invalid PNG Array. Width and height don't match." );
              }
            }
            
            std::unique_ptr< Texture3D > texture( new Texture3D() );
            {
                auto modifier = texture->modify();

                modifier->upload( width, height, depth, GL_RGBA8, GL_RGBA, GL_UNSIGNED_BYTE, texData );
                modifier->minMagFilter( GL_LINEAR, GL_LINEAR );
                modifier->wrap(  GL_REPEAT,  GL_REPEAT,  GL_REPEAT );
            }            
            free( texData );
            return texture;
        }
    }
}
