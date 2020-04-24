#pragma once


namespace ttb
{
    class TypeValueEvent : public Event
    {
    public:
        TypeValueEvent( Event::Type type );

        // Override: Event
        virtual Type type() const override;
        virtual std::unique_ptr< Event > clone() const override;
        virtual std::unique_ptr< Event > move() override;

    private:
        Event::Type const m_type;
    };
}


namespace ttb
{
    inline TypeValueEvent::TypeValueEvent( Event::Type type ) : m_type{ type }
    {
    }

    inline Event::Type TypeValueEvent::type() const
    {
        return m_type;
    }

    inline std::unique_ptr< Event > TypeValueEvent::clone() const
    {
        return std::unique_ptr< Event >{ new TypeValueEvent{ m_type } };
    }

    inline std::unique_ptr< Event > TypeValueEvent::move()
    {
        return std::unique_ptr< Event >{ new TypeValueEvent{ m_type } };
    }
}
