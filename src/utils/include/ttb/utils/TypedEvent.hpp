#pragma once


namespace ttb
{
    class TypedEvent : public Event
    {
    public:
        TypedEvent( Event::Type type );

        // Override: Event
        virtual Type type() const override;
        virtual std::unique_ptr< Event > clone() const override;
        virtual std::unique_ptr< Event > move() override;

    private:
        Event::Type m_type;
    };
}


namespace ttb
{
    inline TypedEvent::TypedEvent( Event::Type type ) : m_type( type )
    {
    }

    inline Event::Type TypedEvent::type() const
    {
        return m_type;
    }

    inline std::unique_ptr< Event > TypedEvent::clone() const
    {
        return std::unique_ptr< Event >( new TypedEvent( m_type ) );
    }

    inline std::unique_ptr< Event > TypedEvent::move()
    {
        return std::unique_ptr< Event >( new TypedEvent( m_type ) );
    }
}
