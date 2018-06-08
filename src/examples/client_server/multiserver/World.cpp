#include "World.hpp"

#include <Client.hpp>


World::World() : m_selector( ttb::NetSelector::create() )
{
}

World::~World()
{
}

void World::addClient( std::shared_ptr< Client > client )
{
    m_selector->add( client->socket() );
    m_clients.push_back( std::move( client ) );
}

void World::removeClient( Client const& client )
{
    auto iter = std::find_if( std::begin( m_clients ), std::end( m_clients ), [&]( auto const& v ) {
        return v.get() == &client;
    } );

    if( iter != std::end( m_clients ) )
    {
        m_selector->remove( *client.socket() );
        m_clients.erase( iter );
    }
}

void World::update( float time, float timeDiff )
{
    m_selector->update();
}
