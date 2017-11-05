#include "NetSelector.hpp"

#include "Selectable.hpp"
#include <ttb/net/Selectable.hpp>
#include <ttb/net/SelectableHolder.hpp>
#include <ttb/net/events.hpp>

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

        void NetSelector::add( std::shared_ptr< SelectableHolder > const& selectable )
        {
            if( selectable )
            {
                if( std::dynamic_pointer_cast< posix::Selectable >( selectable->selectable() ) )
                {
                    std::lock_guard< std::mutex > lock( m_mutex );
                    m_changes.emplace( ChangeType::ADD, selectable );
                }
                else
                {
                    throw std::runtime_error( "Invalid type" );
                }
            }
            else
            {
                throw std::runtime_error( "Null socket" );
            }
        }

        void NetSelector::remove( std::shared_ptr< SelectableHolder > const& selectable )
        {
            if( selectable )
            {
                if( std::dynamic_pointer_cast< posix::Selectable >( selectable->selectable() ) )
                {
                    std::lock_guard< std::mutex > lock( m_mutex );
                    m_changes.emplace( ChangeType::REMOVE, selectable );
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

        void NetSelector::update( bool block )
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
                            auto selectable = std::get< 1 >( change );
                            auto iter = std::find( std::begin( m_selectables ),
                                                   std::end( m_selectables ),
                                                   selectable );

                            if( iter == std::end( m_selectables ) )
                            {
                                m_selectables.push_back( std::move( std::get< 1 >( change ) ) );
                            }
                            break;
                        }

                        case ChangeType::REMOVE:
                        {
                            auto selectable = std::get< 1 >( change );
                            auto iter = std::find( std::begin( m_selectables ),
                                                   std::end( m_selectables ),
                                                   selectable );

                            if( iter != std::end( m_selectables ) )
                            {
                                m_selectables.erase( iter );
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
            for( auto const& selectable : m_selectables )
            {
                auto& sel = dynamic_cast< posix::Selectable const& >( *selectable->selectable() );

                if( sel.isReadable() )
                {
                    FD_SET( sel.handle(), &readSockets );
                    maxFD = std::max( maxFD, sel.handle() );
                }

                if( sel.isWritable() )
                {
                    FD_SET( sel.handle(), &writeSockets );
                    maxFD = std::max( maxFD, sel.handle() );
                }
            }

            timeval timeout = block ? timeval{ 5, 0 } : timeval{ 0, 0 };

            auto result = select( maxFD + 1, &readSockets, &writeSockets, nullptr, &timeout );

            if( result == -1 )
            {
                throw std::runtime_error( "Error during select: " + std::to_string( errno ) );
            }

            if( result > 0 )
            {
                for( auto& selectable : m_selectables )
                {
                    auto& sel = dynamic_cast< posix::Selectable& >( *selectable->selectable() );

                    if( FD_ISSET( sel.handle(), &readSockets ) )
                    {
                        sel.doRead( selectable, *m_eventOutput );
                    }

                    if( FD_ISSET( sel.handle(), &writeSockets ) )
                    {
                        sel.doWrite( selectable, *m_eventOutput );
                    }
                }
            }
        }
    }
}
