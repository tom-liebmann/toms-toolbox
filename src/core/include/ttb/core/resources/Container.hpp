#pragma once

#include "Loader.hpp"

#include <memory>
#include <unordered_map>

namespace ttb::resources
{
    class ContainerBase
    {
    public:
        virtual ~ContainerBase();
    };


    template < typename TType >
    class Container : public ContainerBase
    {
        friend class Manager;

    private:
        Container() = default;

        void loader( std::unique_ptr< Loader< TType > > loader );

        std::shared_ptr< TType > get( std::string const& name );

        std::unique_ptr< Loader< TType > > m_loader;
        std::unordered_map< std::string, std::shared_ptr< TType > > m_resources;
    };
}



namespace ttb::resources
{
    template < typename TType >
    void Container< TType >::loader( std::unique_ptr< Loader< TType > > loader )
    {
        m_loader = std::move( loader );
    }

    template < typename TType >
    std::shared_ptr< TType > Container< TType >::get( std::string const& name )
    {
        auto iter = m_resources.find( name );
        if( iter == std::end( m_resources ) )
        {
            auto result = m_loader->load( name );

            if( !result )
            {
                throw std::runtime_error( "Unable to load " + name );
            }

            m_resources[ name ] = result;
            return result;
        }
        else
        {
            return iter->second;
        }
    }
}
