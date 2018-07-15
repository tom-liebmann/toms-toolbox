#include "NetSelector.hpp"

#include "Selectable.hpp"
#include <ttb/net/Selectable.hpp>
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
            m_interruptor = Interruptor::create();
            m_selectables.push_back(
                std::static_pointer_cast< ttb::posix::Interruptor >( m_interruptor ) );
        }

        void NetSelector::interrupt()
        {
            m_interruptor->interrupt();
        }

        void NetSelector::add( std::shared_ptr< ttb::Selectable > const& selectable )
        {
            auto sel = std::dynamic_pointer_cast< posix::Selectable >( selectable );

            if( sel )
            {
                std::lock_guard< std::mutex > lock( m_mutex );

                m_changes.emplace( sel );

                m_interruptor->interrupt();
            }
            else
            {
                throw std::runtime_error( "Invalid selectable" );
            }
        }

        void NetSelector::remove( ttb::Selectable& selectable )
        {
            auto sel = dynamic_cast< posix::Selectable* >( &selectable );

            if( sel )
            {
                std::lock_guard< std::mutex > lock( m_mutex );

                m_changes.emplace( *sel );

                m_interruptor->interrupt();
            }
            else
            {
                throw std::runtime_error( "Invalid selectable" );
            }
        }

        void NetSelector::update( bool block )
        {
            // process changes
            {
                std::lock_guard< std::mutex > lock( m_mutex );
                while( !m_changes.empty() )
                {
                    auto& change = m_changes.front();

                    switch( change.type() )
                    {
                        case Change::Type::ADD:
                        {
                            auto selectable = change.dataAdd();
                            auto iter = std::find( std::begin( m_selectables ),
                                                   std::end( m_selectables ),
                                                   selectable );

                            if( iter == std::end( m_selectables ) )
                            {
                                m_selectables.push_back( selectable );
                            }
                            break;
                        }

                        case Change::Type::REMOVE:
                        {
                            auto const& selectable = change.dataRemove();
                            auto iter = std::find_if(
                                std::begin( m_selectables ),
                                std::end( m_selectables ),
                                [&]( auto const& v ) { return v.get() == &selectable; } );

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

            FD_ZERO( &readSockets );
            FD_ZERO( &writeSockets );

            int maxFD = 0;
            for( auto const& selectable : m_selectables )
            {
                auto handle = selectable->handle();

                if( handle >= 0 )
                {
                    maxFD = std::max( maxFD, handle );

                    if( selectable->isReadable() )
                    {
                        FD_SET( handle, &readSockets );
                    }

                    if( selectable->isWritable() )
                    {
                        FD_SET( handle, &writeSockets );
                    }
                }
            }

            timeval timeout = block ? timeval{ 10, 0 } : timeval{ 0, 0 };

            auto result = select( maxFD + 1, &readSockets, &writeSockets, nullptr, &timeout );

            if( result == -1 )
            {
                throw std::runtime_error( "Error during select: " + std::to_string( errno ) );
            }

            if( result > 0 )
            {
                for( auto& selectable : m_selectables )
                {
                    auto handle = selectable->handle();

                    if( handle >= 0 )
                    {
                        if( FD_ISSET( handle, &readSockets ) )
                        {
                            selectable->doRead();
                        }

                        if( FD_ISSET( handle, &writeSockets ) )
                        {
                            selectable->doWrite();
                        }
                    }
                }
            }
        }


        NetSelector::Change::Change( std::shared_ptr< ttb::posix::Selectable > data )
            : m_type( Type::ADD )
        {
            new( &m_data.add ) std::shared_ptr< ttb::posix::Selectable >( std::move( data ) );
        }

        NetSelector::Change::Change( Selectable const& remove ) : m_type( Type::REMOVE )
        {
            m_data.remove = &remove;
        }

        NetSelector::Change::~Change()
        {
            switch( m_type )
            {
                case Type::ADD:
                {
                    m_data.add.~shared_ptr< ttb::posix::Selectable >();
                    break;
                }

                default:
                    break;
            }
        }

        NetSelector::Change::Type NetSelector::Change::type() const
        {
            return m_type;
        }

        std::shared_ptr< ttb::posix::Selectable > const& NetSelector::Change::dataAdd()
        {
            return m_data.add;
        }

        Selectable const& NetSelector::Change::dataRemove()
        {
            return *m_data.remove;
        }


        NetSelector::Change::Data::Data()
        {
        }

        NetSelector::Change::Data::~Data()
        {
        }
    }
}
