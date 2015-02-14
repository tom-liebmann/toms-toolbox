#include "PacketSelector.hpp"

#include <tgNet/IPacket.hpp>
#include <tgNet/SocketContainer.hpp>
#include <tgNet/TCPSocket.hpp>

#include <iostream>

using tgNet::PacketSelector;

#ifdef WIN32
    PacketSelector::PacketSelector()
    {
        m_notifyEvent = WSACreateEvent();
        m_events = new WSAEVENT[ 1 ];
        m_events[ 0 ] = m_notifyEvent;
        m_eventSize = 1;
    }

    PacketSelector::~PacketSelector()
    {
        delete[] m_events;

        for( auto& tuple : m_container )
            WSACloseEvent( std::get< 0 >( tuple ) );

        WSACloseEvent( m_notifyEvent );
    }

    void PacketSelector::wait( const std::function< void ( std::unique_ptr< Event > ) >& callback )
    {
        m_running = true;

        while( true )
        {
            DWORD res = WSAWaitForMultipleEvents( m_eventSize, m_events, FALSE, WSA_INFINITE, FALSE );

            std::unique_lock< std::mutex > lock( m_containerMutex );

            if( res == WSA_WAIT_FAILED )
            { }
            else if( res == WSA_WAIT_EVENT_0 + m_eventSize - 1 )
            {
                WSAResetEvent( m_notifyEvent );

                if( !m_running )
                    break;
            }
            else
            {
                auto& tup = m_container[ res - WSA_WAIT_EVENT_0 ];

                auto& handle = std::get< 0 >( tup );
                auto& container = std::get< 1 >( tup );

                try
                {
                    std::unique_ptr< IPacket > packet( IPacket::fromTCPSocket( container->getSocket() ) );

                    WSAResetEvent( handle );
                    WSAEventSelect( container->getSocket()->getHandle(), handle, FD_READ | FD_CLOSE );

                    std::unique_lock< std::mutex > containerLock( container->m_mutex );
                    container->m_events.push_back( std::unique_ptr< Event >(
                        new PacketEvent(
                            container,
                            std::move( packet ) ) ) );
                    m_eventContainer.insert( container );
                }
                catch( TCPSocket::Error& e )
                {
                    m_changeList.push_back( std::make_tuple( 1, container ) );

                    switch( e )
                    {
                        case TCPSocket::Error::CLOSED:
                        {
                            std::unique_lock< std::mutex > containerLock( container->m_mutex );
                            container->m_events.push_back( std::unique_ptr< Event >(
                                new DisconnectEvent(
                                    container,
                                    DisconnectEvent::Reason::NORMAL ) ) );
                            m_eventContainer.insert( container );
                        } break;

                        case TCPSocket::Error::BROKEN:
                        {
                            std::unique_lock< std::mutex > containerLock( container->m_mutex );
                            container->m_events.push_back( std::unique_ptr< Event >(
                                new DisconnectEvent(
                                    container,
                                    DisconnectEvent::Reason::BROKEN ) ) );
                            m_eventContainer.insert( container );
                        } break;

                        default:
                            break;
                    }
                }
            }

            // process new events
            if( callback )
            {
                while( !m_eventContainer.empty() )
                {
                    // get first eventContainer
                    auto& container = *m_eventContainer.begin();

                    // pop first event
                    std::unique_lock< std::mutex > containerLock( container->m_mutex );
                    std::unique_ptr< Event > event = std::move( container->m_events.front() );
                    container->m_events.pop_front();

                    // if container does not have any events left, remove it from eventQueue
                    if( container->m_events.empty() )
                        m_eventContainer.erase( m_eventContainer.begin() );

                    containerLock.unlock();

                    lock.unlock();

                    callback( std::move( event ) );

                    lock.lock();
                }
            }

            if( !m_changeList.empty() )
            {
                while( !m_changeList.empty() )
                {
                    auto& change = m_changeList.front();

                    auto& type = std::get< 0 >( change );
                    auto& container = std::get< 1 >( change );

                    switch( type )
                    {
                        case 0: // INSERTION
                        {
                            WSAEVENT event = WSACreateEvent();
                            WSAEventSelect( container->getSocket()->getHandle(), event, FD_READ | FD_CLOSE );

                            if( !container->m_events.empty() )
                                m_eventContainer.insert( container );

                            m_container.push_back( std::make_tuple( event, std::move( container ) ) );
                        } break;

                        case 1: // DELETION
                        {
                            auto iter2 = m_container.begin();
                            while( iter2 != m_container.end() && std::get< 1 >( *iter2 ) != container )
                                ++iter2;

                            if( iter2 != m_container.end() )
                            {
                                if( !container->m_events.empty() )
                                    m_eventContainer.erase( container );

                                WSACloseEvent( std::get< 0 >( *iter2 ) );

                                m_container.erase( iter2 );
                            }
                        } break;
                    }

                    m_changeList.pop_front();
                }

                delete[] m_events;
                m_eventSize = m_container.size() + 1;
                m_events = new WSAEVENT[ m_eventSize ];

                for( int i = 0; i < m_container.size(); ++i )
                    m_events[ i ] = std::get< 0 >( m_container[ i ] );

                m_events[ m_eventSize - 1 ] = m_notifyEvent;
            }

            lock.unlock();
        }
    }

    void PacketSelector::addSocket( std::shared_ptr< SocketContainer > container )
    {
        std::unique_lock< std::mutex > lock( m_containerMutex );
        m_changeList.push_back( std::make_tuple( 0, std::move( container ) ) );
        WSASetEvent( m_notifyEvent );
    }

    void PacketSelector::removeSocket( const std::shared_ptr< SocketContainer >& container )
    {
        std::unique_lock< std::mutex > lock( m_containerMutex );
        m_changeList.push_back( std::make_tuple( 1, std::move( container ) ) );
        WSASetEvent( m_notifyEvent );
    }

    void PacketSelector::interrupt()
    {
        std::unique_lock< std::mutex > lock( m_containerMutex );
        m_running = false;
        WSASetEvent( m_notifyEvent );
    }
