#pragma once

#include <ttb/net/events.hpp>

#include <memory>


namespace ttb
{
    class TCPSocket;
}


namespace ttb
{
    namespace events
    {
        class ClientConnection : public Event
        {
        public:
            ClientConnection( std::shared_ptr< TCPSocket > socket );

            ClientConnection( ClientConnection const& copy );

            std::shared_ptr< TCPSocket > const& socket();

            // Override: Event
            virtual Type type() const override;
            virtual std::unique_ptr< Event > clone() const override;
            virtual std::unique_ptr< Event > move() override;

        private:
            std::shared_ptr< TCPSocket > m_socket;
        };
    }
}


namespace ttb
{
    namespace events
    {
        inline ClientConnection::ClientConnection( std::shared_ptr< TCPSocket > socket )
            : m_socket( std::move( socket ) )
        {
        }

        inline ClientConnection::ClientConnection( ClientConnection const& copy )
            : m_socket( copy.m_socket )
        {
        }

        inline std::shared_ptr< TCPSocket > const& ClientConnection::socket()
        {
            return m_socket;
        }

        inline Event::Type ClientConnection::type() const
        {
            return CLIENT_CONNECTION;
        }

        inline std::unique_ptr< Event > ClientConnection::clone() const
        {
            return std::unique_ptr< Event >( new ClientConnection( *this ) );
        }

        inline std::unique_ptr< Event > ClientConnection::move()
        {
            return std::unique_ptr< Event >( new ClientConnection( *this ) );
        }
    }
}
