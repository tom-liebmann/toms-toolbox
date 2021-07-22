#include <ttb/core/window.hpp>
#include <ttb/project/Application.hpp>

#include <chrono>
#include <cstdlib>


namespace ttb
{
    class ApplicationRunner
    {
    public:
        static void run();
    };
}


int main( int /* argc */, char** /* argv */ )
{
    ttb::ApplicationRunner::run();

    return EXIT_SUCCESS;
}


namespace ttb
{
    void ApplicationRunner::run()
    {
        auto& app = application();

        ttb::Window::init( "Window Title", app.windowRequest() );
        auto& window = ttb::Window::instance();

        window.use();

        app.init();

        auto lastTime = std::chrono::steady_clock::now();

        while( !app.stopped() )
        {
            window.pollEvents();

            auto const now = std::chrono::steady_clock::now();
            auto const frameTime =
                std::chrono::duration_cast< std::chrono::duration< float > >( now - lastTime )
                    .count();
            lastTime = now;

            app.update( frameTime );

            app.draw();
        }

        window.unuse();

        app.destroy();
    }
}
