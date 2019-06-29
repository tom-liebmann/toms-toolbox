#pragma once

#include <memory>


namespace ttb
{
    namespace resource
    {
        template < typename TType >
        class Loader
        {
        public:
            virtual ~Loader();

            virtual std::shared_ptr< TType const > load( std::string const& name ) const = 0;
        };
    }
}


namespace ttb
{
    namespace resource
    {
        template < typename TType >
        Loader< TType >::~Loader() = default;
    }
}
