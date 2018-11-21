#pragma once

#include <ttb/core/RenderTarget.hpp>
#include <ttb/utils/Event.hpp>
#include <ttb/utils/signal.hpp>

#include <memory>
#include <string>


// declarations
//=============================================================================

namespace ttb
{
    class Window : public RenderTarget
    {
    public:
        enum class Flag : uint32_t
        {
            NONE,
            FULLSCREEN,
            FLOATING,
            RESIZABLE
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

        static std::shared_ptr< Window > create( std::string const& title, Mode const& mode );

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
