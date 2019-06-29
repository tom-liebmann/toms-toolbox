#pragma once

#include <memory>
#include <unordered_map>


namespace ttb
{
    namespace resource
    {
        template < typename TType >
        class Loader;


        template < typename TType >
        class Manager
        {
        public:
            static Manager& instance();

            void loader( std::unique_ptr< Loader< TType > > loader );

            std::shared_ptr< TType const > get( std::string const& name );

        private:
            std::unique_ptr< Loader< TType > > m_loader;
            std::unordered_map< std::string, std::shared_ptr< TType const > > m_resources;
        };
    }
}


namespace ttb
{
    namespace resource
    {
        template < typename TType >
        Manager< TType >& Manager< TType >::instance()
        {
            static Manager< TType > s_instance;
            return s_instance;
        }

        template < typename TType >
        void Manager< TType >::loader( std::unique_ptr< Loader< TType > > loader )
        {
            m_loader = std::move( loader );
        }

        template < typename TType >
        std::shared_ptr< TType const > Manager< TType >::get( std::string const& name )
        {
            auto iter = m_resources.find( name );

            if( iter == std::end( m_resources ) )
            {
                auto resource = m_loader->load( name );
                m_resources[ name ] = resource;
                return resource;
            }
            else
            {
                return iter->second;
            }
        }
    }
}
