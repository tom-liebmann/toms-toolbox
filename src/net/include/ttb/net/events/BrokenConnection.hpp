#pragma once

#include <ttb/net/events.hpp>


namespace ttb
{
    class SelectableHolder;
}


namespace ttb
{
    namespace events
    {
        class BrokenConnection : public Event
        {
        public:
            BrokenConnection( std::shared_ptr< SelectableHolder > source );

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
        inline BrokenConnection::BrokenConnection( std::shared_ptr< SelectableHolder > source )
            : m_source( std::move( source ) )
        {
        }

        inline std::shared_ptr< SelectableHolder > const& BrokenConnection::source()
        {
            return m_source;
        }

        inline Event::Type BrokenConnection::type() const
        {
            return BROKEN_CONNECTION;
        }
    }
}
