#include <ttb/core/window/Window.hpp>

#include <ttb/core/window/Window.hpp>

#include <SDL2/SDL.h>

#undef linux


namespace ttb
{
    namespace emscripten
    {
        class Window : public ttb::Window
        {
        public:
            Window( std::string const& title, WindowMode const& mode );

            ~Window();

            virtual PushOutput< Event const& >& eventOutput() override;

            virtual void update() override;

        private:
            static size_t s_windowCount;

            std::unique_ptr< PushOutput< Event const& > > m_eventOutput;

            SDL_Window* m_handle;
            SDL_GLContext m_context;
        };
    }
}
