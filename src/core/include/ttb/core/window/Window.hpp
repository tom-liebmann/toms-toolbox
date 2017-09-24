#pragma once

#include <ttb/core/EventManager.hpp>
#include <ttb/core/RenderTarget.hpp>
#include <ttb/core/window/WindowMode.hpp>
#include <ttb/utils/dataIO.hpp>

#include <memory>
#include <string>


// declarations
//=============================================================================

namespace ttb
{
    class Window : public RenderTarget
    {
    public:
        Window( std::string const& title, WindowMode const& mode );

        ~Window();

        void mode( WindowMode const& mode );
        WindowMode const& mode() const;

        std::string const& title() const;

        // Event handling
        std::shared_ptr< Provider< SlotType::ACTIVE, Event const& > > eventOutput() const;
        void update();

        // Override: RenderTarget
        virtual size_t width() const override;
        virtual size_t height() const override;
        virtual void begin( State& state ) const override;
        virtual void end( State& state ) const override;

    private:
        class Impl;
        std::unique_ptr< Impl > m_impl;

        WindowMode m_mode;

        std::string m_title;
    };
}
