#pragma once

#include <ttb/core/texture/Texture2D.hpp>

#include <memory>

namespace ttb
{
    namespace TextureFactory
    {
        std::unique_ptr< Texture2D > loadPNG( std::string const& filename );
    }
}
