#include "ObjectBase.hpp"

#include <stdexcept>
#include <string>


namespace android
{
    ObjectBase::ObjectBase( JavaVM* vm ) : m_vm{ vm }
    {
    }

    JNIEnv& ObjectBase::getEnv()
    {
        JNIEnv* env;

        switch( m_vm->GetEnv( (void**)&env, JNI_VERSION_1_6 ) )
        {
            case JNI_OK:
                break;

            case JNI_EDETACHED:
            {
                if( m_vm->AttachCurrentThread( &env, NULL ) != 0 )
                {
                    throw std::runtime_error( "Attachment failed" );
                }
                break;
            }

            case JNI_EVERSION:
                throw std::runtime_error( "Version not supported" );
        }

        return *env;
    }

    jclass ObjectBase::findClass( JNIEnv& env, char const* name )
    {
        auto const cls = env.FindClass( name );

        if( !cls )
        {
            throw std::runtime_error( "Unable to find class: " + std::string( name ) );
        }

        return cls;
    }

    jmethodID
        ObjectBase::findMethod( JNIEnv& env, jclass cls, char const* name, char const* signature )
    {
        auto const mtd = env.GetMethodID( cls, name, signature );

        if( !mtd )
        {
            throw std::runtime_error( "Unable to find method: " + std::string( name ) );
        }

        return mtd;
    }

    jmethodID ObjectBase::findStaticMethod( JNIEnv& env,
                                            jclass cls,
                                            char const* name,
                                            char const* signature )
    {
        auto const mtd = env.GetStaticMethodID( cls, name, signature );

        if( !mtd )
        {
            throw std::runtime_error( "Unable to find static method: " + std::string( name ) );
        }

        return mtd;
    }
}
