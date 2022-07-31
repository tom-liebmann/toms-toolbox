#pragma once

#include <ttb/utils/Event.hpp>


namespace ttb
{
    class TypeValueEvent : public Event
    {
    public:
        TypeValueEvent( uint32_t type );

        // Override: Event
        virtual uint32_t type() const override;

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
}
