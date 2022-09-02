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
        static void run( int argc, char** argv );
    };
}


int main( int argc, char** argv )
{
    ttb::ApplicationRunner::run( argc, argv );

    return EXIT_SUCCESS;
}


namespace ttb
{
    void ApplicationRunner::run( int argc, char** argv )
    {
        auto& app = application();

        auto const appProperties = app.getProperties();

        ttb::Window::init( "Window Title", appProperties.getWindowRequest() );
        auto& window = ttb::Window::instance();

        window.use();

        app.init( argc, argv );

        auto lastTime = std::chrono::steady_clock::now();

        while( !app.stopped() )
        {
            window.pollEvents();

            if( appProperties.getLimitFps() )
            {
                lastTime += std::chrono::duration_cast< std::chrono::steady_clock::duration >(
                    std::chrono::duration< float >{ 1.0f / 60.0f } );
                std::this_thread::sleep_until( lastTime );

                app.update( 1.0f / 60.0f );
            }
            else
            {
                auto const now = std::chrono::steady_clock::now();
                auto const frameTime =
                    std::chrono::duration_cast< std::chrono::duration< float > >( now - lastTime )
                        .count();
                app.update( frameTime );
            }

            app.draw();
        }

        window.unuse();

        app.destroy();
    }
}
