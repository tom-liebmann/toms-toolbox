#pragma once

#include <ttb/core/State.hpp>
#include <ttb/core/Viewport.hpp>


namespace ttb
{
    class ViewportBinder
    {
    public:
        class Handle;

        using Viewport = ttb::Range< uint32_t, 2 >;

        ViewportBinder( Viewport const& viewport, bool applyScissors = false );

        Handle bind( State::Data& data ) const;

        Viewport m_viewport;
        bool m_applyScissors;
    };


    class ViewportBinder::Handle
    {
    public:
        ~Handle();

    private:
        Handle( ViewportBinder const& binder, State::Data& data );

        ViewportBinder const& m_binder;
        State::Data& m_data;
        std::optional< Viewport > m_prevViewport;
        bool m_prevScissors;

        friend ViewportBinder;
    };
}


namespace ttb
{
    inline ViewportBinder::ViewportBinder( Viewport const& viewport, bool applyScissors )
        : m_viewport( viewport ), m_applyScissors{ applyScissors }
    {
    }

    inline auto ViewportBinder::bind( State::Data& data ) const -> Handle
    {
        return { *this, data };
    }
}
