#pragma once

#include <ttb/utils/Event.hpp>


namespace ttb
{
    class Listener;
    class Socket;
    class IPacket;
    class TCPSocket;
}


namespace ttb
{
    namespace events
    {
        constexpr Event::Type PACKET = 6;
        constexpr Event::Type SOCKET_BROKEN = 7;
        constexpr Event::Type SOCKET_CLOSED = 8;
        constexpr Event::Type CONNECTION = 9;


        class ConnectionEvent : public Event
        {
        public:
            ConnectionEvent( std::shared_ptr< ttb::Listener > listener,
                             std::shared_ptr< ttb::TCPSocket > socket );

            std::shared_ptr< ttb::Listener > const& listener();

            std::shared_ptr< ttb::TCPSocket > const& socket();

            virtual Type type() const override;

        private:
            std::shared_ptr< ttb::Listener > m_listener;
            std::shared_ptr< ttb::TCPSocket > m_socket;
        };


        class PacketEvent : public Event
        {
        public:
            PacketEvent( std::shared_ptr< ttb::Socket > socket,
                         std::shared_ptr< ttb::IPacket > packet );

            std::shared_ptr< ttb::Socket > const& socket();

            std::shared_ptr< ttb::IPacket > const& packet();

            virtual Type type() const override;

        private:
            std::shared_ptr< ttb::Socket > m_socket;
            std::shared_ptr< ttb::IPacket > m_packet;
        };


        class SocketBrokenEvent : public Event
        {
        public:
            SocketBrokenEvent( std::shared_ptr< ttb::Socket > socket );

            std::shared_ptr< ttb::Socket > const& socket();

            virtual Type type() const override;

        private:
            std::shared_ptr< ttb::Socket > m_socket;
        };


        class SocketClosedEvent : public Event
        {
        public:
            SocketClosedEvent( std::shared_ptr< ttb::Socket > socket );

            std::shared_ptr< ttb::Socket > const& socket();

            virtual Type type() const override;

        private:
            std::shared_ptr< ttb::Socket > m_socket;
        };
    }
}


namespace ttb
{
    namespace events
    {
        inline ConnectionEvent::ConnectionEvent( std::shared_ptr< ttb::Listener > listener,
                                                 std::shared_ptr< ttb::TCPSocket > socket )
            : m_listener( std::move( listener ) ), m_socket( std::move( socket ) )
        {
        }

        inline std::shared_ptr< ttb::Listener > const& ConnectionEvent::listener()
        {
            return m_listener;
        }

        inline std::shared_ptr< ttb::TCPSocket > const& ConnectionEvent::socket()
        {
            return m_socket;
        }

        inline Event::Type ConnectionEvent::type() const
        {
            return CONNECTION;
        }


        inline PacketEvent::PacketEvent( std::shared_ptr< ttb::Socket > socket,
                                         std::shared_ptr< ttb::IPacket > packet )
            : m_socket( std::move( socket ) ), m_packet( std::move( packet ) )
        {
        }

        inline std::shared_ptr< ttb::Socket > const& PacketEvent::socket()
        {
            return m_socket;
        }

        inline std::shared_ptr< ttb::IPacket > const& PacketEvent::packet()
        {
            return m_packet;
        }

        inline Event::Type PacketEvent::type() const
        {
            return PACKET;
        }


        inline SocketBrokenEvent::SocketBrokenEvent( std::shared_ptr< ttb::Socket > socket )
            : m_socket( std::move( socket ) )
        {
        }

        inline std::shared_ptr< ttb::Socket > const& SocketBrokenEvent::socket()
        {
            return m_socket;
        }

        inline Event::Type SocketBrokenEvent::type() const
        {
            return SOCKET_BROKEN;
        }


        inline SocketClosedEvent::SocketClosedEvent( std::shared_ptr< ttb::Socket > socket )
            : m_socket( std::move( socket ) )
        {
        }

        inline std::shared_ptr< ttb::Socket > const& SocketClosedEvent::socket()
        {
            return m_socket;
        }

        inline Event::Type SocketClosedEvent::type() const
        {
            return SOCKET_CLOSED;
        }
    }
}
