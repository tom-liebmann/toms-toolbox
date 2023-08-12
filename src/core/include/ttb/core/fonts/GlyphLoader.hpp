#pragma once

#include <yaml-cpp/yaml.h>


namespace ttb
{
    class Glyph;
}


namespace ttb
{
    class GlyphLoader
    {
    public:
        GlyphLoader() = delete;

        static Glyph loadFromYaml( YAML::Node const& node );
    };
}
