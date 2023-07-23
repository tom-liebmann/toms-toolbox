#pragma once

#include <jni.h>


namespace android
{
    class ObjectBase
    {
    public:
        ObjectBase( JavaVM* vm );

    protected:
        JNIEnv& getEnv();

        jclass findClass( JNIEnv& env, char const* name );

        jmethodID findMethod( JNIEnv& env, jclass cls, char const* name, char const* signature );

        jmethodID
            findStaticMethod( JNIEnv& env, jclass cls, char const* name, char const* signature );

        JavaVM* m_vm;
    };
}
