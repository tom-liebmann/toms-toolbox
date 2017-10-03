#pragma once

#include <cstdint>
#include <functional>

// declarations
//=============================================================================

namespace ttb
{
    class Event
    {
    public:
        class Type;

        virtual ~Event();

        virtual Type type() const = 0;
    };



    class Event::Type
    {
    public:
        constexpr Type( uint32_t value );

        constexpr uint32_t value() const;

        constexpr bool operator<( const Type& type ) const;
        constexpr bool operator==( const Type& type ) const;
        constexpr operator uint32_t() const;

    private:
        uint32_t m_value;
    };
}



namespace std
{
    template <>
    struct hash< ttb::Event::Type >;
}



// definitions
//=============================================================================
namespace ttb
{
    inline Event::~Event()
    {
    }



    inline constexpr Event::Type::Type( uint32_t value ) : m_value( value )
    {
    }

    inline constexpr uint32_t Event::Type::value() const
    {
        return m_value;
    }

    inline constexpr bool Event::Type::operator<( const Type& type ) const
    {
        return m_value < type.m_value;
    }

    inline constexpr bool Event::Type::operator==( const Type& type ) const
    {
        return m_value == type.m_value;
    }

    inline constexpr Event::Type::operator uint32_t() const
    {
        return m_value;
    }
}



namespace std
{
    template <>
    struct hash< ttb::Event::Type >
    {
        size_t operator()( ttb::Event::Type const& type ) const
        {
            return type.value();
        }
    };
}
