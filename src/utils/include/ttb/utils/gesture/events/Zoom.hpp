#pragma once

#include <ttb/utils/Event.hpp>
#include <ttb/utils/EventType.hpp>


namespace ttb::events::gst
{
    class Zoom : public TypedEvent< ttb::event::type::gst::ZOOM >
    {
    public:
        using Rotation = ttb::Quaternion< float >;

        explicit Zoom( float factor, Rotation rotation = Rotation{} );

        float factor() const;

        Rotation const& rotation() const;

    private:
        float const m_factor;
        Rotation m_rotation;
    };
}


namespace ttb::events::gst
{
    inline Zoom::Zoom( float factor, Rotation rotation )
        : m_factor{ factor }, m_rotation{ rotation }
    {
    }

    inline float Zoom::factor() const
    {
        return m_factor;
    }

    inline auto Zoom::rotation() const -> Rotation const&
    {
        return m_rotation;
    }
}
