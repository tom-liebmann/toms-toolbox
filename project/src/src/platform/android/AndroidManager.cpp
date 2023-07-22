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

        bool isAdManagerInitialized();

        void initializeAdManager();

        void runRewardedAd( std::string const& id );

    private:
        AndroidManagerImpl();

        JNIEnv* getEnv();

        JavaVM* m_javaVm;

        jobject m_activity;
        jobject m_connectionManager;
        jobject m_adManager;

        jmethodID m_enableNetworkCheckMethod;
        jmethodID m_disableNetworkCheckMethod;
        jmethodID m_isNetworkCheckEnabledMethod;
        jmethodID m_isNetworkAvailableMethod;

        jmethodID m_isAdManagerInitializedMethod;
        jmethodID m_initializeAdManagerMethod;
        jmethodID m_runRewardedAdMethod;
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

bool AndroidManager::isAdManagerInitialized()
{
    return AndroidManagerImpl::getInstance().isAdManagerInitialized();
}

void AndroidManager::initializeAdManager()
{
    AndroidManagerImpl::getInstance().initializeAdManager();
}

void AndroidManager::runRewardedAd( std::string const& id )
{
    AndroidManagerImpl::getInstance().runRewardedAd( id );
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

        auto const env = getEnv();

        auto const activityClass = findClass( *env, "toms_toolbox/MainActivity" );
        auto const connectionManagerClass = findClass( *env, "toms_toolbox/ConnectionManager" );
        auto const adManagerClass = findClass( *env, "toms_toolbox/AdManager" );

        // Get activity object
        {
            auto const activityGetInstanceMethod = findStaticMethod(
                *env, activityClass, "getInstance", "()Ltoms_toolbox/MainActivity;" );

            m_activity = env->NewGlobalRef(
                env->CallStaticObjectMethod( activityClass, activityGetInstanceMethod ) );
        }

        // Get connection manager
        {
            auto const activityGetConnectionManagerMethod = findMethod(
                *env, activityClass, "getConnectionManager", "()Ltoms_toolbox/ConnectionManager;" );

            m_connectionManager = env->NewGlobalRef(
                env->CallObjectMethod( m_activity, activityGetConnectionManagerMethod ) );
        }

        // Get ad manager
        {
            auto const activityGetAdManagerMethod =
                findMethod( *env, activityClass, "getAdManager", "()Ltoms_toolbox/AdManager;" );

            m_adManager = env->NewGlobalRef(
                env->CallObjectMethod( m_activity, activityGetAdManagerMethod ) );
        }

        // Get connection manager methods
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

        // Get ad manager methods
        {
            m_isAdManagerInitializedMethod =
                findMethod( *env, adManagerClass, "isInitialized", "()Z" );

            m_initializeAdManagerMethod = findMethod( *env, adManagerClass, "initialize", "()V" );

            m_runRewardedAdMethod = findMethod( *env, adManagerClass, "runRewardedAd", "(Ljava/lang/String;)V" );
        }
    }

    void AndroidManagerImpl::destroy()
    {
        auto const env = getEnv();

        env->DeleteGlobalRef( m_activity );
        env->DeleteGlobalRef( m_connectionManager );
        env->DeleteGlobalRef( m_adManager );
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

    bool AndroidManagerImpl::isAdManagerInitialized()
    {
        auto const env = getEnv();
        return env->CallBooleanMethod( m_adManager, m_isAdManagerInitializedMethod );
    }

    void AndroidManagerImpl::initializeAdManager()
    {
        auto const env = getEnv();
        env->CallVoidMethod( m_adManager, m_initializeAdManagerMethod );
    }

    void AndroidManagerImpl::runRewardedAd( std::string const& id )
    {
        auto const env = getEnv();
        auto const jId = env->NewStringUTF( id.c_str() );
        env->CallVoidMethod( m_adManager, m_runRewardedAdMethod, jId );
        env->DeleteLocalRef( jId );
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
