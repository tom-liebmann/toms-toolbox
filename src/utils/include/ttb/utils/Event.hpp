#pragma once

#include <ttb/utils/EventType.hpp>

#include <cstdint>
#include <functional>
#include <memory>
#include <stdexcept>


// declarations
//=============================================================================

namespace ttb
{
    class Event
    {
    public:
        virtual ~Event();

        virtual uint32_t type() const = 0;
        virtual std::unique_ptr< Event > clone() const;
        virtual std::unique_ptr< Event > move();
    };


    template < uint32_t TType >
    class TypedEvent : public Event
    {
    public:
        virtual uint32_t type() const override;
    };
}


// definitions
//=============================================================================
namespace ttb
{
    inline Event::~Event() = default;

    inline std::unique_ptr< Event > Event::clone() const
    {
        throw std::runtime_error( "Event not clonable" );
    }

    inline std::unique_ptr< Event > Event::move()
    {
        throw std::runtime_error( "Event not movable" );
    }


    template < uint32_t TType >
    inline uint32_t TypedEvent< TType >::type() const
    {
        return TType;
    }
}
