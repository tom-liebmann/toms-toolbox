#pragma once

#include "WindowRequest.hpp"
#include <ttb/core/Context.hpp>
#include <ttb/core/RenderTarget.hpp>
#include <ttb/utils/Event.hpp>

#include <functional>
#include <memory>
#include <string>


namespace ttb
{
    class Window : public Context, public RenderTarget
    {
    public:
        using Size = ttb::Vector< uint16_t, 2 >;
        using EventCallback = std::function< void( Event const& ) >;

        static void init( std::string_view title, WindowRequest const& request );

        static Window& instance();

        std::string_view title() const;

        Size const& size() const;

        bool flag( WindowFlag value ) const;

        void eventCallback( EventCallback callback );

        virtual void pollEvents() = 0;

        // Override: Context
        virtual bool use() override = 0;
        virtual bool unuse() override = 0;

    protected:
        Window( std::string_view title, Size const& size, WindowFlag flags );

        void pushEvent( Event const& event );

        std::string m_title;
        Size m_size;
        WindowFlag m_flags;

    private:
        Window( Window const& rhs ) = delete;
        Window( Window&& rhs ) = delete;
        Window& operator=( Window const& rhs ) = delete;
        Window& operator=( Window&& rhs ) = delete;

        // Override: RenderTarget
        virtual size_t width() const override;
        virtual size_t height() const override;
        virtual void begin( State& state ) const override = 0;
        virtual void end( State& state ) const override = 0;

        EventCallback m_eventCallback;
    };
}
