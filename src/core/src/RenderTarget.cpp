#include <ttb/core/RenderTarget.hpp>

namespace ttb
{
    RenderTarget::~RenderTarget() = default;


    RenderTarget::Handle::~Handle()
    {
        m_target.suspend( m_data );
        m_target.end( m_data );

        m_data.renderTarget = m_prevTarget;
        m_data.viewport = m_prevViewport;
        m_data.enabledScissors = m_prevScissors;

        if( m_prevTarget )
        {
            m_prevTarget->resume( m_data );
        }

        m_data.applyViewport();
    }

    RenderTarget::Handle::Handle( RenderTarget const& target, State::Data& data )
        : m_target{ target }
        , m_data{ data }
        , m_prevTarget{ m_data.renderTarget }
        , m_prevViewport{ m_data.viewport }
        , m_prevScissors{ m_data.enabledScissors }
    {
        // Apply render target
        m_data.renderTarget = &m_target;

        if( m_prevTarget )
        {
            m_prevTarget->suspend( data );
        }

        m_target.begin( data );
        m_target.resume( data );

        // Apply viewport
        m_data.viewport = m_target.viewport();
        m_data.enabledScissors = false;

        m_data.applyViewport();
    }
}
