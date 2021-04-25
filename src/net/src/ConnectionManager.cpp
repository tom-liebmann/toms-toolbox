#include <ttb/net/ConnectionManager.hpp>

#include "ContextThread.hpp"
#include <ttb/net/Connection.hpp>


namespace ttb::net
{
    ConnectionManager::ConnectionManager() : m_contextThread{ std::make_shared< ContextThread >() }
    {
    }

    ConnectionManager::~ConnectionManager() = default;

    void ConnectionManager::add( Connection& connection )
    {
        connection.context( m_contextThread );
    }
}
