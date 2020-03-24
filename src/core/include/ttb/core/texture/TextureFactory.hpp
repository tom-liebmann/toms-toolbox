#pragma once

#include <ttb/core/texture/Texture2D.hpp>
#include <ttb/core/texture/Texture3D.hpp>

#include <memory>
#include <vector>


namespace ttb
{
    namespace TextureFactory
    {
        std::shared_ptr< Texture2D > loadPNG( std::string const& filename );

        void savePNG( std::shared_ptr< Texture2D > texture, std::string const& filename );

#ifndef MODE_ANDROID
        std::unique_ptr< Texture3D > loadPNGArray( std::vector< std::string > const& filenames );
#endif
    }
}
