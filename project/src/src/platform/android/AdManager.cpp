#include <ttb/project/AdManager.hpp>

#include <mutex>

namespace ttb
{
    class AdManagerImpl
    {
    public:
        static AdManagerImpl& getInstance();

        ttb::co::Coroutine< void > init();

        ttb::co::Coroutine< bool > playAd( std::string const& id );

        void reportResult( bool result );

    private:
        bool m_adRunning{ false };
        bool m_adResult{ false };

        std::mutex m_mutex;
    };
}


namespace ttb
{
    AdManager& AdManager::getInstance()
    {
        static AdManager instance;
        return instance;
    }

    ttb::co::Coroutine< void > AdManager::init()
    {
        auto& android = AndroidManager::getInstance();

        android.initializeAdManager();

        while( !android.isAdManagerInitialized() )
        {
            co_await std::suspend_always;
        }
    }

    ttb::co::Coroutine< bool > AdManager::playAd( std::string const& id )
    {
        return AdManagerImpl::getInstance().playAd( id );
    }


    AdManagerImpl& AdManagerImpl::getInstance()
    {
        static AdManagerImpl instance;
        return instance;
    }

    ttb::co::Coroutine< bool > AdManagerImpl::playAd( std::string const& id )
    {
        auto lock = std::unique_lock{ m_mutex };

        if( m_adRunning )
        {
            // An ad is already running
            co_return false;
        }

        co_await init();

        auto& android = AndroidManager::getInstance();

        m_adRunning = true;
        android.runRewardedAd( id );

        while( m_adRunning )
        {
            lock.unlock();
            co_await std::suspend_always;
            lock.lock();
        }

        co_return m_adResult;
    }

    void AdManagerImpl::reportResult( bool result )
    {
        auto const lock = std::lock_guard{ m_mutex };
        m_adRunning = false;
        m_adResult = result;
    }
}


extern "C"
{
    // cppcheck suppress unusedFunction
    JNIEXPORT void JNICALL Java_toms_1toolbox_ApplicationLib_on_1ad_1init_1complete(
        JNIEnv* /* env */, jclass /* cls */ )
    {
        // Initialization status is currently polled. This callback is, therefore, not needed.
    }

    // cppcheck suppress unusedFunction
    JNIEXPORT void JNICALL Java_toms_1toolbox_ApplicationLib_on_1ad_1load_1failed(
        JNIEnv* /* env */, jclass /* cls */ )
    {
        ttb::AdManagerImpl::getInstance().reportResult( false );
    }

    // cppcheck suppress unusedFunction
    JNIEXPORT void JNICALL Java_toms_1toolbox_ApplicationLib_on_1ad_1reward_1earned(
        JNIEnv* /* env */, jclass /* cls */, jint /* amount */, jobject /* type */ )
    {
        ttb::AdManagerImpl::getInstance().reportResult( true );
    }
}
