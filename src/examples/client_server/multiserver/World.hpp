#pragma once

#include <ttb/net/NetSelector.hpp>

#include <memory>
#include <vector>


class Client;


class World
{
public:
    World();

    ~World();

    void addClient( std::shared_ptr< Client > client );
    void removeClient( Client const& client );

    void update( float time, float timeDiff );

private:
    std::unique_ptr< ttb::NetSelector > m_selector;
    std::vector< std::shared_ptr< Client > > m_clients;
};
