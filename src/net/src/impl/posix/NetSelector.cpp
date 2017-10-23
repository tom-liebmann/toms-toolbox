#include "NetSelector.hpp"

#include "Selectable.hpp"
#include <ttb/net/Selectable.hpp>
#include <ttb/net/netEvents.hpp>

#include <arpa/inet.h>
#include <assert.h>
#include <errno.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include <algorithm>
#include <iostream>

namespace ttb
{
    std::unique_ptr< NetSelector > NetSelector::create()
    {
        return std::make_unique< posix::NetSelector >();
    }


    namespace posix
    {
        NetSelector::NetSelector()
        {
            m_eventOutput = std::make_shared< SimpleProvider< SlotType::ACTIVE, Event& > >();
        }

        NetSelector::~NetSelector()
        {
        }

        void NetSelector::add( std::shared_ptr< ttb::Selectable > const& socket )
        {
            if( socket )
            {
                if( auto sck = std::dynamic_pointer_cast< posix::Selectable >( socket ) )
                {
                    std::lock_guard< std::mutex > lock( m_mutex );

                    m_changes.emplace( ChangeType::ADD, std::move( sck ) );
                }
                else
                {
                    throw std::runtime_error( "Invalid socket type" );
                }
            }
            else
            {
                throw std::runtime_error( "Null socket" );
            }
        }

        void NetSelector::remove( std::shared_ptr< ttb::Selectable > const& socket )
        {
            if( socket )
            {
                if( auto sck = std::dynamic_pointer_cast< posix::Selectable >( socket ) )
                {
                    std::lock_guard< std::mutex > lock( m_mutex );

                    m_changes.emplace( ChangeType::REMOVE, std::move( sck ) );
                }
                else
                {
                    throw std::runtime_error( "Invalid socket type" );
                }
            }
            else
            {
                throw std::runtime_error( "Null socket" );
            }
        }

        std::shared_ptr< Provider< SlotType::ACTIVE, Event& > > NetSelector::eventOutput() const
        {
            return m_eventOutput;
        }

        void NetSelector::update()
        {
            // process changes
            {
                std::lock_guard< std::mutex > lock( m_mutex );
                while( !m_changes.empty() )
                {
                    auto& change = m_changes.front();

                    switch( std::get< 0 >( change ) )
                    {
                        case ChangeType::ADD:
                        {
                            auto socket = std::get< 1 >( change );
                            auto iter =
                                std::find( std::begin( m_sockets ), std::end( m_sockets ), socket );

                            if( iter == std::end( m_sockets ) )
                            {
                                m_sockets.push_back( std::move( std::get< 1 >( change ) ) );
                            }
                            break;
                        }

                        case ChangeType::REMOVE:
                        {
                            auto socket = std::get< 1 >( change );
                            auto iter =
                                std::find( std::begin( m_sockets ), std::end( m_sockets ), socket );

                            if( iter != std::end( m_sockets ) )
                            {
                                m_sockets.erase( iter );
                            }
                            break;
                        }
                    }

                    m_changes.pop();
                }
            }

            fd_set readSockets;
            fd_set writeSockets;
            fd_set exceptSockets;

            FD_ZERO( &readSockets );
            FD_ZERO( &writeSockets );

            int maxFD = 0;
            for( auto const& socket : m_sockets )
            {
                if( socket->isReadable() )
                {
                    FD_SET( socket->handle(), &readSockets );
                    maxFD = std::max( maxFD, socket->handle() );
                }

                if( socket->isWritable() )
                {
                    FD_SET( socket->handle(), &writeSockets );
                    maxFD = std::max( maxFD, socket->handle() );
                }
            }

            timeval timeout{ 0, 0 };

            auto result = select( maxFD + 1, &readSockets, &writeSockets, nullptr, &timeout );

            if( result == -1 )
            {
                throw std::runtime_error( "Error during select: " + std::to_string( errno ) );
            }

            if( result > 0 )
            {
                for( auto& socket : m_sockets )
                {
                    if( FD_ISSET( socket->handle(), &readSockets ) )
                    {
                        socket->doRead( *m_eventOutput );
                    }

                    if( FD_ISSET( socket->handle(), &writeSockets ) )
                    {
                        socket->doWrite( *m_eventOutput );
                    }
                }
            }
        }
    }
}
