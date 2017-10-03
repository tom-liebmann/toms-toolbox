#pragma once

#include <ttb/utils/Event.hpp>


namespace ttb
{
    namespace events
    {
        constexpr Event::Type PACKET = 6;
        constexpr Event::Type SOCKET_BROKEN = 7;
        constexpr Event::Type SOCKET_CLOSED = 8;


        class PacketEvent : public Event
        {
        public:
            PacketEvent( std::shared_ptr< ttb::TCPSocket > socket,
                         std::shared_ptr< ttb::IPacket > packet );

            std::shared_ptr< ttb::TCPSocket > const& socket();

            std::shared_ptr< ttb::IPacket > const& packet();

            virtual Type type() const override;

        private:
            std::shared_ptr< ttb::TCPSocket > m_socket;
            std::shared_ptr< ttb::IPacket > m_packet;
        };


        class SocketBrokenEvent : public Event
        {
        public:
            SocketBrokenEvent( std::shared_ptr< ttb::TCPSocket > socket );

            std::shared_ptr< ttb::TCPSocket > const& socket();

            virtual Type type() const override;

        private:
            std::shared_ptr< ttb::TCPSocket > m_socket;
        };


        class SocketClosedEvent : public Event
        {
        public:
            SocketClosedEvent( std::shared_ptr< ttb::TCPSocket > socket );

            std::shared_ptr< ttb::TCPSocket > const& socket();

            virtual Type type() const override;

        private:
            std::shared_ptr< ttb::TCPSocket > m_socket;
        };
    }
}


namespace ttb
{
    namespace events
    {
        PacketEvent::PacketEvent( std::shared_ptr< ttb::TCPSocket > socket,
                                  std::shared_ptr< ttb::IPacket > packet )
            : m_socket( std::move( socket ) ), m_packet( std::move( packet ) )
        {
        }

        std::shared_ptr< ttb::TCPSocket > const& PacketEvent::socket()
        {
            return m_socket;
        }

        std::shared_ptr< ttb::IPacket > const& PacketEvent::packet()
        {
            return m_packet;
        }

        Event::Type PacketEvent::type() const
        {
            return PACKET;
        }


        SocketBrokenEvent::SocketBrokenEvent( std::shared_ptr< ttb::TCPSocket > socket )
            : m_socket( std::move( socket ) )
        {
        }

        std::shared_ptr< ttb::TCPSocket > const& SocketBrokenEvent::socket()
        {
            return m_socket;
        }

        Event::Type SocketBrokenEvent::type() const
        {
            return SOCKET_BROKEN;
        }


        SocketClosedEvent::SocketClosedEvent( std::shared_ptr< ttb::TCPSocket > socket )
            : m_socket( std::move( socket ) )
        {
        }

        std::shared_ptr< ttb::TCPSocket > const& SocketClosedEvent::socket()
        {
            return m_socket;
        }

        Event::Type SocketClosedEvent::type() const
        {
            return SOCKET_CLOSED;
        }
    }
}
