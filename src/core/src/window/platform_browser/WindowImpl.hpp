#pragma once

#include <ttb/core/window/Window.hpp>

#include <ttb/core/gl.hpp>

#include <SDL/SDL.h>


namespace ttb
{
    class Window::Impl
    {
    public:
        ~Impl();

        WindowMode const& mode() const;

        std::string const& title() const;

        void window( Window& window );

        void eventCallback( EventCallback callback );

        void resize( uint16_t width, uint16_t height );

        void begin( State& state );

        void end( State& state );

        void pushEvent( Event const& event );

        void update();

    private:
        Impl( std::string title, WindowMode const& mode );

        EventCallback m_eventCallback;
        WindowMode m_mode;
        std::string m_title;

        Window* m_window;

        SDL_Window* m_handle;
        SDL_GLContext m_context;

        friend class WindowCreator;
    };
}
