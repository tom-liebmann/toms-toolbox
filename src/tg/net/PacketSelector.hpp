#pragma once

#ifdef WIN32
    #include <winsock2.h>
#else
    #include <sys/epoll.h>
#endif

#include <tg/core/EventManager.hpp>
#include <tg/net/IPacket.hpp>

#include <functional>
#include <vector>
#include <mutex>
#include <memory>
#include <list>
#include <atomic>



// forward declarations
//=============================================================================

namespace tg
{
    class SocketContainer;
}



// declarations
//=============================================================================

namespace tg
{
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
                    virtual std::shared_ptr< SocketContainer > getSource() const = 0;
            };

            PacketSelector();

            ~PacketSelector();

            void addSocket( std::shared_ptr< SocketContainer > container );
            void removeSocket( const std::shared_ptr< SocketContainer >& container );
            
            std::unique_ptr< Event > wait();
            void interrupt();

        private:
            std::atomic< bool > m_interrupted;

            void processChanges();
            void insertManagedSocket( std::shared_ptr< SocketContainer > container );
            void removeManagedSocket( const std::shared_ptr< SocketContainer >& container );
            void pollEvents( bool& interrupted );
            void handleEvent( size_t index );
            void wakeUp();

            /**
             * Set of sockets managed by this selector.
             */
            std::vector< std::shared_ptr< SocketContainer > > m_managedSockets;

            /**
             * List of sockets to be deleted from or inserted into the managed list.
             */
            std::list< std::tuple< uint8_t, std::shared_ptr< SocketContainer > > > m_changeList;

            /**
             * Mutex for thread safe handling of socket changes.
             */
            std::mutex m_changeMutex;

            /**
             * List of slots in m_managedSockets not filled with a socket.
             */
            std::list< size_t > m_freeSlots;

            /**
             * Ids of sockets that currently have an event waiting to be processed.
             */
            std::list< size_t > m_eventSockets;

        #ifdef WIN32
            private:
                std::vector< WSAEVENT > m_eventHandles;
                std::vector< WSAEVENT > m_events;
                std::vector< size_t > m_eventIds;
    
                HANDLE m_notifyEvent;
        #else
            private:
                /**
                 * Handle to polling mechanism.
                 */
                int m_epoll;

                /**
                 * Buffer for incomming events.
                 */
                std::array< epoll_event, 64 > m_events;

                /**
                 * Read/Write pipe to interrupt event polling.
                 */
                int m_pipe[ 2 ];
        #endif
    };



    class PacketEvent
        : public PacketSelector::Event
    {
        public:
            PacketEvent(
                const std::shared_ptr< SocketContainer >& source,
                std::unique_ptr< IPacket > packet );

            const std::unique_ptr< IPacket >& getPacket() const;

            // PacketSelector::Event
            virtual Type getType() const override; 
            virtual std::shared_ptr< SocketContainer > getSource() const override;

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

            Reason getReason() const;

            // PacketSelector::Event
            virtual Type getType() const override;
            virtual std::shared_ptr< SocketContainer > getSource() const override;

        private:
            std::weak_ptr< SocketContainer > m_source;
            Reason m_reason;
    };
}



// definitions
//=============================================================================

namespace tg
{
    inline PacketSelector::Event::~Event()
    { }



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
