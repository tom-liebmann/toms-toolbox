#pragma once


namespace ttb
{
    class TypeValueEvent : public Event
    {
    public:
        TypeValueEvent( uint32_t type );

        // Override: Event
        virtual uint32_t type() const override;
        virtual std::unique_ptr< Event > clone() const override;
        virtual std::unique_ptr< Event > move() override;

    private:
        uint32_t const m_type;
    };
}


namespace ttb
{
    inline TypeValueEvent::TypeValueEvent( uint32_t type ) : m_type{ type }
    {
    }

    inline uint32_t TypeValueEvent::type() const
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
