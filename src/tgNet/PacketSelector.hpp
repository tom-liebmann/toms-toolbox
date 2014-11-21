#pragma once

#ifdef WIN32
    #include <winsock2.h>
#else
    #include <sys/epoll.h>
#endif

#include <tgCore/EventManager.hpp>
#include <tgNet/IPacket.hpp>

#include <functional>
#include <vector>
#include <mutex>
#include <memory>
#include <list>

namespace tgNet
{
    class SocketContainer;

    class PacketSelector
    {
        public:
            class Event
            {
                public:
                    enum class Type
                    {
                        PACKET,
                        DISCONNECT
                    };

                    virtual ~Event();

                    virtual Type getType() const = 0;
            };

            PacketSelector();

            ~PacketSelector();

            void addSocket( std::shared_ptr< SocketContainer > container );
            void removeSocket( const std::shared_ptr< SocketContainer >& container );
            
            void wait( const std::function< void ( std::unique_ptr< Event > ) >& callback );
            void interrupt();

        private:
            bool m_running;

            std::list< std::tuple< uint8_t, std::shared_ptr< SocketContainer > > > m_changeList;

        #ifdef WIN32
            private:
                std::mutex m_containerMutex;
                std::vector< std::tuple< WSAEVENT, std::shared_ptr< SocketContainer > > > m_container;
                std::set< std::shared_ptr< SocketContainer > > m_eventContainer;
                WSAEVENT* m_events;
                DWORD m_eventSize;
    
                HANDLE m_notifyEvent;
        #else
            private:
                int m_pipe[ 2 ];
                int m_epoll;
                struct epoll_event m_events[ 64 ];
                std::mutex m_containerMutex;
                std::list< SocketContainer* > m_newContainer;
        #endif
    };



    inline PacketSelector::Event::~Event()
    { }



    class PacketEvent
        : public PacketSelector::Event
    {
        public:
            PacketEvent(
                const std::shared_ptr< SocketContainer >& source,
                std::unique_ptr< IPacket > packet );

            // PacketSelector::Event
            virtual Type getType() const override; 

            std::shared_ptr< SocketContainer > getSource() const;
            const std::unique_ptr< IPacket >& getPacket() const;

        private:
            std::weak_ptr< SocketContainer > m_source;
            std::unique_ptr< IPacket > m_packet;
    };

    class DisconnectEvent
        : public PacketSelector::Event
    {
        public:
            enum class Reason
            {
                NORMAL,
                BROKEN
            };

            DisconnectEvent( const std::shared_ptr< SocketContainer >& source, Reason reason );

            // PacketSelector::Event
            virtual Type getType() const override;

            std::shared_ptr< SocketContainer > getSource() const;
            Reason getReason() const;

        private:
            std::weak_ptr< SocketContainer > m_source;
            Reason m_reason;
    };



    inline PacketEvent::PacketEvent(
        const std::shared_ptr< SocketContainer >& source,
        std::unique_ptr< IPacket > packet )
        : m_source( source )
        , m_packet( std::move( packet ) )
    { }

    inline PacketSelector::Event::Type PacketEvent::getType() const
    {
        return Type::PACKET;
    }

    inline std::shared_ptr< SocketContainer > PacketEvent::getSource() const
    {
        return m_source.lock();
    }

    inline const std::unique_ptr< IPacket >& PacketEvent::getPacket() const
    {
        return m_packet;
    }



    inline DisconnectEvent::DisconnectEvent(
        const std::shared_ptr< SocketContainer >& source,
        Reason reason )
        : m_source( source )
        , m_reason( reason )
    { }

    inline PacketSelector::Event::Type DisconnectEvent::getType() const
    {
        return Type::DISCONNECT;
    }

    inline std::shared_ptr< SocketContainer > DisconnectEvent::getSource() const
    {
        return m_source.lock();
    }

    inline DisconnectEvent::Reason DisconnectEvent::getReason() const
    {
        return m_reason;
    }
}
