#pragma once

#include <ttb/core/texture/Texture2D.hpp>
#include <ttb/core/texture/Texture3D.hpp>
#include <vector>
#include <memory>

using std::vector;

namespace ttb
{
    namespace TextureFactory
    {
        std::unique_ptr< Texture2D > loadPNG( std::string const& filename );
        
        std::unique_ptr< Texture3D > loadPNGArray( std::vector<std::string> const& filenames );
    }
}
