#include <ttb/core/window.hpp>
#include <ttb/project/Application.hpp>

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

        while( !app.stopped() )
        {
            window.pollEvents();

            app.update( 1.0f );

            app.draw();
        }

        window.unuse();

        app.destroy();
    }
}
