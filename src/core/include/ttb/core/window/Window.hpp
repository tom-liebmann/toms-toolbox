#pragma once

#include "WindowMode.hpp"

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
        class Impl;

        using EventCallback = std::function< void( Event const& ) >;

        ~Window();

        WindowMode const& mode() const;

        std::string const& title() const;

        void eventCallback( EventCallback callback );

        void resize( uint16_t width, uint16_t height );

        void update();

        // Override: Context
        virtual bool use() override;
        virtual bool unuse() override;

    private:
        Window( std::unique_ptr< Impl > impl );

        Window( Window const& rhs ) = delete;
        Window( Window&& rhs ) = delete;
        Window& operator=( Window const& rhs ) = delete;
        Window& operator=( Window&& rhs ) = delete;

        // Override: RenderTarget
        virtual size_t width() const override;
        virtual size_t height() const override;
        virtual void begin( State& state ) const override;
        virtual void end( State& state ) const override;

        std::unique_ptr< Impl > m_impl;

        friend class WindowCreator;
    };
}
