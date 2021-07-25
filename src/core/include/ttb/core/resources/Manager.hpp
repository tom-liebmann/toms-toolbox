#pragma once

#include "Container.hpp"
#include "Loader.hpp"

#include <memory>
#include <string>
#include <type_traits>
#include <unordered_map>

namespace std
{
    template <>
    struct hash< std::reference_wrapper< std::type_info const > >
    {
        size_t operator()( std::reference_wrapper< std::type_info const > const& value ) const;
    };

    bool operator==( std::reference_wrapper< std::type_info const > const& lhs,
                     std::reference_wrapper< std::type_info const > const& rhs );
}

namespace ttb::resources
{
    class Manager
    {
    public:
        template < typename TType, typename... TArgs >
        void loader( TArgs&&... args );

        template < typename TType >
        std::shared_ptr< TType > get( std::string const& name );

    private:
        std::unordered_map< std::reference_wrapper< std::type_info const >,
                            std::unique_ptr< ContainerBase > >
            m_container;
    };
}



namespace std
{
    inline size_t hash< std::reference_wrapper< std::type_info const > >::operator()(
        std::reference_wrapper< std::type_info const > const& value ) const
    {
        return value.get().hash_code();
    }

    inline bool operator==( std::reference_wrapper< std::type_info const > const& lhs,
                            std::reference_wrapper< std::type_info const > const& rhs )
    {
        return lhs.get() == rhs.get();
    }
}

namespace ttb::resources
{
    template < typename TType, typename... TArgs >
    void Manager::loader( TArgs&&... args )
    {
        auto loader = std::unique_ptr< TType >{ new TType{ std::forward< TArgs >( args )... } };

        using ResourceType = typename TType::ResourceType;

        if( auto const iter = m_container.find( typeid( ResourceType ) );
            iter == std::end( m_container ) )
        {
            auto container =
                std::unique_ptr< Container< ResourceType > >{ new Container< ResourceType >{} };
            container->loader( std::move( loader ) );
            m_container[ typeid( ResourceType ) ] = std::move( container );
        }
        else
        {
            auto& container = static_cast< Container< ResourceType >& >( *iter->second );
            container.loader( std::move( loader ) );
        }
    }

    template < typename TType >
    std::shared_ptr< TType > Manager::get( std::string const& name )
    {
        auto iter = m_container.find( typeid( TType ) );
        if( iter == std::end( m_container ) )
        {
            throw std::runtime_error( "Missing loader for resource " + name );
        }

        return static_cast< Container< TType >& >( *iter->second ).get( name );
    }
}
