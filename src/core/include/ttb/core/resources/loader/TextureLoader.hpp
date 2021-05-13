#pragma once

#include <ttb/core/resources/DatabaseLoader.hpp>
#include <ttb/core/texture.hpp>


namespace ttb::resources
{
    class TextureLoader : public DatabaseLoader< ttb::Texture2D >
    {
    public:
        TextureLoader( std::string const& filename );

        // Override: DatabaseLoader
        virtual std::shared_ptr< ttb::Texture2D > load( YAML::Node const& node ) const override;
    };
}
