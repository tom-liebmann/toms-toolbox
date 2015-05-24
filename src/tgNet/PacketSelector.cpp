#include "PacketSelector.hpp"

#include <tgNet/IPacket.hpp>
#include <tgNet/SocketContainer.hpp>
#include <tgNet/TCPSocket.hpp>

#include <iostream>
#include <algorithm>

using tgNet::PacketSelector;

#ifdef WIN32

#else

    #include <sys/socket.h>
    #include <sys/types.h>
    #include <fcntl.h>
    #include <netdb.h>
    #include <unistd.h>
    #include <stdio.h>

#endif

PacketSelector::PacketSelector()
{
    #ifdef WIN32

        m_notifyEvent = WSACreateEvent();
        m_events = std::vector< WSAEVENT >( { m_notifyEvent } );

    #else

        m_epoll = epoll_create1( 0 );
        if( m_epoll == -1 )
            throw std::runtime_error( "Unable to initialize epoll." );

        // init interruption pipe
        epoll_event event;
        event.events = EPOLLIN;
        event.data.fd = -1;
        pipe( m_pipe );
        if( epoll_ctl( m_epoll, EPOLL_CTL_ADD, m_pipe[ 0 ], &event ) == -1 )
            throw std::runtime_error( "Unable to initialize interruption pipe." );

    #endif
}

PacketSelector::~PacketSelector()
{
    #ifdef WIN32

        for( auto& event : m_eventHandles ) 
            WSACloseEvent( event );

        WSACloseEvent( m_notifyEvent );

    #else

        close( m_pipe[ 0 ] );
        close( m_pipe[ 1 ] );

    #endif
}

std::unique_ptr< PacketSelector::Event > PacketSelector::wait()
{
    m_interrupted = false;

    while( true )
    {
        processChanges();

        // check for already existing events
        if( !m_eventSockets.empty() )
        {
            auto& container = m_managedSockets[ m_eventSockets.front() ];

            // pop first event
            std::unique_ptr< Event > event = std::move( container->m_events.front() );
            container->m_events.pop_front();

            // if container does not have any events left, remove it from eventQueue
            if( container->m_events.empty() )
                m_eventSockets.pop_front();

            // if socket disconnected, remove it from the managed ones
            if( event->getType() == Event::Type::DISCONNECT )
                removeManagedSocket( container );

            return event;
        }

        // if no events are present, wait for new ones
        bool interrupted;
        pollEvents( interrupted );

        if( interrupted && m_interrupted )
            return std::unique_ptr< Event >();
    }
}

void PacketSelector::addSocket( std::shared_ptr< SocketContainer > container )
{
    // set flags of socket to non-blocking
    #ifdef WIN32
    #else

        int flags = fcntl( container->getSocket()->getHandle(), F_GETFL, 0 );
        flags |= O_NONBLOCK;
        fcntl( container->getSocket()->getHandle(), F_SETFL, flags );

    #endif

    // push socket to change list
    std::unique_lock< std::mutex > lock( m_changeMutex );
    m_changeList.push_back( std::make_tuple( 0, std::move( container ) ) );

    wakeUp();
}

void PacketSelector::removeSocket( const std::shared_ptr< SocketContainer >& container )
{
    // push socket to change list
    std::unique_lock< std::mutex > lock( m_changeMutex );
    m_changeList.push_back( std::make_tuple( 1, std::move( container ) ) );

    wakeUp();
}

void PacketSelector::interrupt()
{
    std::unique_lock< std::mutex > lock( m_changeMutex );
    m_interrupted = true;

    wakeUp();
}

void PacketSelector::wakeUp()
{
    // interrupt event polling
    #ifdef WIN32

        WSASetEvent( m_notifyEvent );

    #else

        char buff = ' ';
        write( m_pipe[ 1 ], &buff, 1 );

    #endif
}

void PacketSelector::processChanges()
{
    std::unique_lock< std::mutex > lock( m_changeMutex );
    // apply changes to managed socket list
    if( !m_changeList.empty() )
    {
        do
        {
            const auto& change = m_changeList.front();

            lock.unlock();

            const auto& type = std::get< 0 >( change );
            const auto& container = std::get< 1 >( change );

            switch( type )
            {
                case 0: // INSERTION
                    insertManagedSocket( std::move( container ) );
                    break;

                case 1: // DELETION
                    removeManagedSocket( container );
                    break;
            }

            lock.lock();

            m_changeList.pop_front();
        } while( !m_changeList.empty() );

        #ifdef WIN32

            m_events.resize( m_managedSockets.size() - m_freeSlots.size() + 1 );
            m_eventIds.resize( m_events.size() );

            size_t k = 0;
            for( size_t i = 0; i < m_events.size() - 1; ++i )
            {
                while( !m_managedSockets[ k ] )
                    ++k;

                m_events[ i ] = m_eventHandles[ k ];
                m_eventIds[ i ] = k;

                ++k;
            }

            m_events[ m_events.size() - 1 ] = m_notifyEvent;

        #endif
    }
    lock.unlock();
}

