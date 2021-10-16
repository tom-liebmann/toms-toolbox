#pragma once

#include <ttb/core/texture/Texture2D.hpp>
// #include <ttb/core/texture/Texture3D.hpp>

#include <memory>
#include <vector>


namespace ttb
{
    namespace TextureFactory
    {
        std::shared_ptr< Texture2D > loadData( void const* data, size_t dataSize );

        std::shared_ptr< Texture2D > loadPNG( std::string const& filename, uint8_t levels = 1 );

        void savePNG( std::shared_ptr< Texture2D > texture, std::string const& filename );

        // std::unique_ptr< Texture3D > loadPNGArray( std::vector< std::string > const& filenames );
    }
}
