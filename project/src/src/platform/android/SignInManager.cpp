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

            ttb::co::Coroutine< std::optional< std::string > > performSilentSignIn();

            ttb::co::Coroutine< std::optional< std::string > > performExplicitSignIn();

            void reportSignInResult( std::optional< std::string > result );

        private:
            bool m_isSignInActive{ false };
            std::optional< std::string > m_signInResult;

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

    ttb::co::Coroutine< std::optional< std::string > > SignInManager::performSilentSignIn()
    {
        return SignInManagerImpl::getInstance().performSilentSignIn();
    }

    ttb::co::Coroutine< std::optional< std::string > > SignInManager::performExplicitSignIn()
    {
        return SignInManagerImpl::getInstance().performExplicitSignIn();
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

        ttb::co::Coroutine< std::optional< std::string > > SignInManagerImpl::performSilentSignIn()
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

        ttb::co::Coroutine< std::optional< std::string > >
            SignInManagerImpl::performExplicitSignIn()
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

        void SignInManagerImpl::reportSignInResult( std::optional< std::string > result )
        {
            auto const lock = std::lock_guard{ m_mutex };
            m_isSignInActive = false;
            m_signInResult = std::move( result );
        }
    }
}


extern "C"
{
    // cppcheck suppress unusedFunction
    JNIEXPORT void JNICALL Java_toms_1toolbox_ApplicationLib_on_1silent_1sign_1in_1success(
        JNIEnv* env, jclass /* cls */, jstring tokenId )
    {
        auto const tokenStr = std::string{ env->GetStringUTFChars( tokenId, NULL ) };
        ttb::SignInManagerImpl::getInstance().reportSignInResult( tokenStr );
    }

    // cppcheck suppress unusedFunction
    JNIEXPORT void JNICALL Java_toms_1toolbox_ApplicationLib_on_1silent_1sign_1in_1failure(
        JNIEnv* /* env */, jclass /* cls */ )
    {
        ttb::SignInManagerImpl::getInstance().reportSignInResult( {} );
    }

    // cppcheck suppress unusedFunction
    JNIEXPORT void JNICALL Java_toms_1toolbox_ApplicationLib_on_1explicit_1sign_1in_1success(
        JNIEnv* env, jclass /* cls */, jstring tokenId )
    {
        auto const tokenStr = std::string{ env->GetStringUTFChars( tokenId, NULL ) };
        ttb::SignInManagerImpl::getInstance().reportSignInResult( tokenStr );
    }

    // cppcheck suppress unusedFunction
    JNIEXPORT void JNICALL Java_toms_1toolbox_ApplicationLib_on_1explicit_1sign_1in_1failure(
        JNIEnv* /* env */, jclass /* cls */ )
    {
        ttb::SignInManagerImpl::getInstance().reportSignInResult( {} );
    }
}
