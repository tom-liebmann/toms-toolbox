#pragma once

#include <ttb/core/fonts/Font.hpp>
#include <ttb/core/resources/DatabaseLoader.hpp>


namespace ttb::resources
{
    class FontLoader : public DatabaseLoader< ttb::Font >
    {
    public:
        FontLoader( std::string const& filename );

        // Override: DatabaseLoader
        virtual std::shared_ptr< ttb::Font > load( YAML::Node const& node ) const override;
    };
}
