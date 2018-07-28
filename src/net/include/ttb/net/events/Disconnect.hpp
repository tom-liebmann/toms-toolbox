#pragma once

#include <ttb/net/events.hpp>


namespace ttb
{
    namespace events
    {
        class Disconnect : public Event
        {
        public:
            enum class Reason
            {
                NORMAL,
                BROKEN,
                TIMEOUT
            };

            Disconnect( Reason reason );

            Reason reason() const;

            // Override: Event
            virtual Type type() const override;
            virtual std::unique_ptr< Event > clone() const override;
            virtual std::unique_ptr< Event > move() override;

        private:
            Reason m_reason;
        };
    }
}


namespace ttb
{
    namespace events
    {
        inline Disconnect::Disconnect( Reason reason ) : m_reason( reason )
        {
        }

        inline Disconnect::Reason Disconnect::reason() const
        {
            return m_reason;
        }

        inline Event::Type Disconnect::type() const
        {
            return DISCONNECT;
        }

        inline std::unique_ptr< Event > Disconnect::clone() const
        {
            return std::unique_ptr< Event >( new Disconnect( m_reason ) );
        }

        inline std::unique_ptr< Event > Disconnect::move()
        {
            return std::unique_ptr< Event >( new Disconnect( m_reason ) );
        }
    }
}
