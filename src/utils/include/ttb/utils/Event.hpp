#pragma once

#include <ttb/math/Matrix.hpp>
#include <ttb/math/Vector.hpp>
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

        using Transform =
            std::function< ttb::Vector< float, 2 >( ttb::Vector< float, 2 > const& ) >;
        using EventCallback = std::function< void( ttb::Event const& ) >;

        virtual void transform( Transform const& transform, EventCallback const& callback ) const;

        void transform( ttb::Matrix< float, 3, 3 > const& transform,
                        EventCallback const& callback ) const;
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


    template < uint32_t TType >
    inline uint32_t TypedEvent< TType >::type() const
    {
        return TType;
    }
}
