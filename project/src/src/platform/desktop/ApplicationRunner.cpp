#include <ttb/core/window.hpp>
#include <ttb/project/Application.hpp>

#include <chrono>
#include <cstdlib>
#include <thread>


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

            lastTime += std::chrono::duration_cast< std::chrono::steady_clock::duration >(
                std::chrono::duration< float >{ 1.0f / 60.0f } );
            std::this_thread::sleep_until( lastTime );

            app.update( 1.0f / 60.0f );

            app.draw();
        }

        window.unuse();

        app.destroy();
    }
}
