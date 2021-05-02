#include <android/log.h>
#include <jni.h>

int function()
{
    return 4;
}

extern "C"
{
    JNIEXPORT void JNICALL Java_toms_1toolbox_ApplicationLib_test_1callback( JNIEnv* /* env */,
                                                                             jobject /* obj */ )
    {
        __android_log_print( ANDROID_LOG_INFO, "com.juicy_trash.pong_royale", "Test callback" );
    }
}
