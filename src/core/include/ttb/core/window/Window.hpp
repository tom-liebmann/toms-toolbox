#pragma once

#include <ttb/core/RenderTarget.hpp>
#include <ttb/core/window/WindowMode.hpp>
#include <ttb/utils/Event.hpp>
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
        static std::shared_ptr< Window > create( std::string const& title, WindowMode const& mode );

        virtual ~Window();

        void mode( WindowMode const& mode );

        WindowMode const& mode() const;

        std::string const& title() const;

        // Event handling
        virtual PushOutput< Event const& >& eventOutput() = 0;

        virtual void update() = 0;

        // Override: RenderTarget
        virtual size_t width() const override;

        virtual size_t height() const override;

        virtual void begin( State& state ) const override;

        virtual void end( State& state ) const override;

    protected:
        Window( std::string const& title, WindowMode const& mode );

    private:
        WindowMode m_mode;

        std::string m_title;
    };
}
