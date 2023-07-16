#pragma once

#include <cstdint>
#include <string>
#include <vector>


namespace ttb::resources
{
    class AssetManager
    {
    public:
        static AssetManager& instance();

        std::string loadContent( std::string const& filename );

        std::vector< uint8_t > loadData( std::string const& filename );

        std::string const& appDir() const;

    private:
        AssetManager();
    };
}
