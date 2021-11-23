#include "WindowImpl.hpp"

#include <jni.h>

extern "C"
{
    // cppcheck suppress unusedFunction
    JNIEXPORT void JNICALL Java_toms_1toolbox_ApplicationLib_on_1resize( JNIEnv* /* env */,
                                                                         jobject /* obj */,
                                                                         jint width,
                                                                         jint height )
    {
        ttb::priv::WindowImpl::onResize( width, height );
    }

    // cppcheck suppress unusedFunction
    JNIEXPORT void JNICALL Java_toms_1toolbox_ApplicationLib_on_1pointer_1down(
        JNIEnv* /* env */, jobject /* obj */, jint pointerId, jfloat x, jfloat y )
    {
        ttb::priv::WindowImpl::onPointerDown( pointerId, x, y );
    }

    // cppcheck suppress unusedFunction
    JNIEXPORT void JNICALL Java_toms_1toolbox_ApplicationLib_on_1pointer_1up(
        JNIEnv* /* env */, jobject /* obj */, jint pointerId, jfloat x, jfloat y )
    {
        ttb::priv::WindowImpl::onPointerUp( pointerId, x, y );
    }

    // cppcheck suppress unusedFunction
    JNIEXPORT void JNICALL Java_toms_1toolbox_ApplicationLib_on_1pointer_1move(
        JNIEnv* /* env */, jobject /* obj */, jint pointerId, jfloat x, jfloat y )
    {
        ttb::priv::WindowImpl::onPointerMove( pointerId, x, y );
    }
}
