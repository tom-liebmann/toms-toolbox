#pragma once

#include <memory>

namespace ttb::resources
{
    template < typename TType >
    class Loader
    {
    public:
        using ResourceType = TType;

        virtual ~Loader();

        virtual std::shared_ptr< TType > load( std::string const& name ) const = 0;
    };
}



namespace ttb::resources
{
    template < typename TType >
    inline Loader< TType >::~Loader() = default;
}
