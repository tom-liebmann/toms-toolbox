#pragma once

#include "Loader.hpp"

#include <yaml-cpp/yaml.h>

#if defined( PLATFORM_ANDROID )
#include "AssetManager.hpp"
#elif defined( PLATFORM_LINUX ) || defined( PLATFORM_BROWSER )
#include <fstream>
#endif

#include <iterator>
#include <string>


namespace ttb::resources
{
    template < typename TType >
    class DatabaseLoader : public Loader< TType >
    {
    public:
        DatabaseLoader( std::string const& filename );

        virtual ~DatabaseLoader();

        virtual std::shared_ptr< TType > load( std::string const& name ) const override;

        std::string const& rootPath() const;

    private:
        virtual std::shared_ptr< TType > load( YAML::Node const& node ) const = 0;

        std::string m_rootPath;
        YAML::Node m_database;
    };
}


namespace ttb::resources
{
    template < typename TType >
    DatabaseLoader< TType >::DatabaseLoader( std::string const& filename )
    {
#if defined( PLATFORM_ANDROID )

        auto const src = AssetManager::instance().loadContent( filename );

#elif defined( PLATFORM_LINUX ) || defined( PLATFORM_BROWSER )

        std::ifstream fileStream( filename );
        std::string const src( ( std::istreambuf_iterator< char >( fileStream ) ),
                               std::istreambuf_iterator< char >() );

#endif

        m_database = YAML::Load( src );

        m_rootPath = filename.substr( 0, filename.find_last_of( '/' ) );
    }

    template < typename TType >
    DatabaseLoader< TType >::~DatabaseLoader() = default;

    template < typename TType >
    std::shared_ptr< TType > DatabaseLoader< TType >::load( std::string const& name ) const
    {
        if( auto node = m_database[ name ]; node )
        {
            if( auto const result = load( node ); result )
            {
                return result;
            }
        }

        throw std::runtime_error( "Unable to load entry: " + name );
    }

    template < typename TType >
    std::string const& DatabaseLoader< TType >::rootPath() const
    {
        return m_rootPath;
    }
}
