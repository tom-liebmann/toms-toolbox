#include "SignInManager.hpp"


namespace android
{
    SignInManager::SignInManager( JavaVM* vm, jobject object )
        : ObjectBase{ vm }, m_object{ object }
    {
        auto& env = getEnv();

        auto const cls = findClass( env, "toms_toolbox/SignInManager" );
        m_isSignedInMethod = findMethod( env, cls, "isSignedIn", "()Z" );
        m_performSilentSignInMethod = findMethod( env, cls, "performSilentSignIn", "()V" );
        m_performExplicitSignInMethod = findMethod( env, cls, "performExplicitSignIn", "()V" );
        m_generateServerTokenMethod =
            findMethod( env, cls, "generateServerToken", "(Ljava/lang/String;)V" );
    }

    SignInManager::~SignInManager()
    {
        auto& env = getEnv();
        env.DeleteGlobalRef( m_object );
    }

    bool SignInManager::isSignedIn()
    {
        auto& env = getEnv();
        return env.CallBooleanMethod( m_object, m_isSignedInMethod );
    }

    void SignInManager::performSilentSignIn()
    {
        auto& env = getEnv();
        env.CallVoidMethod( m_object, m_performSilentSignInMethod );
    }

    void SignInManager::performExplicitSignIn()
    {
        auto& env = getEnv();
        env.CallVoidMethod( m_object, m_performExplicitSignInMethod );
    }

    void SignInManager::generateServerToken( std::string const& token )
    {
        auto& env = getEnv();
        auto const jToken = env.NewStringUTF( token.c_str() );
        env.CallVoidMethod( m_object, m_generateServerTokenMethod, jToken );
        env.DeleteLocalRef( jToken );
    }
}
