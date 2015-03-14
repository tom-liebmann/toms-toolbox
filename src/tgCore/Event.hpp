#pragma once

// declarations
//=============================================================================

namespace tg
{
    class Event
    {
    public:
        class Type;

        virtual ~Event();

        virtual Type getType() const = 0;
    };



    class Event::Type
    {
    public:
        constexpr Type( uint32_t value );

        constexpr bool operator<( const Type& type );
        constexpr bool operator==( const Type& type );
        constexpr operator uint32_t();

    private:
        uint32_t m_value;
    };
}



// definitions
//=============================================================================
namespace tg
{
    inline Event::~Event()
    { }



    inline constexpr Event::Type::Type( uint32_t value )
        : m_value( value )
    { }

    inline constexpr bool Event::Type::operator<( const Type& type )
    {
        return m_value < type.m_value;
    }

    inline constexpr bool Event::Type::operator==( const Type& type )
    {
        return m_value == type.m_value;
    }

    inline constexpr Event::Type::operator uint32_t()
    {
        return m_value;
    }
}
