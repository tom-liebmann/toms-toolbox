#pragma once

#include <ttb/net/events.hpp>


namespace ttb
{
    namespace events
    {
        class ClientConnection : public Event
        {
        public:
            ClientConnection( std::shared_ptr< SelectableHolder > source,
                              std::shared_ptr< TCPSocket > socket );

            std::shared_ptr< SelectableHolder > const& source();

            std::shared_ptr< TCPSocket > const& socket();

            virtual Type type() const override;

        private:
            std::shared_ptr< SelectableHolder > m_source;
            std::shared_ptr< TCPSocket > m_socket;
        };
    }
}


namespace ttb
{
    namespace events
    {
        inline ClientConnection::ClientConnection( std::shared_ptr< SelectableHolder > source,
                                                   std::shared_ptr< TCPSocket > socket )
            : m_source( std::move( source ) ), m_socket( std::move( socket ) )
        {
        }

        inline std::shared_ptr< SelectableHolder > const& ClientConnection::source()
        {
            return m_source;
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
