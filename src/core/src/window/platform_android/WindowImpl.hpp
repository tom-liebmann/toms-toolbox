#pragma once

#include <ttb/core/window/Window.hpp>

#include <ttb/core/gl.hpp>


namespace ttb
{
    class Window::Impl
    {
    public:
        static void onResize( uint16_t width, uint16_t height );
        static void onPointerDown( int pointerId, float x, float y );
        static void onPointerUp( int pointerId, float x, float y );
        static void onPointerMove( int poitnerId, float x, float y );

        WindowMode const& mode() const;

        std::string const& title() const;

        void window( Window& window );

        void eventCallback( EventCallback callback );

        void resize( uint16_t width, uint16_t height );

        void begin( State& state );

        void end( State& state );

        void pushEvent( Event const& event );

    private:
        Impl( std::string title, WindowMode const& mode );

        EventCallback m_eventCallback;
        WindowMode m_mode;
        std::string m_title;

        Window* m_window;

        friend class WindowCreator;
    };
}
