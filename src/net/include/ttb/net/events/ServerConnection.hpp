#pragma once

#include <ttb/net/events.hpp>


namespace ttb
{
    namespace events
    {
        class ServerConnection : public Event
        {
        public:
            ServerConnection( std::shared_ptr< SelectableHolder > source );

            std::shared_ptr< SelectableHolder > const& source();

            virtual Type type() const override;

        private:
            std::shared_ptr< SelectableHolder > m_source;
        };
    }
}


namespace ttb
{
    namespace events
    {
        inline ServerConnection::ServerConnection( std::shared_ptr< SelectableHolder > source )
            : m_source( std::move( source ) )
        {
        }

        inline std::shared_ptr< SelectableHolder > const& ServerConnection::source()
        {
            return m_source;
        }

        inline Event::Type ServerConnection::type() const
        {
            return SERVER_CONNECTION;
        }
    }
}
