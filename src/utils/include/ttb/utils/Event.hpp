#pragma once

#include <cstdint>
#include <functional>
#include <memory>


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
        virtual std::unique_ptr< Event > clone() const;
        virtual std::unique_ptr< Event > move();
    };


    template < uint32_t TType >
    class TypedEvent : public Event
    {
    public:
        virtual Type type() const override;
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

    inline std::unique_ptr< Event > Event::clone() const
    {
        throw std::runtime_error( "Event not clonable" );
    }

    inline std::unique_ptr< Event > Event::move()
    {
        throw std::runtime_error( "Event not movable" );
    }


    template < uint32_t TType >
    inline Event::Type TypedEvent< TType >::type() const
    {
        return { TType };
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
