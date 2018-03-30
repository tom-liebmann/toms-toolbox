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

            std::shared_ptr< TCPSocket > const& socket();

            virtual Type type() const override;

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

        inline std::shared_ptr< TCPSocket > const& ClientConnection::socket()
        {
            return m_socket;
        }

        inline Event::Type ClientConnection::type() const
        {
            return CLIENT_CONNECTION;
        }
    }
}
