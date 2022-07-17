#include "AndroidManager.hpp"

#include <ttb/core/Logger.hpp>

#include <jni.h>


namespace
{
    class AndroidManagerImpl
    {
    public:
        static AndroidManagerImpl& getInstance();

        void init( JavaVM* javaVm );

        void destroy();

        void enableNetworkCheck();

        void disableNetworkCheck();

        bool isNetworkCheckEnabled();

        bool isNetworkAvailable();

    private:
        AndroidManagerImpl();

        JNIEnv* getEnv();

        JavaVM* m_javaVm;

        jobject m_activity;
        jobject m_connectionManager;

        jmethodID m_enableNetworkCheckMethod;
        jmethodID m_disableNetworkCheckMethod;
        jmethodID m_isNetworkCheckEnabledMethod;
        jmethodID m_isNetworkAvailableMethod;
    };

    jclass findClass( JNIEnv& env, char const* name );

    jmethodID findMethod( JNIEnv& env, jclass cls, char const* name, char const* signature );

    jmethodID findStaticMethod( JNIEnv& env, jclass cls, char const* name, char const* signature );
}


AndroidManager& AndroidManager::getInstance()
{
    static auto instance = AndroidManager{};
    return instance;
}

void AndroidManager::enableNetworkCheck()
{
    AndroidManagerImpl::getInstance().enableNetworkCheck();
}

void AndroidManager::disableNetworkCheck()
{
    AndroidManagerImpl::getInstance().disableNetworkCheck();
}

bool AndroidManager::isNetworkCheckEnabled()
{
    return AndroidManagerImpl::getInstance().isNetworkCheckEnabled();
}

bool AndroidManager::isNetworkAvailable()
{
    return AndroidManagerImpl::getInstance().isNetworkAvailable();
}

AndroidManager::AndroidManager() = default;


namespace
{
    AndroidManagerImpl& AndroidManagerImpl::getInstance()
    {
        static auto instance = AndroidManagerImpl{};
        return instance;
    }

    void AndroidManagerImpl::init( JavaVM* javaVm )
    {
        m_javaVm = javaVm;

        ttb::logger( 'E' ) << "initializing" << std::endl;

        auto const env = getEnv();

        auto const activityClass = findClass( *env, "toms_toolbox/MainActivity" );
        auto const connectionManagerClass = findClass( *env, "toms_toolbox/ConnectionManager" );

        ttb::logger( 'E' ) << "got classes" << std::endl;

        // Get activity object
        {
            auto const activityGetInstanceMethod = findStaticMethod(
                *env, activityClass, "getInstance", "()Ltoms_toolbox/MainActivity;" );

            m_activity = env->NewGlobalRef(
                env->CallStaticObjectMethod( activityClass, activityGetInstanceMethod ) );
        }

        ttb::logger( 'E' ) << "got activity" << std::endl;

        // Get connection manager
        {
            auto const activityGetConnectionManagerMethod = findMethod(
                *env, activityClass, "getConnectionManager", "()Ltoms_toolbox/ConnectionManager;" );

            m_connectionManager = env->NewGlobalRef(
                env->CallObjectMethod( m_activity, activityGetConnectionManagerMethod ) );
        }

        ttb::logger( 'E' ) << "got connection manager" << std::endl;

        // Get methods
        {
            m_enableNetworkCheckMethod =
                findMethod( *env, connectionManagerClass, "enableNetworkCheck", "()V" );

            m_disableNetworkCheckMethod =
                findMethod( *env, connectionManagerClass, "disableNetworkCheck", "()V" );

            m_isNetworkCheckEnabledMethod =
                findMethod( *env, connectionManagerClass, "isNetworkCheckEnabled", "()Z" );

            m_isNetworkAvailableMethod =
                findMethod( *env, connectionManagerClass, "isNetworkAvailable", "()Z" );
        }

        ttb::logger( 'E' ) << "got methods" << std::endl;
    }

    void AndroidManagerImpl::destroy()
    {
        auto const env = getEnv();

        env->DeleteGlobalRef( m_activity );
        env->DeleteGlobalRef( m_connectionManager );
    }

    void AndroidManagerImpl::enableNetworkCheck()
    {
        auto const env = getEnv();
        env->CallVoidMethod( m_connectionManager, m_enableNetworkCheckMethod );
    }

    void AndroidManagerImpl::disableNetworkCheck()
    {
        auto const env = getEnv();
        env->CallVoidMethod( m_connectionManager, m_disableNetworkCheckMethod );
    }

    bool AndroidManagerImpl::isNetworkCheckEnabled()
    {
        auto const env = getEnv();
        return env->CallBooleanMethod( m_connectionManager, m_isNetworkCheckEnabledMethod );
    }

    bool AndroidManagerImpl::isNetworkAvailable()
    {
        auto const env = getEnv();
        return env->CallBooleanMethod( m_connectionManager, m_isNetworkAvailableMethod );
    }

    AndroidManagerImpl::AndroidManagerImpl() = default;

    JNIEnv* AndroidManagerImpl::getEnv()
    {
        JNIEnv* env;

        switch( m_javaVm->GetEnv( (void**)&env, JNI_VERSION_1_6 ) )
        {
            case JNI_OK:
                break;

            case JNI_EDETACHED:
            {
                if( m_javaVm->AttachCurrentThread( &env, NULL ) != 0 )
                {
                    throw std::runtime_error( "Attachment failed" );
                }
                break;
            }

            case JNI_EVERSION:
                throw std::runtime_error( "Version not supported" );
        }

        return env;
    }


    jclass findClass( JNIEnv& env, char const* name )
    {
        auto const cls = env.FindClass( name );

        if( !cls )
        {
            throw std::runtime_error( "Unable to find class: " + std::string( name ) );
        }

        return cls;
    }

    jmethodID findMethod( JNIEnv& env, jclass cls, char const* name, char const* signature )
    {
        auto const mtd = env.GetMethodID( cls, name, signature );

        if( !mtd )
        {
            throw std::runtime_error( "Unable to find method: " + std::string( name ) );
        }

        return mtd;
    }

    jmethodID findStaticMethod( JNIEnv& env, jclass cls, char const* name, char const* signature )
    {
        auto const mtd = env.GetStaticMethodID( cls, name, signature );

        if( !mtd )
        {
            throw std::runtime_error( "Unable to find static method: " + std::string( name ) );
        }

        return mtd;
    }
}


extern "C"
{
    JNIEXPORT void JNICALL Java_toms_1toolbox_ApplicationLib_init( JNIEnv* env, jclass /* cls */ )
    {
        JavaVM* javaVm;
        env->GetJavaVM( &javaVm );

        AndroidManagerImpl::getInstance().init( javaVm );
    }
}
