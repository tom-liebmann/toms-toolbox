#pragma once

#include "WindowFlag.hpp"
#include <ttb/math/Vector.hpp>


namespace ttb
{
    class WindowRequest
    {
    public:
        using Size = ttb::Vector< uint16_t, 2 >;

        WindowRequest( Size const& size, WindowFlag flags );

        Size const& size() const;

        WindowFlag flags() const;

    private:
        Size m_size;
        WindowFlag m_flags;
    };
}


namespace ttb
{
    inline WindowRequest::WindowRequest( Size const& size, WindowFlag flags = WindowFlag::NONE )
        : m_size( size ), m_flags{ flags }
    {
    }

    inline auto WindowRequest::size() const -> Size const&
    {
        return m_size;
    }

    inline WindowFlag WindowRequest::flags() const
    {
        return m_flags;
    }
}
