#pragma once


namespace ttb
{
    namespace events
    {
        class ConnectionFailed : public Event
        {
        public:
            // Override: Event
            virtual Type type() const override;
            virtual std::unique_ptr< Event > clone() const override;
            virtual std::unique_ptr< Event > move() override;
        };
    }
}


namespace ttb
{
    namespace events
    {
        inline Event::Type ConnectionFailed::type() const
        {
            return CONNECTION_FAILED;
        }

        inline std::unique_ptr< Event > ConnectionFailed::clone() const
        {
            return std::unique_ptr< Event >( new ConnectionFailed() );
        }

        inline std::unique_ptr< Event > ConnectionFailed::move()
        {
            return std::unique_ptr< Event >( new ConnectionFailed() );
        }
    }
}
