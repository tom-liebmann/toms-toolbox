#pragma once

#include <ttb/net/events.hpp>


namespace ttb
{
    namespace events
    {
        class Disconnect : public Event
        {
        public:
            Disconnect( std::shared_ptr< SelectableHolder > source );

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
        inline Disconnect::Disconnect( std::shared_ptr< SelectableHolder > source )
            : m_source( std::move( source ) )
        {
        }

        inline std::shared_ptr< SelectableHolder > const& Disconnect::source()
        {
            return m_source;
        }

        inline Event::Type Disconnect::type() const
        {
            return DISCONNECT;
        }
    }
}
