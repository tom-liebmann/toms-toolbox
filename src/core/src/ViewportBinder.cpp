#include <ttb/core/ViewportBinder.hpp>


namespace ttb
{
    ViewportBinder::Handle::~Handle()
    {
        m_data.viewport = m_prevViewport;
        m_data.enabledScissors = m_prevScissors;

        m_data.applyViewport();
    }

    ViewportBinder::Handle::Handle( ViewportBinder const& binder, State::Data& data )
        : m_binder( binder )
        , m_data( data )
        , m_prevViewport( m_data.viewport )
        , m_prevScissors( m_data.enabledScissors )
    {
        m_data.viewport = m_binder.m_viewport;
        m_data.enabledScissors = m_binder.m_applyScissors;

        m_data.applyViewport();
    }
}
