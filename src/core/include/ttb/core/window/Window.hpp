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
        class Impl;

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

        using EventOutput = Signal< void( Event const& ) >;

        Mode const& mode() const;

        std::string const& title() const;

        Impl& impl();

        Impl const& impl() const;

        // Event handling
        EventOutput& eventOutput();

        // Override: RenderTarget
        virtual size_t width() const override;
        virtual size_t height() const override;
        virtual void begin( State& state ) const override;
        virtual void end( State& state ) const override;

        // Override: Context
        virtual bool use() override;
        virtual bool unuse() override;

#if defined( MODE_ANDROID )

    public:
        static void init( std::string const& title, uint16_t width, uint16_t height );

        static std::shared_ptr< Window > instance();

#elif defined( MODE_GCC )

    public:
        static std::shared_ptr< Window > create( std::string const& title, Mode const& mode );

        static std::shared_ptr< Window >
            create( std::string const& title, Mode const& mode, Window const& sharedWindow );

        void resize( uint16_t width, uint16_t height );

        void update();

#endif

    private:
        Window( std::unique_ptr< Impl > impl );

        std::unique_ptr< Impl > m_impl;
    };


    Window::Flag operator|( Window::Flag lhs, Window::Flag rhs );
}


namespace ttb
{
    inline Window::Impl& Window::impl()
    {
        return *m_impl;
    }

    inline Window::Impl const& Window::impl() const
    {
        return *m_impl;
    }


    inline Window::Mode::Mode( uint16_t width, uint16_t height, Flag flags )
        : m_width{ width }, m_height{ height }, m_flags{ flags }
    {
    }

    inline uint16_t Window::Mode::width() const
    {
        return m_width;
    }

    inline uint16_t Window::Mode::height() const
    {
        return m_height;
    }

    inline bool Window::Mode::flag( Flag value ) const
    {
        return static_cast< uint32_t >( m_flags ) & static_cast< uint32_t >( value );
    }

    inline Window::Flag Window::Mode::flags() const
    {
        return m_flags;
    }


    inline Window::Flag operator|( Window::Flag lhs, Window::Flag rhs )
    {
        return static_cast< Window::Flag >( static_cast< uint32_t >( lhs ) |
                                            static_cast< uint32_t >( rhs ) );
    }
}
