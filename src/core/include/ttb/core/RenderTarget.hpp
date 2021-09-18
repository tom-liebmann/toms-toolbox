#pragma once

#include <ttb/core/State.hpp>
#include <ttb/core/Viewport.hpp>

#include <cstdint>
#include <cstdlib>


// definitions
//=================================================================================================

namespace ttb
{
    class RenderTarget
    {
    public:
        class Handle;

        virtual ~RenderTarget();

        virtual Viewport viewport() const = 0;

        virtual void begin( State::Data& data ) const = 0;

        virtual void suspend( State::Data& data ) const = 0;

        virtual void resume( State::Data& data ) const = 0;

        virtual void end( State::Data& data ) const = 0;

        Handle bind( State::Data& data ) const;
    };


    class RenderTarget::Handle
    {
    public:
        ~Handle();

        Handle() = delete;
        Handle( Handle&& rhs ) = delete;
        Handle( Handle const& ) = delete;
        Handle& operator=( Handle&& rhs ) = delete;
        Handle& operator=( Handle const& ) = delete;

    private:
        Handle( RenderTarget const& target, State::Data& data );

        RenderTarget const& m_target;
        State::Data& m_data;

        RenderTarget const* m_prevTarget{ nullptr };

        std::optional< Viewport > m_prevViewport;
        bool m_prevScissors{ false };

        friend RenderTarget;
    };
}


namespace ttb
{
    inline auto RenderTarget::bind( State::Data& data ) const -> Handle
    {
        return { *this, data };
    }
}