#else

    #include <sys/socket.h>
    #include <sys/types.h>
    #include <fcntl.h>
    #include <netdb.h>
    #include <unistd.h>
    #include <stdio.h>

    PacketSelector::NetEvent::NetEvent( Type type, SocketContainer* source )
        : m_type( type )
        , m_source( source )
    {
    }

    PacketSelector::PacketSelector()
    {
        m_epoll = epoll_create1( 0 );

        if( m_epoll == -1 )
            std::cout << "EPOLL-INIT-ERROR: " << errno << std::endl;

        struct epoll_event event;
        event.events = EPOLLIN;
        event.data.ptr = NULL;
        pipe( m_pipe );

        if( epoll_ctl( m_epoll, EPOLL_CTL_ADD, m_pipe[ 0 ], &event ) == -1 )
            std::cout << "EPOLL-PIPE-ERROR: " << errno << std::endl;
    }

    PacketSelector::~PacketSelector()
    {
        close( m_pipe[ 0 ] );
        close( m_pipe[ 1 ] );
    }

    void PacketSelector::wait( std::function< void ( std::unique_ptr< NetEvent > event ) > callback )
    {
        bool rebuildList = false;
        m_running = true;
        while( true )
        {
            int num = epoll_wait( m_epoll, m_events, 64, -1 );

            if( num == -1 )
            {
                switch( errno )
                {
                    case EINTR:
                        break;
                    default:
                        std::cout << "EPOLL-ERROR: " << errno << std::endl;
                }
            }

            bool breakLoop = false;

            for( int i = 0; i < num; ++i )
            {
                if( m_events[ i ].data.ptr == NULL )
                {
                    char buff;
                    read( m_pipe[ 0 ], &buff, 1 );

                    if( !m_running )
                        breakLoop = true;
                }
                else
                {
                    SocketContainer* container = static_cast< SocketContainer* >( m_events[ i ].data.ptr );

                    try
                    {
                        std::unique_ptr< IPacket > packet( IPacket::fromTCPSocket( container->getSocket() ) );

                        callback( std::unique_ptr< NetEvent >( new NetEvent_Impl< IPacket >(
                            NetEvent::Type::PACKET,
                            container,
                            std::move( packet ) ) ) );
                    }
                    catch( TCPSocket::Error& e )
                    {
                        epoll_ctl( m_epoll, EPOLL_CTL_DEL, container->getSocket()->handle(), NULL );

                        switch( e )
                        {
                            case TCPSocket::Error::CLOSED:
                                callback( std::unique_ptr< NetEvent >( new NetEvent( NetEvent::Type::CLOSED, container ) ) );
                                break;
                            case TCPSocket::Error::BROKEN:
                                callback( std::unique_ptr< NetEvent >( new NetEvent( NetEvent::Type::BROKEN, container ) ) );
                                break;
                            default:
                                break;
                        }
                    }
                }
            }

            // add new sockets
            m_containerMutex.lock();
            for( auto i = m_newContainer.begin(); i != m_newContainer.end(); i = m_newContainer.erase( i ) )
            {
                {
                    struct epoll_event event;
                    event.events = EPOLLIN;
                    event.data.ptr = *i;
                    epoll_ctl( m_epoll, EPOLL_CTL_ADD, (*i)->getSocket()->handle(), &event );
                }

                callback( std::unique_ptr< NetEvent >( new NetEvent(
                    NetEvent::Type::CONNECT,
                    *i ) ) );
            }
            m_containerMutex.unlock();

            if( breakLoop )
                break;
        }
    }

    void PacketSelector::add( SocketContainer* container )
    {
        int flags = fcntl( container->getSocket()->handle(), F_GETFL, 0 );
        flags |= O_NONBLOCK;
        fcntl( container->getSocket()->handle(), F_SETFL, flags );
        {
            std::unique_lock< std::mutex > lock( m_containerMutex );
            m_newContainer.push_back( container );
        }
        char buff = ' ';
        write( m_pipe[ 1 ], &buff, 1 );
    }

    void PacketSelector::notify()
    {
        m_running = false;
        char buff = ' ';
        write( m_pipe[ 1 ], &buff, 1 );
    }

#endif
