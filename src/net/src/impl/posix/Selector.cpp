#include "Selector.hpp"

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


namespace
{
    constexpr size_t INTERRUPT_SLOT = std::numeric_limits< size_t >::max();
}


namespace ttb
{
    std::unique_ptr< Selector > Selector::create()
    {
        return std::make_unique< posix::Selector >();
    }


    namespace posix
    {
        Selector::Selector()
            : m_pollDescriptor( epoll_create1( 0 ) ), m_writePending( false ), m_running( true )
        {
            if( m_pollDescriptor == -1 )
            {
                throw std::runtime_error( "Unable to create epoll descriptor (" +
                                          std::to_string( errno ) + ")" );
            }

            pipe( m_interruptHandles.get() );

            struct epoll_event event;
            event.events = EPOLLIN | EPOLLOUT | EPOLLET;
            event.data.u64 = INTERRUPT_SLOT;
            if( epoll_ctl( m_pollDescriptor, EPOLL_CTL_ADD, m_interruptHandes[ 0 ], &event ) == -1 )
            {
                throw std::runtime_error( "Unable to create interruption pipe" );
            }
        }

        Selector::~Selector()
        {
            close( m_interruptHandles[ 0 ] );
            close( m_interruptHandles[ 1 ] );
            close( m_pollDescriptor );
        }

        void Selector::notifyWrite( posix::Selectable& selectable )
        {
            {
                std::lock_guard< std::mutex > lock( m_writeLock );
                m_writables[ m_currentWriteSet ].insert( &selectable );
            }
            m_writeCondition.notify_all();
        }

        void Selector::notifyRead( posix::Selectable& selectable )
        {
            {
                std::lock_guard< std::mutex > lock( m_changeLock );
                m_changesSelectables.push_back( &selectable );
            }
            interrupt();
        }

        void Selector::startPolling( posix::Selectable& selectable,
                                     std::unique_lock< std::mutex >& selectableLock )
        {
            struct epoll_event event;
            event.events = EPOLLIN | EPOLLOUT | EPOLLET;
            event.data.u64 = selectable.m_slot;
            if( epoll_ctl( m_pollDescriptor, EPOLL_CTL_ADD, selectable.m_handle, &event ) == -1 )
            {
                throw std::runtime_error( "Unable to add socket to polling (" +
                                          std::to_string( errno ) + ")" );
            }
        }

        void Selector::stopPolling( posix::Selectable& selectable,
                                    std::unique_lock< std::mutex >& selectableLock )
        {
            if( epoll_ctl( m_pollDescriptor, EPOLL_CTL_DEL, selectable.m_handle, nullptr ) == -1 )
            {
                throw std::runtime_error( "Unable to add socket to polling (" +
                                          std::to_string( errno ) + ")" );
            }
        }

        void Selector::add( std::shared_ptr< ttb::Selectable > const& selectable )
        {
            auto sel = std::dynamic_pointer_cast< posix::Selectable >( selectable );

            if( sel )
            {
                std::unique_lock< std::mutex > selectableLock( sel->m_mutex );

                if( sel->m_selector != this )
                {
                    if( sel->m_selector )
                    {
                        sel->m_selector->remove( sel, selectableLock );
                    }

                    std::unique_lock< std::mutex > changelock( m_changeMutex );

                    if( m_freeSlots.empty() )
                    {
                        sel->m_slot = m_selectables.size();
                        m_selectables.push_back( sel );
                    }
                    else
                    {
                        sel->m_slot = m_freeSlots.back();
                        m_freeSlots.pop_back();
                        m_selectables[ sel->m_slot ] = sel;
                    }

                    sel->m_selector = this;

                    notifyWrite( *sel );
                }
            }
            else
            {
                throw std::runtime_error( "Invalid selectable" );
            }
        }

        void Selector::remove( ttb::posix::Selectable& selectable,
                               std::unique_lock< std::mutex >& selectableLock )
        {
            std::unique_lock< std::mutex > changelock( m_changeMutex );

            selectable.m_selector = nullptr;
            m_selectables[ selectable.m_slot ].reset();
            m_newFreeSlots.push_back( selectable.m_slot );

            std::unique_lock< std::mutex > writeLock( m_writeMutex );
            m_writables[ m_currentWriteSet ].erase( &selectable );
        }

        void Selector::remove( ttb::Selectable& selectable )
        {
            auto sel = dynamic_cast< posix::Selectable* >( &selectable );

            if( sel )
            {
                std::unique_lock< std::mutex > selectableLock( sel->m_mutex );

                if( sel->m_selector == this )
                {
                    remove( *sel, selectableLock );
                }
            }
            else
            {
                throw std::runtime_error( "Invalid selectable" );
            }
        }

        void Selector::processWrites( bool blocking )
        {
            std::unique_lock< std::mutex > lock( m_writeLock );

            if( !m_writables[ m_currentWriteSet ].empty() )
            {
                m_currentWriteSet = 1 - m_currentWriteSet;

                auto& writables = m_writables[ 1 - m_currentWriteSet ];

                lock.unlock();

                for( auto selectable : writables )
                {
                    if( !selectable->writeData() )
                    {
                        lock.lock();

                        m_writables[ m_currentWriteSet ].insert( selectable );

                        lock.unlock();
                    }
                }

                writables.clear();
            }
            else
            {
                if( blocking )
                {
                    m_writeCondition.wait( lock );
                }
            }
        }

        void Selector::interrupt()
        {
            static uint8_t message = 1;
            write( m_interruptHandles[ 1 ], &message, sizeof( message ) );
        }

        void Selector::processReads( bool blocking )
        {
            // While we were polling, some selectables could have been removed leaving free slots in
            // the selectable vector. Before this point, we were unable to reuse the ids as the
            // event polling could contain an old event pointing to the empty slot.
            std::unique_lock< std::mutex > changeLock( m_changeMutex );
            {
                m_freeSlots.insert( std::end( m_freeSlots ),
                                    std::begin( m_newFreeSlots ),
                                    std::end( m_newFreeSlots ) );
                m_newFreeSlots.clear();
            }
            changeLock.unlock();

            int timeout = blocking ? -1 : 0;

            int eventCount =
                epoll_wait( m_pollDescriptor, m_pollEvents, m_pollEvents.size(), timeout );

            if( eventCount == -1 )
            {
                throw std::runtime_error( "Unable to poll events (" + std::to_string( errno ) +
                                          ")" );
            }
            else
            {
                for( int eventNr = 0; eventNr < eventCount; ++eventNr )
                {
                    auto const& event = m_pollEvents[ eventNr ];
                    if( event.data.u64 == INTERRUPT_SLOT )
                    {
                        // The selector was interrupted through the self-pipe trick
                        static uint8_t message;
                        read( m_handles[ 0 ], &message, sizeof( message ) );
                    }
                    else
                    {
                        changeLock.lock();
                        auto selectable = m_selectables[ event.data.u64 ];

                        if( selectable )
                        {
                            std::unique_lock< std::mutex > selectableLock( selectable->m_mutex );
                            changeLock.unlock();

                            selectable->processRead( selectableLock );
                        }
                    }
                }
            }
        }
    }
}
