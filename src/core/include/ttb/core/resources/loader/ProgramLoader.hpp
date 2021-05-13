#pragma once

#include <ttb/core/resources/DatabaseLoader.hpp>
#include <ttb/core/shader.hpp>


namespace ttb::resources
{
    class ProgramLoader : public DatabaseLoader< ttb::Program >
    {
    public:
        ProgramLoader( std::string const& filename );

        // Override: DatabaseLoader
        virtual std::shared_ptr< ttb::Program > load( YAML::Node const& node ) const override;
    };
}