void PacketSelector::insertManagedSocket( std::shared_ptr< SocketContainer > container )
{
    size_t index;
    if( m_freeSlots.empty() )
    {
        index = m_managedSockets.size();
        m_managedSockets.resize( index + 1 );
    }
    else
    {
        index = m_freeSlots.front();
        m_freeSlots.pop_front();
    }

    if( !container->m_events.empty() )
        m_eventSockets.push_back( index );

    #ifdef WIN32

        if( m_eventHandles.size() < index + 1 )
            m_eventHandles.resize( index + 1 );

        m_eventHandles[ index ] = WSACreateEvent();
        WSAEventSelect( container->getSocket()->getHandle(), m_eventHandles[ index ], FD_READ | FD_CLOSE );

    #else

        struct epoll_event event;
        event.events = EPOLLIN;
        event.data.fd = index;
        epoll_ctl( m_epoll, EPOLL_CTL_ADD, container->getSocket()->getHandle(), &event );

    #endif

    m_managedSockets[ index ] = std::move( container );
}

void PacketSelector::removeManagedSocket( const std::shared_ptr< SocketContainer >& container )
{
    // check, whether socket is in the managed list
    size_t index = std::distance(
        m_managedSockets.begin(),
        std::find(
            m_managedSockets.begin(),
            m_managedSockets.end(),
            container ) );

    if( index != m_managedSockets.size() )
    {
        if( !container->m_events.empty() )
            m_eventSockets.remove( index );

        #ifdef WIN32

            WSACloseEvent( m_eventHandles[ index ] );

        #else

            epoll_ctl( m_epoll, EPOLL_CTL_DEL, container->getSocket()->getHandle(), NULL );

        #endif

        m_managedSockets[ index ].reset();
        m_freeSlots.push_back( index );
    }
}

void PacketSelector::pollEvents( bool& interrupted )
{
    #ifdef WIN32

        DWORD res = WSAWaitForMultipleEvents( m_events.size(), m_events.data(), FALSE, WSA_INFINITE, FALSE );

        if( res == WSA_WAIT_FAILED )
        {
            // woke up but nothing happened
        }
        else if( res - WSA_WAIT_EVENT_0 == m_eventIds.size() )
        {
            WSAResetEvent( m_notifyEvent );
            interrupted = true;
        }
        else
        {
            auto event = m_events[ res - WSA_WAIT_EVENT_0 ];
            size_t index = m_eventIds[ res - WSA_WAIT_EVENT_0 ];

            WSAResetEvent( event );
            WSAEventSelect( m_managedSockets[ index ]->getSocket()->getHandle(), event, FD_READ | FD_CLOSE );

            handleEvent( index );
        }

    #else

        int numEvents = epoll_wait( m_epoll, m_events.data(), 64, -1 );

        if( numEvents == -1 )
        {
            if( errno != EINTR )
                throw std::runtime_error( "An epoll-error occured." );
        }
        else
        {
            interrupted = false;

            for( size_t i = 0; i < numEvents; ++i )
            {
                if( m_events[ i ].data.fd == -1 ) // event on the pipe happened
                {
                    // empty the pipe
                    // do I really need locking here?
                    std::unique_lock< std::mutex > lock( m_changeMutex );
                    char buff;
                    read( m_pipe[ 0 ], &buff, 1 );

                    interrupted = true;
                }
                else
                {
                    // handle event for socket with appropriate index
                    handleEvent( m_events[ i ].data.fd  );
                }
            }
        }

    #endif
}

void PacketSelector::handleEvent( size_t index )
{
    auto& container = m_managedSockets[ index ];

    try
    {
        auto packet = IPacket::fromTCPSocket( *container->getSocket() );

        if( container->m_events.empty() )
            m_eventSockets.push_back( index );

        container->m_events.push_back( std::unique_ptr< Event >(
            new PacketEvent(
                container,
                std::move( packet ) ) ) );
    }
    catch( TCPSocket::Error& e )
    {
        switch( e )
        {
            case TCPSocket::Error::CLOSED:
            {
                if( container->m_events.empty() )
                    m_eventSockets.push_back( index );

                container->m_events.push_back( std::unique_ptr< Event >(
                    new DisconnectEvent(
                        container,
                        DisconnectEvent::Reason::NORMAL ) ) );
            } break;

            case TCPSocket::Error::BROKEN:
            {
                if( container->m_events.empty() )
                    m_eventSockets.push_back( index );

                container->m_events.push_back( std::unique_ptr< Event >(
                    new DisconnectEvent(
                        container,
                        DisconnectEvent::Reason::BROKEN ) ) );
            } break;

            default:
                break;
        }
    }
}
