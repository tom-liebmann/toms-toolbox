#pragma once

#include <ttb/core/Bindable.hpp>
#include <ttb/core/State.hpp>
#include <ttb/core/Viewport.hpp>


namespace ttb
{
    class ViewportBinder : public Bindable< ViewportBinder >
    {
    public:
        using Viewport = ttb::Range< uint32_t, 2 >;

        ViewportBinder( Viewport const& viewport, bool applyScissors = false );

        Viewport m_viewport;
        bool m_applyScissors;

        friend Bindable;
    };


    template <>
    class Bindable< ViewportBinder >::Binder
    {
    public:
        ~Binder();

    private:
        Binder( ViewportBinder const& binder, State::Data& data );

        ViewportBinder const& m_binder;
        State::Data& m_data;
        std::optional< Viewport > m_prevViewport;
        bool m_prevScissors;

        friend Bindable;
    };
}


namespace ttb
{
    inline ViewportBinder::ViewportBinder( Viewport const& viewport, bool applyScissors )
        : m_viewport( viewport ), m_applyScissors{ applyScissors }
    {
    }
}
