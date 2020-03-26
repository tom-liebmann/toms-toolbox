#pragma once

#include <ttb/core/Context.hpp>
#include <ttb/core/RenderTarget.hpp>
#include <ttb/utils/Event.hpp>
#include <ttb/utils/signal.hpp>

#include <memory>
#include <string>


// declarations
//=============================================================================

namespace ttb
{
    class Window : public Context, public RenderTarget
    {
    public:
        enum class Flag : uint32_t
        {
            NONE = 0,
            FULLSCREEN = 1 << 0,
            FLOATING = 1 << 1,
            RESIZABLE = 1 << 2,
            VSYNC = 1 << 3,
            HIDDEN = 1 << 4,
        };

        class Mode
        {
        public:
            Mode( uint16_t width, uint16_t height, Flag flags = Flag::NONE );

            uint16_t width() const;

            uint16_t height() const;

            bool flag( Flag value ) const;

            Flag flags() const;

        private:
            uint16_t m_width;
            uint16_t m_height;
            Flag m_flags;
        };

#ifndef MODE_ANDROID

        static std::shared_ptr< Window > create( std::string const& title, Mode const& mode );

        static std::shared_ptr< Window >
            create( std::string const& title, Mode const& mode, Window const& sharedWindow );

#endif

        virtual ~Window();

        void mode( Mode const& mode );

        Mode const& mode() const;

        std::string const& title() const;

        // Event handling
        virtual Signal< void( Event const& ) >& eventOutput() = 0;
        virtual void update() = 0;
        virtual void resize( uint16_t width, uint16_t height ) = 0;

        // Override: RenderTarget
        virtual size_t width() const override;
        virtual size_t height() const override;
        virtual void begin( State& state ) const override;
        virtual void end( State& state ) const override;

    protected:
        Window( std::string const& title, Mode const& mode );

    private:
        Mode m_mode;

        std::string m_title;
    };


    Window::Flag operator|( Window::Flag lhs, Window::Flag rhs );
}
