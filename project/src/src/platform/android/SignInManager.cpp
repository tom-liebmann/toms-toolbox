#include <ttb/project/SignInManager.hpp>

#include "AndroidManager.hpp"
#include "android/SignInManager.hpp"


namespace ttb
{
    namespace
    {
        class SignInManagerImpl
        {
        public:
            static SignInManagerImpl& getInstance();

            ttb::co::Coroutine< bool > performSilentSignIn();

            ttb::co::Coroutine< bool > performExplicitSignIn();

            ttb::co::Coroutine< std::optional< std::string > >
                generateServerToken( std::string const& serverId );

            void reportSignInResult( bool success );

            void reportServerTokenResult( std::optional< std::string > token );

        private:
            bool m_isSignInActive{ false };
            bool m_signInResult{ false };

            bool m_isServerTokenActive{ false };
            std::optional< std::string > m_serverTokenResult;

            std::mutex m_mutex;
        };
    }
}


namespace ttb
{
    SignInManager& SignInManager::getInstance()
    {
        static SignInManager instance;
        return instance;
    }

    ttb::co::Coroutine< bool > SignInManager::performSilentSignIn()
    {
        return SignInManagerImpl::getInstance().performSilentSignIn();
    }

    ttb::co::Coroutine< bool > SignInManager::performExplicitSignIn()
    {
        return SignInManagerImpl::getInstance().performExplicitSignIn();
    }

    ttb::co::Coroutine< std::optional< std::string > >
        SignInManager::generateServerToken( std::string const& serverId )
    {
        return SignInManagerImpl::getInstance().generateServerToken( serverId );
    }
}

namespace ttb
{
    namespace
    {
        SignInManagerImpl& SignInManagerImpl::getInstance()
        {
            static SignInManagerImpl instance;
            return instance;
        }

        ttb::co::Coroutine< bool > SignInManagerImpl::performSilentSignIn()
        {
            auto lock = std::unique_lock{ m_mutex };

            if( !m_isSignInActive )
            {
                m_isSignInActive = true;

                lock.unlock();
                auto& signInManager = AndroidManager::getInstance().getSignInManager();
                signInManager.performSilentSignIn();
                lock.lock();
            }

            while( m_isSignInActive )
            {
                lock.unlock();
                co_await ::co::suspend_always{};
                lock.lock();
            }

            co_return m_signInResult;
        }

        ttb::co::Coroutine< bool > SignInManagerImpl::performExplicitSignIn()
        {
            if( auto const silentSignInResult = co_await performSilentSignIn() )
            {
                co_return silentSignInResult;
            }

            auto lock = std::unique_lock{ m_mutex };

            if( !m_isSignInActive )
            {
                m_isSignInActive = true;

                lock.unlock();
                auto& signInManager = AndroidManager::getInstance().getSignInManager();
                signInManager.performExplicitSignIn();
                lock.lock();
            }

            while( m_isSignInActive )
            {
                lock.unlock();
                co_await ::co::suspend_always{};
                lock.lock();
            }

            co_return m_signInResult;
        }

        ttb::co::Coroutine< std::optional< std::string > >
            SignInManagerImpl::generateServerToken( std::string const& serverId )
        {
            auto lock = std::unique_lock{ m_mutex };

            if( !m_isServerTokenActive )
            {
                m_isServerTokenActive = true;

                lock.unlock();
                auto& signInManager = AndroidManager::getInstance().getSignInManager();
                signInManager.generateServerToken( serverId );
                lock.lock();
            }

            while( m_isServerTokenActive )
            {
                lock.unlock();
                co_await ::co::suspend_always{};
                lock.lock();
            }

            co_return m_serverTokenResult;
        }

        void SignInManagerImpl::reportSignInResult( bool success )
        {
            auto const lock = std::lock_guard{ m_mutex };
            m_isSignInActive = false;
            m_signInResult = success;
        }

        void SignInManagerImpl::reportServerTokenResult( std::optional< std::string > token )
        {
            auto const lock = std::lock_guard{ m_mutex };
            m_isServerTokenActive = false;
            m_serverTokenResult = std::move( token );
        }
    }
}


extern "C"
{
    // cppcheck suppress unusedFunction
    JNIEXPORT void JNICALL Java_toms_1toolbox_ApplicationLib_on_1silent_1sign_1in_1success(
        JNIEnv* /* env */, jclass /* cls */ )
    {
        ttb::SignInManagerImpl::getInstance().reportSignInResult( true );
    }

    // cppcheck suppress unusedFunction
    JNIEXPORT void JNICALL Java_toms_1toolbox_ApplicationLib_on_1silent_1sign_1in_1failure(
        JNIEnv* /* env */, jclass /* cls */ )
    {
        ttb::SignInManagerImpl::getInstance().reportSignInResult( false );
    }

    // cppcheck suppress unusedFunction
    JNIEXPORT void JNICALL Java_toms_1toolbox_ApplicationLib_on_1explicit_1sign_1in_1success(
        JNIEnv* /* env */, jclass /* cls */ )
    {
        ttb::SignInManagerImpl::getInstance().reportSignInResult( true );
    }

    // cppcheck suppress unusedFunction
    JNIEXPORT void JNICALL Java_toms_1toolbox_ApplicationLib_on_1explicit_1sign_1in_1failure(
        JNIEnv* /* env */, jclass /* cls */ )
    {
        ttb::SignInManagerImpl::getInstance().reportSignInResult( false );
    }

    JNIEXPORT void JNICALL Java_toms_1toolbox_ApplicationLib_play_1on_1server_1token_1success(
        JNIEnv* env, jclass /* cls */, jstring token )
    {
        auto const tokenStr = std::string{ env->GetStringUTFChars( token, NULL ) };
        ttb::SignInManagerImpl::getInstance().reportServerTokenResult( tokenStr );
    }

    JNIEXPORT void JNICALL Java_toms_1toolbox_ApplicationLib_play_1on_1server_1token_1failure(
        JNIEnv* /* env */, jclass /* cls */ )
    {
        ttb::SignInManagerImpl::getInstance().reportServerTokenResult( {} );
    }
}
