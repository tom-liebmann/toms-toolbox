#include <ttb/core/window.hpp>
#include <ttb/project/Application.hpp>

#include <jni.h>


namespace ttb
{
    class ApplicationRunner
    {
    public:
        static void init_gl();

        static void step();
    };
}


namespace ttb
{
    void ApplicationRunner::init_gl()
    {
        auto& app = application();

        auto const appProperties = app.getProperties();

        ttb::Window::init( "Window Title", appProperties.getWindowRequest() );
        app.init( 0, nullptr );
    }

    void ApplicationRunner::step()
    {
        application().update( 1.0f / 60.0f );

        application().draw();
    }
}


extern "C"
{
    JNIEXPORT void JNICALL Java_toms_1toolbox_ApplicationLib_init_1gl( JNIEnv* /* env */,
                                                                       jobject /* obj */ )
    {
        ttb::ApplicationRunner::init_gl();
    }

    JNIEXPORT void JNICALL Java_toms_1toolbox_ApplicationLib_step( JNIEnv* /* env */,
                                                                   jobject /* obj */ )
    {
        ttb::ApplicationRunner::step();
    }
}
